#ifndef __RUNNER_H__
#define __RUNNER_H__
#include <wx/wx.h>
#include "tupi.h"

class Tupiew;

class LayoutRunner : public wxThread
{
public:
  LayoutRunner (Tupiew *t, void *l);
  bool keepRunning;
private:
  virtual ExitCode Entry ();
  Tupiew *tupiew;
  void *layout;
};

#include "tupiew.h"
#endif
