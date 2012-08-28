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
#include "PajeSpaceTimeFrame.h"
#include <QtGui>
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif
#include <qmath.h>

PajeSpaceTimeFrame::PajeSpaceTimeFrame (QWidget *parent)
  : QFrame(parent)
{
//  setFrameStyle(Sunken | StyledPanel);
  graphicsView = new PajeSpaceTimeView (this);
  graphicsView->setRenderHint(QPainter::Antialiasing, false);
  graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
  graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
  QSize iconSize(size, size);

  QToolButton *zoomInIcon = new QToolButton;
  zoomInIcon->setAutoRepeat(true);
  zoomInIcon->setAutoRepeatInterval(33);
  zoomInIcon->setAutoRepeatDelay(0);
  zoomInIcon->setText("ZoomIn");
  QToolButton *zoomOutIcon = new QToolButton;
  zoomOutIcon->setAutoRepeat(true);
  zoomOutIcon->setAutoRepeatInterval(33);
  zoomOutIcon->setAutoRepeatDelay(0);
  zoomOutIcon->setText("ZoomOut");

  resetButton = new QToolButton;
  resetButton->setText(tr("ResetZoom"));
  resetButton->setEnabled(false);

  zoomVSlider = new QSlider;
  zoomVSlider->setMinimum(0);
  zoomVSlider->setMaximum(1000);
  zoomVSlider->setValue(0);
  zoomVSlider->setTickPosition(QSlider::NoTicks);
  zoomVSlider->setInvertedAppearance (true);

  zoomHSlider = new QSlider (Qt::Horizontal);
  zoomHSlider->setMinimum(0);
  zoomHSlider->setMaximum(1000);
  zoomHSlider->setValue  (0);
  zoomHSlider->setTickPosition(QSlider::NoTicks);

  currentTimeLabel = new QLabel ();
  currentTimeLabel->setMinimumWidth(100);

  // VZoom slider layout
  QVBoxLayout *zoomVSliderLayout = new QVBoxLayout;
  zoomVSliderLayout->addWidget(zoomVSlider);
  // HZoom slider layout
  QHBoxLayout *zoomHSliderLayout = new QHBoxLayout;
  zoomHSliderLayout->addWidget(zoomHSlider);
  zoomHSliderLayout->addWidget(currentTimeLabel);

  QHBoxLayout *labelLayout = new QHBoxLayout;
  label2 = new QLabel(tr("Pointer Mode"));
  selectModeButton = new QToolButton;
  selectModeButton->setText(tr("Select"));
  selectModeButton->setCheckable(true);
  selectModeButton->setChecked(true);
  dragModeButton = new QToolButton;
  dragModeButton->setText(tr("Drag"));
  dragModeButton->setCheckable(true);
  dragModeButton->setChecked(false);
  antialiasButton = new QToolButton;
  antialiasButton->setText(tr("Antialiasing"));
  antialiasButton->setCheckable(true);
  antialiasButton->setChecked(false);
  openGlButton = new QToolButton;
  openGlButton->setText(tr("OpenGL"));
  openGlButton->setCheckable(true);
#ifndef QT_NO_OPENGL
  openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
  openGlButton->setEnabled(false);
#endif

  QButtonGroup *pointerModeGroup = new QButtonGroup;
  pointerModeGroup->setExclusive(true);
  pointerModeGroup->addButton(selectModeButton);
  pointerModeGroup->addButton(dragModeButton);

  labelLayout->addWidget(label2);
  labelLayout->addWidget(selectModeButton);
  labelLayout->addWidget(dragModeButton);
  labelLayout->addStretch();
  labelLayout->addWidget (zoomInIcon);
  labelLayout->addWidget (zoomOutIcon);
  labelLayout->addWidget(resetButton);
  labelLayout->addStretch();
  labelLayout->addWidget(antialiasButton);
  labelLayout->addWidget(openGlButton);

  QGridLayout *topLayout = new QGridLayout;
  topLayout->addLayout(labelLayout, 0, 0);
  topLayout->addWidget(graphicsView, 1, 0);
  topLayout->addLayout(zoomVSliderLayout, 1, 1);
  topLayout->addLayout(zoomHSliderLayout, 2, 0);
  setLayout(topLayout);


  connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
  connect(zoomVSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
  connect(zoomHSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
  connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
  connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
  connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
  connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
  connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
  connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
  connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
  connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));

  //activate opengl by default
#ifndef QT_NO_OPENGL
  openGlButton->click();
#endif

  spaceLimit = 0;
  timeLimit = 0;
  resetView();
}

PajeSpaceTimeView *PajeSpaceTimeFrame::view () const
{
  return graphicsView;
}

void PajeSpaceTimeFrame::setSpaceTimeLimit (double space, double time)
{
  this->spaceLimit = space;
  this->timeLimit = time;
  resetView ();
}

void PajeSpaceTimeFrame::setCurrentTime (double current_time)
{
  QString description;
  if (current_time >= 0){
    description = QString::number(current_time);
    currentTimeLabel->setText(description);
  }
}

void PajeSpaceTimeFrame::showEvent ( QShowEvent * event )
{
  setupMatrix();
}

void PajeSpaceTimeFrame::resizeEvent ( QResizeEvent * event )
{
  setupMatrix();
}

void PajeSpaceTimeFrame::resetView()
{
  zoomVSlider->setValue(0);
  zoomHSlider->setValue(0);
  setupMatrix();
  graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

  resetButton->setEnabled(false);
}

void PajeSpaceTimeFrame::setResetButtonEnabled()
{
  resetButton->setEnabled(true);
}

void PajeSpaceTimeFrame::setupMatrix()
{
  QSize s = this->view()->viewport()->size();
  double h = s.height() - .1*s.height();
  double w = s.width() - .1*s.width();

  double scaley = h/spaceLimit * qPow (2, (double)(zoomVSlider->value())/50);
  double scalex = w/timeLimit * qPow (2, (double)(zoomHSlider->value())/50);

  QMatrix matrix;
  matrix.scale(scalex, scaley);

  graphicsView->setMatrix(matrix);
  setResetButtonEnabled();
}

void PajeSpaceTimeFrame::togglePointerMode()
{
  graphicsView->setDragMode(selectModeButton->isChecked()
                            ? QGraphicsView::RubberBandDrag
                            : QGraphicsView::ScrollHandDrag);
  graphicsView->setInteractive(selectModeButton->isChecked());
}

void PajeSpaceTimeFrame::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
  graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void PajeSpaceTimeFrame::toggleAntialiasing()
{
  graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

void PajeSpaceTimeFrame::zoomIn(int level)
{
  zoomVSlider->setValue(zoomVSlider->value() + level);
  zoomHSlider->setValue(zoomHSlider->value() + level);
}


void PajeSpaceTimeFrame::zoomOut(int level)
{
  zoomVSlider->setValue(zoomVSlider->value() - level);
  zoomHSlider->setValue(zoomHSlider->value() - level);
}

void PajeSpaceTimeFrame::zoomXIn (int level)
{
  int new_hlevel = zoomHSlider->value() + level;
  if (new_hlevel > zoomHSlider->maximum()){
    zoomHSlider->setMaximum (new_hlevel);
  }
  zoomHSlider->setValue(new_hlevel);
}

void PajeSpaceTimeFrame::zoomXOut (int level)
{
  int new_hlevel = zoomHSlider->value() - level;
  if (new_hlevel > zoomHSlider->maximum()){
    zoomHSlider->setMaximum (new_hlevel);
  }
  zoomHSlider->setValue(new_hlevel);
}
