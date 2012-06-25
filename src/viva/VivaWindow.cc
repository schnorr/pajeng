#include "VivaWindow.h"

VivaWindow::VivaWindow(VivaGraph *vivagraph)
{
  this->vivagraph = vivagraph;

  frame = new VivaGraphView (vivagraph, this);
  setCentralWidget (frame);

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  readSettings();

  //Connect FRAME signals to VIVAGRAPH slots
  connect (frame, SIGNAL(mouseOverPoint(QPointF)),
           vivagraph, SLOT(mouseOverPoint(QPointF)));
  connect (frame, SIGNAL(leftMouseClickedOverPoint(QPointF)),
           vivagraph, SLOT(leftMouseClicked(QPointF)));
  connect (frame, SIGNAL(rightMouseClickedOverPoint(QPointF)),
           vivagraph, SLOT(rightMouseClicked(QPointF)));

  //Connect VIVAGRAPH signals to WINDOW slots
  connect (vivagraph, SIGNAL(highlightNode(VivaNode*)),
           this, SLOT(highlightNode(VivaNode*)));
  connect (vivagraph, SIGNAL(unhighlightNode()),
           this, SLOT(unhighlightNode()));

  //Connect VIVAGRAPH signals to FRAME slots
  connect (vivagraph, SIGNAL(graphChanged(void)),
           frame, SLOT(render(void)));

  setCurrentFile("");
  setUnifiedTitleAndToolBarOnMac(true);

  vivagraph->setView (frame);
}

void VivaWindow::closeEvent(QCloseEvent *event)
{
  // if (maybeSave()) {
  //   writeSettings();
  //   event->accept();
  // } else {
  //   event->ignore();
  // }
}

void VivaWindow::newFile()
{
  // if (maybeSave()) {
  //   textEdit->clear();
  //   setCurrentFile("");
  // }
}

void VivaWindow::open()
{
  // if (maybeSave()) {
  //   QString fileName = QFileDialog::getOpenFileName(this);
  //   if (!fileName.isEmpty())
  //     loadFile(fileName);
  // }
}

bool VivaWindow::save()
{
  // if (curFile.isEmpty()) {
  //   return saveAs();
  // } else {
  //   return saveFile(curFile);
  // }
}

bool VivaWindow::saveAs()
{
  // QString fileName = QFileDialog::getSaveFileName(this);
  // if (fileName.isEmpty())
  //   return false;

  // return saveFile(fileName);
}

void VivaWindow::about()
{
  QMessageBox::about(this, tr("About Application"),
                     tr("The <b>Application</b> example demonstrates how to "
                        "write modern GUI applications using Qt, with a menu bar, "
                        "toolbars, and a status bar."));
}

void VivaWindow::documentWasModified()
{
  // setWindowModified(textEdit->document()->isModified());
}

void VivaWindow::createActions()
{
  playAct = new QAction (QIcon(":/images/media-playback-start.svg"), tr("Play"), this);
  playAct->setStatusTip(tr("Play the particle system algorithm"));
  connect(playAct, SIGNAL(triggered()), vivagraph, SLOT(start_runner()));

  stopAct = new QAction (QIcon(":/images/media-playback-stop.svg"), tr("Stop"), this);
  stopAct->setStatusTip(tr("Stop the particle system algorithm"));
  connect(stopAct, SIGNAL(triggered()), vivagraph, SLOT(stop_runner()));

  shakeAct = new QAction (QIcon(":/images/view-refresh.svg"), tr("Shake"), this);
  shakeAct->setStatusTip(tr("Shake the particle system"));
  connect(shakeAct, SIGNAL(triggered()), vivagraph, SLOT(refresh()));

  topAct = new QAction (QIcon(":/images/go-top.svg"), tr("Top"), this);
  topAct->setStatusTip(tr("Collapse as much as possible the current nodes of the graph"));
  connect(topAct, SIGNAL(triggered()), frame, SLOT(finishRendering()));
  connect(topAct, SIGNAL(triggered()), vivagraph, SLOT(go_top()));
  connect(topAct, SIGNAL(triggered()), frame, SLOT(initRendering()));

  upAct = new QAction (QIcon(":/images/go-up.svg"), tr("Up"), this);
  upAct->setStatusTip(tr("Collapse by one level all current nodes of the graph"));
  connect(upAct, SIGNAL(triggered()), frame, SLOT(finishRendering()));
  connect(upAct, SIGNAL(triggered()), vivagraph, SLOT(go_up()));
  connect(upAct, SIGNAL(triggered()), frame, SLOT(initRendering()));

  downAct = new QAction (QIcon(":/images/go-down.svg"), tr("Down"), this);
  downAct->setStatusTip(tr("Expand by one level all current nodes of the graph"));
  connect(downAct, SIGNAL(triggered()), frame, SLOT(finishRendering()));
  connect(downAct, SIGNAL(triggered()), vivagraph, SLOT(go_down()));
  connect(downAct, SIGNAL(triggered()), frame, SLOT(initRendering()));

  bottomAct = new QAction (QIcon(":/images/go-bottom.svg"), tr("Bottom"), this);
  bottomAct->setStatusTip(tr("Expand as much as possible the current nodes of the graph"));
  connect(bottomAct, SIGNAL(triggered()), frame, SLOT(finishRendering()));
  connect(bottomAct, SIGNAL(triggered()), vivagraph, SLOT(go_bottom()));
  connect(bottomAct, SIGNAL(triggered()), frame, SLOT(initRendering()));

  openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  // saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
  // saveAct->setShortcuts(QKeySequence::Save);
  // saveAct->setStatusTip(tr("Save the document to disk"));
  // connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  // saveAsAct = new QAction(tr("Save &As..."), this);
  // saveAsAct->setShortcuts(QKeySequence::SaveAs);
  // saveAsAct->setStatusTip(tr("Save the document under a new name"));
  // connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  exitAct = new QAction(tr("&Quit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  // cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
  // cutAct->setShortcuts(QKeySequence::Cut);
  // cutAct->setStatusTip(tr("Cut the current selection's contents to the "
  //                         "clipboard"));
  // connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

  // copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
  // copyAct->setShortcuts(QKeySequence::Copy);
  // copyAct->setStatusTip(tr("Copy the current selection's contents to the "
  //                          "clipboard"));
  // connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

  // pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
  // pasteAct->setShortcuts(QKeySequence::Paste);
  // pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
  //                           "selection"));
  // connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  // cutAct->setEnabled(false);
  // copyAct->setEnabled(false);
  // connect(textEdit, SIGNAL(copyAvailable(bool)),
  //         cutAct, SLOT(setEnabled(bool)));
  // connect(textEdit, SIGNAL(copyAvailable(bool)),
  //         copyAct, SLOT(setEnabled(bool)));
}

void VivaWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  // fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  // fileMenu->addAction(saveAct);
  // fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  // editMenu = menuBar()->addMenu(tr("&Edit"));
  // editMenu->addAction(cutAct);
  // editMenu->addAction(copyAct);
  // editMenu->addAction(pasteAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

void VivaWindow::createToolBars()
{
  particleSystemToolBar = addToolBar(tr("Particle System"));
  particleSystemToolBar->addAction (playAct);
  particleSystemToolBar->addAction (stopAct);
  particleSystemToolBar->addAction (shakeAct);

  graphHierarchyToolBar = addToolBar(tr("Graph Hierarchy"));
  graphHierarchyToolBar->addAction (topAct);
  graphHierarchyToolBar->addAction (upAct);
  graphHierarchyToolBar->addAction (downAct);
  graphHierarchyToolBar->addAction (bottomAct);
}

void VivaWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

void VivaWindow::readSettings()
{
  QSettings settings("Trolltech", "Application Example");
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(400, 400)).toSize();
  resize(size);
  move(pos);
}

void VivaWindow::writeSettings()
{
  QSettings settings("Trolltech", "Application Example");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
}

void VivaWindow::loadFile(const QString &fileName)
{
//   QFile file(fileName);
//   if (!file.open(QFile::ReadOnly | QFile::Text)) {
//     QMessageBox::warning(this, tr("Application"),
//                          tr("Cannot read file %1:\n%2.")
//                          .arg(fileName)
//                          .arg(file.errorString()));
//     return;
//   }

//   QTextStream in(&file);
// #ifndef QT_NO_CURSOR
//   QApplication::setOverrideCursor(Qt::WaitCursor);
// #endif
//   textEdit->setPlainText(in.readAll());
// #ifndef QT_NO_CURSOR
//   QApplication::restoreOverrideCursor();
// #endif

//   setCurrentFile(fileName);
//   statusBar()->showMessage(tr("File loaded"), 2000);
}

bool VivaWindow::saveFile(const QString &fileName)
{
//   QFile file(fileName);
//   if (!file.open(QFile::WriteOnly | QFile::Text)) {
//     QMessageBox::warning(this, tr("Application"),
//                          tr("Cannot write file %1:\n%2.")
//                          .arg(fileName)
//                          .arg(file.errorString()));
//     return false;
//   }

//   QTextStream out(&file);
// #ifndef QT_NO_CURSOR
//   QApplication::setOverrideCursor(Qt::WaitCursor);
// #endif
//   out << textEdit->toPlainText();
// #ifndef QT_NO_CURSOR
//   QApplication::restoreOverrideCursor();
// #endif

//   setCurrentFile(fileName);
//   statusBar()->showMessage(tr("File saved"), 2000);
//   return true;
}

void VivaWindow::setCurrentFile(const QString &fileName)
{
  // curFile = fileName;
  // textEdit->document()->setModified(false);
  // setWindowModified(false);

  // QString shownName = curFile;
  // if (curFile.isEmpty())
  //   shownName = "untitled.txt";
  // setWindowFilePath(shownName);
}

QString VivaWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

void VivaWindow::highlightNode (VivaNode *node)
{
  statusBar()->showMessage(QString::fromStdString(node->container->description()), 0);
}

void VivaWindow::unhighlightNode (void)
{
  statusBar()->clearMessage();
}
