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

#pragma once

#include <QVector>
#include <QPair>
#include <QColor>
#include <QString>

/// scheme singleton class
class Scheme
{
public:
    static Scheme &instance ()
        {
            static Scheme instance;
            return instance;
        }

    /// get number of predefined schemes
    static int getNumSchemes ();
    /// get next scheme index (it goes to 0 after last index)
    static int getNextScheme ();
    /// get scheme name
    static QString getSchemeName (int scheme);
    /// get scheme colors
    const QVector<QColor> &getScheme (int scheme);
    /// get current scheme name
    static QString getSchemeName ();
    /// get current scheme colors
    const QVector<QColor> &getScheme ();
    /// set current scheme by its index
    static void setScheme (int scheme);

private:
    Scheme ();
    ~Scheme ();
    Scheme (const Scheme &);
    Scheme & operator= (const Scheme &);
};
