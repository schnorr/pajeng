/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __VIVA_GRAPH_H
#define __VIVA_GRAPH_H
#include <QObject>
#include <QPoint>
#include <libconfig.h>
#include <algorithm>
#include "VivaLayoutRunner.h"
#include "PajeComponent.h"
#include "tupi.h"
#include "VivaNode.h"
#include "VivaGraphView.h"

class VivaLayoutRunner;
class VivaGraphView;

class VivaGraph : public QObject, public PajeComponent 
{
  Q_OBJECT;

private:
  std::set<std::string> nodeTypes;
  std::set<std::string> edgeTypes;
  config_t config;

public: //for scale management
  std::map<std::string,double> compositionsScale;

private:
  void *layout;
  VivaLayoutRunner *runner;
  VivaGraphView *view;
  std::map<PajeContainer*,VivaNode*> nodeMap;
  std::map<PajeContainer*,std::set<PajeContainer*> > edges;
  std::map<PajeContainer*,tp_point> positions;

  VivaNode *getSelectedNodeByPosition (tp_point point);
  bool hasChildren (PajeContainer *container);
  bool hasParent (PajeContainer *container);
  void expandNode (VivaNode *node);
  void collapseNode (PajeContainer *container);
  void addNode (PajeContainer *container);
  void addNode (PajeContainer *container, tp_point np);
  void deleteNode (VivaNode *node);
  bool shouldBePresent (PajeContainer *container);
  void interconnectNodes (void);

public:
  std::vector<VivaNode*> nodes;
  bool layoutDone;

private:
  void defineEdges (void);
  void addEdge (PajeContainer *a1, PajeContainer *a2);
  void defineEdges (PajeContainer *root); //recursive call
  void layoutNodes (void);

public:
  VivaGraph (std::string conffile);
  ~VivaGraph ();
  void setView (VivaGraphView *view);
  void draw (void);
  void qualityChanged (int quality);
  void scaleSliderChanged ();

public slots:
  void stop_runner (void);
  void start_runner (void);
  void go_bottom (void);
  void go_down (void);
  void go_up (void);
  void go_top (void);
  void refresh (void);

public:
  //for configurations
  void defineMaxForConfigurations (void);
  double maxForConfigurationWithName (std::string configurationName);
  double userScaleForConfigurationWithName (std::string configurationName);

protected:
  void timeLimitsChanged (void);
  void timeSelectionChanged (void);
  void hierarchyChanged (void);

signals:
  void graphChanged (void);
  void highlightNode (VivaNode *node);
  void unhighlightNode (void);

public slots:
  void mouseOverPoint (QPointF point);
  void leftMouseClicked (QPointF point);
  void rightMouseClicked (QPointF point);
  void layoutUpdated (void);
};

#endif
