#include "runner.h"

LayoutRunner::LayoutRunner (Tupiew *t, tp_layout *l)
  : wxThread (wxTHREAD_JOINABLE)
{
  tupiew = t;
  layout = l;
  keepRunning = true;
}

void *LayoutRunner::Entry ()
{
  while (!TestDestroy () && keepRunning){
    double limit = layout_stabilization_limit (layout);
    double current = layout_stabilization (layout);
    if (current > limit){
      break;
    }else{
      layout_compute (layout);
    }
    wxCommandEvent event (TupiLayoutUpdated);
    if (keepRunning){
      wxPostEvent (tupiew, event);
    }
  }
  std::cout << "terminating ..... " << std::endl;
  return (wxThread::ExitCode)0;
}
