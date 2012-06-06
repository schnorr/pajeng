#ifndef __GRAPH_FRAME_H
#define __GRAPH_FRAME_H
#include <wx/wx.h>
#include <gtk/gtk.h>
#include <wx/glcanvas.h>
#include <GL/glu.h>
#include <cairo.h>
#include "basic.h"

class VivaGraph;

class GraphFrame : public wxGLCanvas
{
protected:
  double ratio;
  wxRealPoint translate;
  wxPoint mousePosition;
  double aspect_ratio;

public:
  GraphFrame (wxWindow *parent,
                wxWindowID id,
                const wxPoint & pos = wxDefaultPosition,
                const wxSize & size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString & name = _T("GraphFrame"));


private:
  VivaGraph *vivagraph;
  wxRealPoint convertPoint (wxPoint point);

  void OnPaint (wxPaintEvent& event);
  void setVivaGraph (VivaGraph *vivagraph);
  void OnMouseMotion (wxMouseEvent& event);
  void OnMouseWheel (wxMouseEvent& event);
  void OnSize (wxSizeEvent& event);

  //user interactions
  void leftMouseClicked (wxMouseEvent& event);
  void rightMouseClicked (wxMouseEvent& event);

  //from VivaGraph, and runner
  void OnVivaGraphChanged (wxCommandEvent& event);
  void OnVivaGraphLayoutUpdated (wxCommandEvent& event);
};

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(VivaGraphChanged, -1)
DECLARE_EVENT_TYPE(VivaGraphLayoutUpdated, -1)
END_DECLARE_EVENT_TYPES()

#include "VivaGraph.h"
#endif
