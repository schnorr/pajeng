#ifndef __RUNNER_H__
#define __RUNNER_H__
#include <wx/wx.h>
#include "tupi_private.h"

class Tupiew;

class LayoutRunner : public wxThread
{
public:
  LayoutRunner (Tupiew *t, tp_layout *l);
  bool keepRunning;
private:
  virtual ExitCode Entry ();
  Tupiew *tupiew;
  tp_layout *layout;
};

#include "tupiew.h"
#endif
