#ifndef __GRAPH_WINDOW_H
#define __GRAPH_WINDOW_H
#include <wx/artprov.h>
#include "basic.h"
#include "graphframe.h"
#include "timesliceframe.h"
#include "VivaGraph.h"

class VivaGraph;

class GraphWindow : public wxFrame
{
private:
  VivaGraph *vivagraph;

  wxMenuBar *menubar;
  wxMenu *file;
  wxMenu *edit;
  wxMenu *view;
  wxMenu *help;

  wxPanel *main_panel;
  wxBoxSizer *main_vbox;

private:
  std::map<std::string,wxSlider*> scaleSliders;

protected:
  void OnTimeIntervalMenu (wxCommandEvent& event);
  void OnGraphConfigurationMenu (wxCommandEvent& event);
  void OnUpdateQuality (wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnOpen(wxCommandEvent& event);
  void OnAbout (wxCommandEvent& event);
  void OnScaleSliderChanged (wxScrollEvent& event);
  void OnFDThreadManagement (wxCommandEvent& event);
  void OnGoButtonsPressed (wxCommandEvent& event);
  void OnRefreshButtonPressed (wxCommandEvent& event);
  void OnCreateScaleSliders (wxCommandEvent& event);

public:
  GraphWindow (wxWindow *parent, VivaGraph *vivagraph);
  ~GraphWindow (void);
  double scaleSliderValue (std::string name);
};

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(CreateScaleSliders, -1)
END_DECLARE_EVENT_TYPES()

#endif
