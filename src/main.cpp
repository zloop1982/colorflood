/*
  Copyright (C) 2010 Serge Ziryukin <ftrvxmtrx@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "window.hpp"

/*
=================
main
=================
*/
int main (int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("ftrvxmtrx");
    QCoreApplication::setApplicationName("Color Flood");

    QTranslator translator;

    if (QLocale::Russian == QLocale::system().language())
        translator.load(":/colorflood.qm");

    app.installTranslator(&translator);

    Window window;
    window.show();

    return app.exec();
}
