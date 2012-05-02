#include "loadingbar.h"


LoadingBar::LoadingBar(const wxString& title)
  : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 100))
{
  //the timer
  timer = new wxTimer (this);
  this->Connect (wxEVT_TIMER, wxTimerEventHandler (LoadingBar::OnTimer));


  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  progress = new wxGauge (this, wxID_ANY, 300, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL, wxDefaultValidator, wxEmptyString);
  vbox->Add(progress, 1, wxEXPAND | wxALL, 10);
  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
  cancel = new wxButton (this, wxID_CANCEL, wxT("Cancel"));
  hbox->Add(cancel, 1, wxALIGN_RIGHT | wxEXPAND | wxALL, 10);
  vbox->Add(hbox, 0, wxALIGN_RIGHT | wxBOTTOM, 0);
  this->SetSizer (vbox);
  this->Connect (wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LoadingBar::OnCancel));
  timer->Start (100);
}

LoadingBar::~LoadingBar (void)
{
  delete timer;
}

void LoadingBar::OnTimer (wxTimerEvent &event)
{
  static int val = 0;
  progress->SetValue (val++);
}

void LoadingBar::OnCancel (wxCommandEvent &event)
{
  exit(1);
}
