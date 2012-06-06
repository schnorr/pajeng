#include "graphframe.h"

#define NODE_SIZE 30


DEFINE_EVENT_TYPE (VivaGraphChanged)
DEFINE_EVENT_TYPE (VivaGraphLayoutUpdated)

GraphFrame::GraphFrame (wxWindow* parent,
                        wxWindowID id,
                        const wxPoint & pos,
                        const wxSize & size,
                        long style,
                        const wxString& name)
{
  Create (parent, id, pos, size, style, name);
}

bool GraphFrame::Create(wxWindow* parent,
                        wxWindowID id,
                        const wxPoint & pos,
                        const wxSize & size,
                        long style,
                        const wxString& name)
{
  if (!BasicFrame::Create(parent, id, pos, size, style, name) )
    return false;
  vivagraph = NULL;

  this->Connect(wxEVT_PAINT,
                wxPaintEventHandler(GraphFrame::OnPaint));

  this->Connect(VivaGraphChanged,
                wxCommandEventHandler(GraphFrame::OnVivaGraphChanged));
  this->Connect(VivaGraphLayoutUpdated,
                wxCommandEventHandler(GraphFrame::OnVivaGraphLayoutUpdated));

  this->Connect(wxEVT_LEFT_DOWN,
                wxMouseEventHandler(GraphFrame::leftMouseClicked));
  this->Connect(wxEVT_RIGHT_DOWN,
                wxMouseEventHandler(GraphFrame::rightMouseClicked));

  return true;
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

  //apply transformation matrix to take mouse user interaction
  wxPaintDC dc(this);
  cairo_t* cr = gdk_cairo_create(dc.m_window);
  cairo_translate (cr, translate.x, translate.y);
  cairo_scale (cr, ratio, ratio);

  //draw edges, then the nodes themselves
  std::vector<VivaNode*>::iterator it;
  for (it = vivagraph->nodes.begin(); it != vivagraph->nodes.end(); it++){
    (*it)->drawEdges(cr);
  }
  for (it = vivagraph->nodes.begin(); it != vivagraph->nodes.end(); it++){
    (*it)->draw(cr);
  }

  cairo_destroy(cr);
}

void GraphFrame::leftMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);

  //get the device context, apply the current transformations
  wxPaintDC dc(this);
  cairo_t* cr = gdk_cairo_create(dc.m_window);
  cairo_translate (cr, translate.x, translate.y);
  cairo_scale (cr, ratio, ratio);

  //transform the mouse position in the screen space to the logical space
  wxRealPoint mouseLogical = wxRealPoint (screen.x, screen.y);
  cairo_device_to_user (cr, &mouseLogical.x, &mouseLogical.y);
  cairo_destroy (cr);

  vivagraph->leftMouseClicked (wxPoint(mouseLogical.x, mouseLogical.y));
}

void GraphFrame::rightMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);

  //get the device context, apply the current transformations
  wxPaintDC dc(this);
  cairo_t* cr = gdk_cairo_create(dc.m_window);
  cairo_translate (cr, translate.x, translate.y);
  cairo_scale (cr, ratio, ratio);

  //transform the mouse position in the screen space to the logical space
  wxRealPoint mouseLogical = wxRealPoint (screen.x, screen.y);
  cairo_device_to_user (cr, &mouseLogical.x, &mouseLogical.y);
  cairo_destroy (cr);

  vivagraph->rightMouseClicked (wxPoint(mouseLogical.x, mouseLogical.y));
}

void GraphFrame::OnVivaGraphChanged (wxCommandEvent& event)
{
  Refresh();
}

void GraphFrame::OnVivaGraphLayoutUpdated (wxCommandEvent& event)
{
  Refresh();
}
