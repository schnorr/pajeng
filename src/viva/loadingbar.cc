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
#include "loadingbar.h"

LoadingBar::LoadingBar(const wxString& title, PajeFileReader *reader)
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

  //save the reader
  this->reader = reader;
  progress->SetRange(100);
  progress->SetValue(0);
  thread = new PajeThreadedReader (reader);
}

LoadingBar::~LoadingBar (void)
{
  delete timer;
}

void LoadingBar::OnTimer (wxTimerEvent &event)
{
  double porcentage = (double)reader->traceRead()/(double)reader->traceSize(); 
  progress->SetValue(porcentage*100);
  if (!reader->hasMoreData()){
    timer->Stop();
    EndModal (0);
  }
}

void LoadingBar::OnCancel (wxCommandEvent &event)
{
  thread->Delete();
  delete thread;
  EndModal(1);
}

PajeThreadedReader::PajeThreadedReader (PajeFileReader *reader)
  : wxThread (wxTHREAD_JOINABLE)
{
  this->reader = reader;
  if(wxTHREAD_NO_ERROR == Create()) {
    Run();
  }
}

void *PajeThreadedReader::Entry (void)
{
  while(!TestDestroy() && reader->hasMoreData()){
    static int i = 0;
    try {
      reader->startChunk (i);
      reader->readNextChunk();
      reader->endOfChunkLast (!reader->hasMoreData());
      i++;
    }catch(std::string exception){
      std::cout << "Exception: " << exception << std::endl;
      exit(1);
    }
  }
  return static_cast<ExitCode>(NULL);
}
