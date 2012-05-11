#ifndef __VIVA_GRAPH_H
#define __VIVA_GRAPH_H
#include <wx/wx.h> //to VivaNode
#include <libconfig.h++>
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
  std::set<std::string> nodeTypes;
  std::set<std::string> edgeTypes;
  libconfig::Setting *configuration;

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
  bool shouldBePresent (PajeContainer *container);

public:
  std::vector<VivaNode*> nodes;

public:
  VivaGraph (std::string conffile);
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
