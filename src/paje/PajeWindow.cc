#include "PajeWindow.h"

PajeWindow *PajeWindow::instance = NULL;

PajeWindow::PajeWindow()
{
  frame = new PajeSpaceTimeFrame (this);
  setCentralWidget (frame);

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  setUnifiedTitleAndToolBarOnMac(true);
}

void PajeWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}

void PajeWindow::about()
{
  QMessageBox::about(this, tr("About Paje"),
                     tr("<b>Paje</b>, a space/time viewer of Paje trace files."));
}

void PajeWindow::createActions()
{
  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void PajeWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(exitAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

void PajeWindow::createToolBars()
{
}

void PajeWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}
