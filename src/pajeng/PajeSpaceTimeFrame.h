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
#ifndef __PAJE_SPACE_TIME_FRAME_H
#define __PAJE_SPACE_TIME_FRAME_H
#include <QFrame>
#include "PajeSpaceTimeView.h"

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)

class PajeSpaceTimeView;

class PajeSpaceTimeFrame : public QFrame
{
  Q_OBJECT;
public:
  PajeSpaceTimeFrame (QWidget *parent = 0);
  PajeSpaceTimeView *view () const;
  void setSpaceTimeLimit (double space, double time);
  void setCurrentTime (double current_time);

protected:
  void showEvent (QShowEvent *event);
  void resizeEvent (QResizeEvent *event);

public slots:
  void zoomIn (int level = 6);
  void zoomOut (int level = 6);
  void zoomXIn (int level = 6);
  void zoomXOut (int level = 6);

private slots:
  void resetView();
  void setResetButtonEnabled();
  void setupMatrix();
  void togglePointerMode();
  void toggleOpenGL();
  void toggleAntialiasing();

private:
  PajeSpaceTimeView *graphicsView;
  QLabel *label2;
  QToolButton *selectModeButton;
  QToolButton *dragModeButton;
  QToolButton *openGlButton;
  QToolButton *antialiasButton;
  QToolButton *resetButton;
  QSlider *zoomVSlider;
  QSlider *zoomHSlider;
  QLabel *currentTimeLabel;

  double spaceLimit;
  double timeLimit;
};

#endif
