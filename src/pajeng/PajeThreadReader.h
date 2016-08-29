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

public:
  PajeThreadReader (PajeFileReader *reader);
  ~PajeThreadReader (void);
  void read ();

private:
  void run ();
};

Q_DECLARE_METATYPE (std::string);
