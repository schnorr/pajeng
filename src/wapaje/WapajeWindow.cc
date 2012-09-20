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
#include "WapajeWindow.h"

WapajeWindow *WapajeWindow::instance = NULL;

WapajeWindow::WapajeWindow()
{
  spacetimeFrame = new WapajeSpaceTimeFrame ();
  treemapFrame = new WapajeTreemapFrame ();
  treemapFrame->hide();

  QSplitter *split = new QSplitter;
  split->setChildrenCollapsible (false);
  split->setOrientation (Qt::Horizontal);
  split->addWidget (spacetimeFrame);
  split->addWidget (treemapFrame);

  setCentralWidget (split);

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  setUnifiedTitleAndToolBarOnMac(true);
}

void WapajeWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}

void WapajeWindow::treemap()
{
  if (treemapFrame->isHidden()){
    app->connectTreemap ();
    treemapFrame->show();
  }else{
    app->disconnectTreemap ();
    treemapFrame->hide();
  }
}

void WapajeWindow::spacetime()
{
  if (spacetimeFrame->isHidden()){
    app->connectSpacetime ();
    spacetimeFrame->show();
  }else{
    app->disconnectSpacetime ();
    spacetimeFrame->hide();
  }
}

void WapajeWindow::about()
{
  QMessageBox::about(this, tr("About Paje"),
                     tr("<b>Paje</b>, a space/time viewer of Paje trace files."));
}

void WapajeWindow::createActions()
{
  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  treemapAct = new QAction(tr("&Treemap View"), this);
  treemapAct->setStatusTip(tr("Enable or disable the treemap view"));
  connect(treemapAct, SIGNAL(triggered()), this, SLOT(treemap()));

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

void WapajeWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(exitAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction (spacetimeAct);
  viewMenu->addAction (treemapAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

void WapajeWindow::createToolBars()
{
}

void WapajeWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}
