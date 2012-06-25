#include <QtGui>
#include <QMainWindow>
#include "VivaGraph.h"
#include "VivaGraphView.h"

class QAction;
class QMenu;
class QPlainTextEdit;

class VivaWindow : public QMainWindow
{
  Q_OBJECT;

public:
  VivaWindow(VivaGraph *vivagraph);

private:
  VivaGraph *vivagraph;
  VivaGraphView *frame;

protected:
  void closeEvent(QCloseEvent *event);

  private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void about();
  void documentWasModified();

private:
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void readSettings();
  void writeSettings();
  /* bool maybeSave(); */
  void loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);
  QString strippedName(const QString &fullFileName);

  QString curFile;

  QMenu *fileMenu;
  /* QMenu *editMenu; */
  /* QMenu *viewMenu; */
  QMenu *helpMenu;

  QToolBar *particleSystemToolBar;
  QToolBar *graphHierarchyToolBar;

  QAction *playAct;
  QAction *stopAct;
  QAction *shakeAct;

  QAction *bottomAct;
  QAction *downAct;
  QAction *upAct;
  QAction *topAct;


  QAction *openAct;
  QAction *exitAct;
  QAction *aboutAct;
  QAction *aboutQtAct;

public slots:
  void highlightNode (VivaNode *node);
  void unhighlightNode (void);
};
