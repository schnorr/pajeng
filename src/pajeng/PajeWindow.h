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
#ifndef __PAJE_WINDOW_H
#define __PAJE_WINDOW_H
#include <QtGui>
#include <QMainWindow>
#include "PajeSpaceTimeFrame.h"
#include "PajeApplication.h"

QT_FORWARD_DECLARE_CLASS(QSplitter)
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QMenu)

class PajeApplication;

class PajeWindow : public QMainWindow
{
  Q_OBJECT;

private:
  PajeApplication *app;
  static PajeWindow *instance;
public:
  PajeSpaceTimeFrame *spacetimeFrame;
  void setApplication (PajeApplication *a) { app = a; };

  static PajeWindow *getInstance() {
    if (!instance){
      instance = new PajeWindow ();
    }
    return instance;
  };

protected:
  PajeWindow();
  void closeEvent(QCloseEvent *event);

private slots:
  void spacetime();
  void about();


private:
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void readSettings();
  void writeSettings();

  QMenu *fileMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
  QAction *exitAct;
  QAction *spacetimeAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
};
#endif
