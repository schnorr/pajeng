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
#include "PajeWindow.h"

PajeWindow *PajeWindow::instance = NULL;

PajeWindow::PajeWindow()
{
  spacetimeFrame = new PajeSpaceTimeFrame ();

  QSplitter *split = new QSplitter;
  split->setChildrenCollapsible (false);
  split->setOrientation (Qt::Horizontal);
  split->addWidget (spacetimeFrame);

  setCentralWidget (split);

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

void PajeWindow::spacetime()
{
  if (spacetimeFrame->isHidden()){
    app->connectSpacetime ();
    spacetimeFrame->show();
  }else{
    app->disconnectSpacetime ();
    spacetimeFrame->hide();
  }
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

  spacetimeAct = new QAction(tr("&Space/Time View"), this);
  spacetimeAct->setStatusTip(tr("Enable or disable the Space/Time view"));
  connect(spacetimeAct, SIGNAL(triggered()), this, SLOT(spacetime()));

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

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction (spacetimeAct);

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
