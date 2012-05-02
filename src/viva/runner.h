#ifndef __RUNNER_H__
#define __RUNNER_H__
#include <wx/wx.h>
#include "tupi_private.h"

class Triview;

class LayoutRunner : public wxThread
{
public:
  LayoutRunner (Triview *t, tp_layout *l);
  bool keepRunning;
private:
  virtual ExitCode Entry ();
  Triview *tupiew;
  /* tp_layout *layout; */
};

#include "triview.h"
#endif
