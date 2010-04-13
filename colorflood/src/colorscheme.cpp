/*
  Copyright 2010 Serge Ziryukin <ftrvxmtrx@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
*/

#include <QSettings>
#include "colorscheme.hpp"

static int currentScheme = 0;
static QVector<QPair<QString, QVector<QBrush> > > schemes;

ColorScheme::ColorScheme ()
{
    schemes.clear();

    QVector<QBrush> s;

    s << QColor(0x00, 0x00, 0xff); // blue
    s << QColor(0xff, 0x00, 0x00); // red
    s << QColor(0x00, 0xff, 0x00); // green
    s << QColor(0xff, 0xff, 0x00); // yellow
    s << QColor(0xff, 0x00, 0xff); // magenta
    s << QColor(0x80, 0x00, 0x80); // purple
    /*: default color scheme name */
    schemes << QPair<QString, QVector<QBrush> >(QObject::tr("Default"), s);

    s.clear();
    s << QBrush(QColor(0x00, 0x00, 0x00), Qt::SolidPattern);
    s << QBrush(QColor(0x33, 0x33, 0x33), Qt::Dense3Pattern);
    s << QBrush(QColor(0x66, 0x66, 0x66), Qt::Dense1Pattern);
    s << QBrush(QColor(0x99, 0x99, 0x99), Qt::SolidPattern);
    s << QBrush(QColor(0xcc, 0xcc, 0xcc), Qt::CrossPattern);
    s << QBrush(QColor(0xff, 0xff, 0xff), Qt::SolidPattern);
    /*: black-and-white color scheme name */
    schemes << QPair<QString, QVector<QBrush> >(QObject::tr("Black-and-white"), s);

    QSettings settings;
    currentScheme = settings.value("colorScheme", 0).toInt();

    if (currentScheme < 0 || currentScheme > s.size())
        currentScheme = 0;
}

ColorScheme::~ColorScheme ()
{
    QSettings settings;
    settings.setValue("colorScheme", currentScheme);
}

int ColorScheme::getNumSchemes ()
{
    return schemes.size();
}

QString ColorScheme::getSchemeName (int scheme)
{
    Q_ASSERT(scheme > 0 && scheme < getNumSchemes());
    return schemes.at(scheme).first;
}

const QVector<QBrush> &ColorScheme::getScheme (int scheme)
{
    Q_ASSERT(scheme > 0 && scheme < getNumSchemes());
    return schemes.at(scheme).second;
}

QString ColorScheme::getSchemeName ()
{
    return schemes.at(currentScheme).first;
}

const QVector<QBrush> &ColorScheme::getScheme ()
{
    return schemes.at(currentScheme).second;
}

void ColorScheme::setScheme (int scheme)
{
    Q_ASSERT(scheme >= 0 && scheme < getNumSchemes());
    currentScheme = scheme;
}
