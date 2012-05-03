#ifndef __GRAPH_VIEW_H
#define __GRAPH_VIEW_H
#include <wx/artprov.h>
#include "basic.h"
#include "VivaGraph.h"

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
  void OnUpdateQuality (wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnAbout (wxCommandEvent& event);
  void OnPaint (wxPaintEvent& event);

public:
  GraphView (VivaGraph *vivagraph);
  ~GraphView (void);
};

#endif
