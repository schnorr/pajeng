#include "VivaLayoutRunner.h"
#include <QDateTime>

VivaLayoutRunner::VivaLayoutRunner (VivaGraph *vivagraph, VivaGraphView *view, void *layout)
{
  keepRunning = true;
  this->layout = layout;
  this->view = view;
  start();
}

VivaLayoutRunner::~VivaLayoutRunner (void)
{
}

void VivaLayoutRunner::run ()
{
  while(!isFinished() && keepRunning){
    double limit = layout_stabilization_limit (layout);
    double current = layout_stabilization (layout);
    if (current > limit){
      break;
    }else{
      layout_compute (layout);
      update ();
    }
  }
}

void VivaLayoutRunner::update ()
{
  if(view)
    view->render();
}
