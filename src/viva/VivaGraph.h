#ifndef __VIVA_GRAPH_H
#define __VIVA_GRAPH_H
#include <wx/wx.h> //to VivaNode
#include <algorithm>
#include "graphframe.h"
#include "timesliceframe.h"
#include "PajeComponent.h"
#include "tupi_private.h"
#include "VivaNode.h"

class GraphFrame;

class VivaRunner : public wxThread
{
public:
  GraphFrame *view;
  tp_layout *layout;
  bool keepRunning;

public:
  VivaRunner (tp_layout *layout, GraphFrame *view);
  virtual ExitCode Entry (void);
};

class VivaGraph : public PajeComponent 
{
private:
  GraphFrame *view;
  tp_layout *layout;
  VivaRunner *runner;
  std::map<PajeContainer*,VivaNode*> nodeMap;

  void stop_runner (void);
  void start_runner (void);
  VivaNode *getSelectedNodeByPosition (wxPoint point);
  bool hasChildren (PajeContainer *container);
  bool hasParent (PajeContainer *container);
  void expandNode (VivaNode *node);
  void collapseNode (PajeContainer *container);
  void addNode (PajeContainer *container);
  void deleteNode (VivaNode *node);

public:
  std::vector<VivaNode*> nodes;

public:
  VivaGraph ();
  ~VivaGraph ();
  void setView (GraphFrame *view);
  void leftMouseClicked (wxPoint point);
  void rightMouseClicked (wxPoint point);
  void qualityChanged (int quality);

protected:
  void timeLimitsChanged (void);
  void timeSelectionChanged (void);
  void hierarchyChanged (void);
};

#endif
