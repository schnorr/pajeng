/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
#include <Qt>
#include "VivaApplication.h"

int main(int argc, char **argv)
{
  QApplication::setAttribute( Qt::AA_X11InitThreads );
  VivaApplication app(argc, argv);
  try {
    app.init();
  }catch (std::string exception){
    std::cout << "Exception: " << exception << std::endl;
    return 1;
  }
  return app.exec();
}
