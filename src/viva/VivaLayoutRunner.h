#ifndef __VIVA_LAYOUT_RUNNER_H
#define __VIVA_LAYOUT_RUNNER_H
#include <iostream>
#include <QThread>
#include <QTimer>
#include <tupi.h>
#include "VivaGraph.h"
#include "VivaGraphView.h"

class VivaGraph;
class VivaGraphView;

class VivaLayoutRunner : public QThread
{
  Q_OBJECT;

public:
  void *layout;
  bool keepRunning;
  VivaGraphView *view;

private:
  void update (void);

public:
  VivaLayoutRunner (VivaGraph *vivagraph, VivaGraphView *view, void *layout);
  ~VivaLayoutRunner (void);
  void run ();
};
#endif
