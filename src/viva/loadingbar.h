#ifndef LOADING_BAR_H
#define LOADING_BAR_H
#include <wx/wx.h>

class LoadingBar : public wxDialog
{
private:
  wxTimer *timer;
  wxGauge *progress;
  wxButton *cancel;
  wxButton *pause;

public:
  LoadingBar(const wxString& title);
  ~LoadingBar (void);
  void OnTimer (wxTimerEvent &event);
  void OnCancel (wxCommandEvent &event);
};

#endif
