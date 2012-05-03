#ifndef __RUNNER_H__
#define __RUNNER_H__
#include <wx/wx.h>
#include "tupi_private.h"

class Triview;

class LayoutRunner : public wxThread
{
public:
  Triview *view;
  tp_layout *layout;
  bool keepRunning;

  LayoutRunner (Triview *t, tp_layout *l);
  virtual ExitCode Entry ();
};

#include "triview.h"
#endif
