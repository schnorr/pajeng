/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
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
  void setVivaGraph (VivaGraph *vivagraph);

private:
  VivaGraph *vivagraph;
  wxRealPoint convertPoint (wxPoint point);

  void OnPaint (wxPaintEvent& event);
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
