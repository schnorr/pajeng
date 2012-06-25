#include <QProgressDialog>
#include <QMetaType>
#include <QThread>
#include "PajeFileReader.h"

class PajeThreadReader : public QThread
{
  Q_OBJECT;

private:
  QProgressDialog *dialog;
  PajeFileReader *reader;
  bool killed;

signals:
  void percentageCompleted (int);
  void closeDialogIfStillOpen (void);
  void readingException (std::string exception);

public slots:
  void throwException (std::string exception);

public:
  PajeThreadReader (PajeFileReader *reader);
  ~PajeThreadReader (void);
  void read ();

private:
  void run ();
};

Q_DECLARE_METATYPE (std::string);
