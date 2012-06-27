#ifndef __ST_ENTITY_TYPE_LAYOUT_H_
#define __ST_ENTITY_TYPE_LAYOUT_H_
#include <map>
#include <vector>
#include <QRectF>
#include "PajeType.h"
#include "PajeContainer.h"

class STTypeLayout
{
private:
  PajeType *entityType;
  std::map<PajeContainer*, QRectF*> rectInContainer;
  double offset_verticalaxis;

public:
  STTypeLayout (PajeType *type);
  PajeType *type (void);
  virtual bool isContainer (void);
  virtual std::vector<STTypeLayout*> subtypes (void);
  static STTypeLayout *Create (PajeType *type);
  double height (void);
  double offset (void);
  void setOffset (double val);
  void setRectInContainer (QRectF *rect, PajeContainer *container);
};

class STEventTypeLayout : public STTypeLayout
{
private:
  double width;

public:
  STEventTypeLayout (PajeType *type, double width);
};

class STStateTypeLayout : public STTypeLayout
{
private:
  double inset;

public:
  STStateTypeLayout (PajeType *type, double inset);
};

class STLinkTypeLayout : public STTypeLayout
{
public:
  STLinkTypeLayout (PajeType *type);
};

class STVariableTypeLayout : public STTypeLayout
{
public:
  STVariableTypeLayout (PajeType *type);
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
  std::map<PajeContainer*,QRectF*> rectsOfContainers;

public:
  STContainerTypeLayout (PajeType *type);
  std::vector<STTypeLayout*> subtypes (void);
  void addSubtype (STTypeLayout *sub);
  bool isContainer (void);
  double subcontainersOffset (void);
  double subtypeSeparation (void);
  double siblingSeparation (void);
  void setRectOfContainer (PajeContainer *container, QRectF *rect);
  QRectF *rectOfContainer (PajeContainer *container);
  void setVerticalOffsets (void);


};

#endif
