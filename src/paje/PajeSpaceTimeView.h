#ifndef __PAJE_SPACE_TIME_VIEW_H_
#define __PAJE_SPACE_TIME_VIEW_H_
#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "PajeComponent.h"
#include "STEntityTypeLayout.h"

class PajeSpaceTimeView : public QGraphicsView, public PajeComponent
{
  Q_OBJECT;

private:
  QGraphicsScene scene;

public:
  PajeSpaceTimeView (QWidget *parent = NULL);

private:
  /* Handling with layout descriptors */
  std::map<PajeType*,STTypeLayout*> layoutDescriptors;
  STTypeLayout *layoutDescriptorForType (PajeType *type);
  STTypeLayout *createTypeLayout (PajeType *type, STContainerTypeLayout *containerLayout);
  QRectF *calcRectOfContainer (PajeContainer *container, STContainerTypeLayout *layout, double minY);
  QRectF *calcRectOfAllInstances (PajeContainer *container, STContainerTypeLayout *layout, double minY);
  void renewLayoutDescriptors (void);

protected: //from PajeComponent protocol
  void hierarchyChanged (void);

protected: //from QGraphicsView
};

#endif
