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
#ifndef LOADING_BAR_H
#define LOADING_BAR_H
#include <wx/wx.h>
#include "PajeFileReader.h"

class PajeThreadedReader : public wxThread
{
private:
  PajeFileReader *reader;

public:
  PajeThreadedReader (PajeFileReader *reader);
  virtual ExitCode Entry (void);
};

class LoadingBar : public wxDialog
{
private:
  wxTimer *timer;
  wxGauge *progress;
  wxButton *cancel;
  wxButton *pause;

  PajeFileReader *reader;
  PajeThreadedReader *thread;

public:
  LoadingBar(const wxString& title, PajeFileReader *reader);
  ~LoadingBar (void);
  void OnTimer (wxTimerEvent &event);
  void OnCancel (wxCommandEvent &event);
};

#endif
