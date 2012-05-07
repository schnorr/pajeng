#ifndef __GRAPH_VIEW_H
#define __GRAPH_VIEW_H
#include <wx/artprov.h>
#include "basic.h"
#include "VivaGraph.h"

class VivaGraph;

DECLARE_EVENT_TYPE (VivaGraphChanged, -1)
DECLARE_EVENT_TYPE (VivaGraphLayoutUpdated, -1)

class GraphView : public BasicFrame
{
private:
  VivaGraph *vivagraph;

  wxMenuBar *menubar;
  wxMenu *file;
  wxMenu *edit;
  wxMenu *view;
  wxMenu *help;

protected:
  void OnTimeIntervalMenu (wxCommandEvent& event);
  void OnGraphConfigurationMenu (wxCommandEvent& event);
  void OnUpdateQuality (wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnAbout (wxCommandEvent& event);
  void OnPaint (wxPaintEvent& event);

  //user interactions
  void leftMouseClicked (wxMouseEvent& event);
  void rightMouseClicked (wxMouseEvent& event);

  //from VivaGraph, and runner
  void OnVivaGraphChanged (wxCommandEvent& event);
  void OnVivaGraphLayoutUpdated (wxCommandEvent& event);

public:
  GraphView (VivaGraph *vivagraph);
  ~GraphView (void);
};

#endif
