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
