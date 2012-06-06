#include "graphframe.h"

#define NODE_SIZE 30

DEFINE_EVENT_TYPE (VivaGraphChanged)
DEFINE_EVENT_TYPE (VivaGraphLayoutUpdated)

GraphFrame::GraphFrame (wxWindow *parent,
                            wxWindowID id,
                            const wxPoint & pos,
                            const wxSize & size,
                            long style,
                            const wxString & name):wxGLCanvas(parent,id,pos,size,style,name)
{

  ratio = 1;
  translate = wxRealPoint(0.0,0.0);
  mousePosition = wxPoint(0,0);

  this->Connect(wxEVT_PAINT, wxPaintEventHandler(GraphFrame::OnPaint));
  this->Connect(wxEVT_MOTION, wxMouseEventHandler(GraphFrame::OnMouseMotion));
  this->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GraphFrame::OnMouseWheel));
  this->Connect(wxEVT_SIZE, wxSizeEventHandler(GraphFrame::OnSize));


  this->Connect(wxEVT_LEFT_DOWN,
                wxMouseEventHandler(GraphFrame::leftMouseClicked));
  this->Connect(wxEVT_RIGHT_DOWN,
                wxMouseEventHandler(GraphFrame::rightMouseClicked));

  this->Connect(VivaGraphChanged,
                wxCommandEventHandler(GraphFrame::OnVivaGraphChanged));
  this->Connect(VivaGraphLayoutUpdated,
                wxCommandEventHandler(GraphFrame::OnVivaGraphLayoutUpdated));
}

void GraphFrame::setVivaGraph (VivaGraph *vivagraph)
{
  this->vivagraph = vivagraph;
  vivagraph->setView (this);
}

wxRealPoint GraphFrame::convertPoint (wxPoint point)
{
  //detect
  GLdouble   sx,sy,sz;
  GLdouble   depth;
  GLdouble pMat[4*4];
  GLdouble   mMat[4*4];
  GLint viewport[4]; // MinX MinY MaxX MaxY

  glLoadIdentity();
  glTranslatef (translate.x, -translate.y, 0);
  glScalef (ratio, ratio, 1);
  glGetDoublev(GL_MODELVIEW_MATRIX, mMat);
  glGetDoublev(GL_PROJECTION_MATRIX, pMat);
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluUnProject((GLint)point.x, viewport[3] - (GLint)point.y,  // send the mouse coordinates
               0,
               mMat,pMat,viewport,
               &sx, &sy, &sz);
  return wxRealPoint (sx, sy);
}

void GraphFrame::OnPaint(wxPaintEvent& event)
{
  if (!vivagraph) return;

  SetCurrent();
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1, 1, 1, 0);

  glLoadIdentity ();
  glTranslatef (translate.x, -translate.y, 0);
  glScalef (ratio, ratio, 1);

  //draw edges, then the nodes themselves
  std::vector<VivaNode*>::iterator it;
  for (it = vivagraph->nodes.begin(); it != vivagraph->nodes.end(); it++){
    (*it)->drawEdges();
  }
  for (it = vivagraph->nodes.begin(); it != vivagraph->nodes.end(); it++){
    (*it)->draw();
  }

  SwapBuffers();
};



void GraphFrame::OnMouseMotion (wxMouseEvent& event)
{
  wxPoint p;
  event.GetPosition (&p.x, &p.y);

  if (event.Dragging()){
    wxPoint dif = p - mousePosition;
    translate = translate + wxRealPoint(dif.x, dif.y);
    Refresh();
  }

  mousePosition = p;
}

void GraphFrame::leftMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);
  vivagraph->leftMouseClicked (convertPoint (screen));
}


void GraphFrame::OnMouseWheel(wxMouseEvent& event)
{
  //get mouse position in the screen (in the frame to be precise)
  wxPoint mouse;
  event.GetPosition (&mouse.x, &mouse.y);

  //apply current transformations
  wxPaintDC dc(this);
  cairo_t* cr = gdk_cairo_create(dc.m_window);
  cairo_translate (cr, translate.x, translate.y);
  cairo_scale (cr, ratio, ratio);

  //transform the mouse position in the screen space to the logical space
  wxRealPoint graphPoint = wxRealPoint (mouse.x, mouse.y);
  cairo_device_to_user (cr, &graphPoint.x, &graphPoint.y);

  //calculate and apply the new user scale in the device context
  double factor = ratio * 0.1;
  if (event.GetWheelRotation() > 0){
    ratio += factor;
  }else{
    ratio -= factor;
  }
  cairo_destroy (cr);
  cr = NULL;
  cr =  gdk_cairo_create(dc.m_window);
  cairo_translate (cr, translate.x, translate.y);
  cairo_scale (cr, ratio, ratio);

  wxRealPoint aux = graphPoint;
  cairo_user_to_device (cr, &aux.x, &aux.y);
  cairo_destroy (cr);

  //calculate the difference between the two mouse screen positions
  wxRealPoint dif = wxRealPoint(mouse.x, mouse.y) - aux;
  translate = translate + dif;

  //update mousePosition
  mousePosition = mouse;
  Refresh();
}


void GraphFrame::rightMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);
  vivagraph->rightMouseClicked (convertPoint (screen));
}


void GraphFrame::OnSize (wxSizeEvent& event)
{
  wxSize size = GetSize();
  int width = size.GetWidth();
  int height = size.GetHeight();
  glViewport(0, 0, width, height);
  aspect_ratio = (double)width/(double)height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-width/2, width/2, -height/2, height/2, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  Refresh();
}


void GraphFrame::OnVivaGraphChanged (wxCommandEvent& event)
{
  Refresh();
}

void GraphFrame::OnVivaGraphLayoutUpdated (wxCommandEvent& event)
{
  Refresh();
}
