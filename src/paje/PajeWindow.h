#ifndef __PAJE_WINDOW_H
#define __PAJE_WINDOW_H
#include <QtGui>
#include <QMainWindow>
#include "PajeSpaceTimeFrame.h"
#include "PajeTreemapFrame.h"
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
  PajeTreemapFrame *treemapFrame;
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
  void treemap();
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
  QAction *treemapAct;
  QAction *spacetimeAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
};
#endif
