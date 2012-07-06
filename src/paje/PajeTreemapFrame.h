#ifndef __PAJE_TREEMAP_FRAME_H
#define __PAJE_TREEMAP_FRAME_H
#include <QFrame>
#include "PajeTreemapView.h"

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)

class PajeTreemapView;

class PajeTreemapFrame : public QFrame
{
  Q_OBJECT;
public:
  PajeTreemapFrame (QWidget *parent = 0);
  PajeTreemapView *view () const;

public slots:
//  void zoomIn (int level = 1);
//  void zoomOut (int level = 1);

private slots:
  /* void resetView(); */
  /* void setResetButtonEnabled(); */
  /* void setupMatrix(); */
  /* void togglePointerMode(); */
  /* void toggleOpenGL(); */
  /* void toggleAntialiasing(); */

private:
  PajeTreemapView *graphicsView;
  /* QLabel *label2; */
  /* QToolButton *selectModeButton; */
  /* QToolButton *dragModeButton; */
  /* QToolButton *openGlButton; */
  /* QToolButton *antialiasButton; */
  /* QToolButton *resetButton; */
  /* QSlider *zoomVSlider; */
  /* QSlider *zoomHSlider; */
};

#endif
