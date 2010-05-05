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

#include <QSettings>
#include "scheme.hpp"

static int currentScheme = 0;
static QVector<QPair<QString, QVector<QColor> > > schemes;

Scheme::Scheme ()
{
    schemes.clear();

    QVector<QColor> s;

    s << QColor(0x60, 0x60, 0xa8);
    s << QColor(0xf6, 0xf6, 0x1d);
    s << QColor(0x46, 0xb0, 0xe0);
    s << QColor(0x7e, 0xa0, 0x20);
    s << QColor(0xf0, 0x70, 0xa0);
    s << QColor(0xdc, 0x4a, 0x20);
    /*: default scheme name */
    schemes << QPair<QString, QVector<QColor> >(QObject::tr("Default scheme"), s);

    s.clear();
    s << QColor(0x00, 0x00, 0x00);
    s << QColor(0x20, 0x20, 0x20);
    s << QColor(0x66, 0x66, 0x66);
    s << QColor(0x99, 0x99, 0x99);
    s << QColor(0xcc, 0xcc, 0xcc);
    s << QColor(0xff, 0xff, 0xff);
    /*: black-and-white scheme name */
    schemes << QPair<QString, QVector<QColor> >(QObject::tr("Black-and-white scheme"), s);

    QSettings settings;
    currentScheme = settings.value("scheme", 0).toInt();

    if (currentScheme < 0 || currentScheme > s.size())
        currentScheme = 0;
}

Scheme::~Scheme ()
{
    QSettings settings;
    settings.setValue("scheme", currentScheme);
}

int Scheme::getNumSchemes ()
{
    return schemes.size();
}

int Scheme::getNextScheme ()
{
    return (currentScheme + 1) % schemes.size();
}

QString Scheme::getSchemeName (int scheme)
{
    Q_ASSERT(scheme >= 0 && scheme < getNumSchemes());
    return schemes.at(scheme).first;
}

const QVector<QColor> &Scheme::getScheme (int scheme)
{
    Q_ASSERT(scheme > 0 && scheme < getNumSchemes());
    return schemes.at(scheme).second;
}

QString Scheme::getSchemeName ()
{
    return schemes.at(currentScheme).first;
}

const QVector<QColor> &Scheme::getScheme ()
{
    return schemes.at(currentScheme).second;
}

void Scheme::setScheme (int scheme)
{
    Q_ASSERT(scheme >= 0 && scheme < getNumSchemes());
    currentScheme = scheme;
}
