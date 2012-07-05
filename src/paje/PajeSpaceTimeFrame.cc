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

  zoomSlider = new QSlider;
  zoomSlider->setMinimum(0);
  zoomSlider->setMaximum(500);
  zoomSlider->setValue(250);
  zoomSlider->setTickPosition(QSlider::TicksRight);

  // Zoom slider layout
  QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
//  zoomSliderLayout->addWidget(zoomInIcon);
  zoomSliderLayout->addWidget(zoomSlider);
//  zoomSliderLayout->addWidget(zoomOutIcon);

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
  topLayout->addLayout(zoomSliderLayout, 1, 1);
  setLayout(topLayout);


  connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
  connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
  connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
  connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
  connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
  connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
  connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
  connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
  connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));

  setupMatrix();
}

PajeSpaceTimeView *PajeSpaceTimeFrame::view () const
{
  return graphicsView;
}


void PajeSpaceTimeFrame::resetView()
{
  zoomSlider->setValue(250);
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
  qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

  QMatrix matrix;
  matrix.scale(scale, scale);

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
  zoomSlider->setValue(zoomSlider->value() + level);
}

void PajeSpaceTimeFrame::zoomOut(int level)
{
  zoomSlider->setValue(zoomSlider->value() - level);
}
