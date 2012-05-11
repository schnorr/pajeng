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

void GraphFrame::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxColour black, gray, white;
  black.Set(wxT("#000000"));
  gray.Set(wxT("#c0c0c0"));
  white.Set(wxT("#ffffff"));

  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  if (!vivagraph) return;

  std::vector<VivaNode*>::iterator it;
  for (it = vivagraph->nodes.begin(); it != vivagraph->nodes.end(); it++){
    (*it)->draw(dc);
  }
}

void GraphFrame::leftMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);

  //get the device context, apply the current transformations
  wxPaintDC dc(this);
  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  //transform the mouse position in the screen space to the logical space
  wxPoint mouseLogical;
  mouseLogical.x = dc.DeviceToLogicalX (screen.x);
  mouseLogical.y = dc.DeviceToLogicalY (screen.y);

  vivagraph->leftMouseClicked (mouseLogical);
}

void GraphFrame::rightMouseClicked (wxMouseEvent& event)
{
  wxPoint screen;
  event.GetPosition (&screen.x, &screen.y);

  //get the device context, apply the current transformations
  wxPaintDC dc(this);
  dc.SetUserScale (ratio, ratio);
  dc.SetDeviceOrigin (translate.x, translate.y);

  //transform the mouse position in the screen space to the logical space
  wxPoint mouseLogical;
  mouseLogical.x = dc.DeviceToLogicalX (screen.x);
  mouseLogical.y = dc.DeviceToLogicalY (screen.y);

  vivagraph->rightMouseClicked (mouseLogical);
}

void GraphFrame::OnVivaGraphChanged (wxCommandEvent& event)
{
  Refresh();
}

void GraphFrame::OnVivaGraphLayoutUpdated (wxCommandEvent& event)
{
  Refresh();
}
