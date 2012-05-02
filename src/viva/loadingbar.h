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
