#include "timeslicewindow.h"

TimeSliceWindow::TimeSliceWindow (wxWindow *parent, VivaGraph *vivagraph)
  : wxFrame(parent, wxID_ANY, wxT("Time Slice Window"), wxDefaultPosition, wxSize(700,450))
{
  wxPanel *panel = new wxPanel(this, -1);
  TimeSliceFrame *timesliceframe = new TimeSliceFrame (panel, -1);
  timesliceframe->setVivaGraph (vivagraph);

  wxBoxSizer *vbox = new wxBoxSizer (wxVERTICAL);
  vbox->Add (timesliceframe, 0, wxEXPAND);
  panel->SetSizer(vbox);
  Centre();
}

TimeSliceWindow::~TimeSliceWindow ()
{
}
