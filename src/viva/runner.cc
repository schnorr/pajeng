#include "runner.h"

LayoutRunner::LayoutRunner (Triview *t, tp_layout *l)
  : wxThread (wxTHREAD_JOINABLE)
{
  view = t;
  layout = l;
  keepRunning = true;
  if(wxTHREAD_NO_ERROR == Create()) {
    Run();
  }
}

void *LayoutRunner::Entry ()
{
  std::cout << __FUNCTION__ << std::endl;
  while (!TestDestroy () && keepRunning){
    double limit = layout_stabilization_limit (layout);
    double current = layout_stabilization (layout);
    std::cout << current << "/" << limit << std::endl;
    if (current > limit){
      break;
    }else{
      layout_compute (layout);
    }
    wxCommandEvent event (TupiLayoutUpdated);
    if (keepRunning){
      wxPostEvent (view, event);
    }
  }
  std::cout << "terminating ..... " << std::endl;
  return (wxThread::ExitCode)0;
}
