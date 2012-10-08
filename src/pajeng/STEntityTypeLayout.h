/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __ST_ENTITY_TYPE_LAYOUT_H_
#define __ST_ENTITY_TYPE_LAYOUT_H_
#include <map>
#include <vector>
#include <QRectF>
#include "PajeType.h"
#include "PajeContainer.h"
#include "PajeComponent.h"

class STContainerTypeLayout;

class STTypeLayout
{
private:
  PajeType *entityType;
  STContainerTypeLayout *parentContainerLayout;
  std::map<PajeContainer*,QPointF> layoutPos; //in parent's coordinates
  std::map<PajeContainer*,double> layoutHeight;

public:
  static STTypeLayout *Create (PajeType *type, STContainerTypeLayout *parent); //factory
  STTypeLayout (PajeType *type, STContainerTypeLayout *parent);
  STContainerTypeLayout *parentLayout (void);
  PajeType *type (void);
  virtual bool isContainer (void);
  virtual std::vector<STTypeLayout*> subtypes (void);
  double height (void);
  virtual double inset (void);
  QPointF layoutPositionForContainer (PajeContainer *container); //in parent's coordinates
  void setLayoutPositionForContainer (PajeContainer *container, QPointF val); //in parent's coordinates
  double layoutHeightForContainer (PajeContainer *container);
  void setLayoutHeightForContainer (PajeContainer *container, double val);
};

class STEventTypeLayout : public STTypeLayout
{
private:
  double width;

public:
  STEventTypeLayout (PajeType *type, STContainerTypeLayout *parent, double width);
};

class STStateTypeLayout : public STTypeLayout
{
private:
  double _inset;

public:
  STStateTypeLayout (PajeType *type, STContainerTypeLayout *parent, double inset);
  double inset();
};

class STLinkTypeLayout : public STTypeLayout
{
public:
  STLinkTypeLayout (PajeType *type, STContainerTypeLayout *parent);
};

class STVariableTypeLayout : public STTypeLayout
{
public:
  STVariableTypeLayout (PajeType *type, STContainerTypeLayout *parent);
};

class STContainerTypeLayout : public STTypeLayout
{
private:
  double _supEventsOffset;  // base of superior events
  double _infEventsOffset;  // base of inferior events
  double _subcontainersOffset;  // start of subcontainers
  double _variablesOffset;
  double _heightForVariables;

  double _siblingSeparation; // separation from other containers of same type in same container
  double _subtypeSeparation; // separation between two subtypes

  std::vector<STStateTypeLayout*> stateSubtypes;
  std::vector<STContainerTypeLayout*> containerSubtypes;

public:
  STContainerTypeLayout (PajeType *type, STContainerTypeLayout *parent);
  QPointF recursiveSetLayoutPositions (PajeContainer *container, PajeComponent *filter, QPointF current); //in parent's coordinates

  std::vector<STTypeLayout*> subtypes (void);
  void addSubtype (STTypeLayout *sub);
  bool isContainer (void);
  double subcontainersOffset (void);
  double subtypeSeparation (void);
  double siblingSeparation (void);
};

#endif
