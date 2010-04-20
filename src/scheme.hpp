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

#ifndef _SCHEME_HPP
#define _SCHEME_HPP

#include <QVector>
#include <QPair>
#include <QBrush>
#include <QString>

class Scheme
{
public:
    static Scheme & instance ()
        {
            static Scheme instance;
            return instance;
        }

    static int getNumSchemes ();
    static int getNextScheme ();
    static QString getSchemeName (int scheme);
    const QVector<QBrush> &getScheme (int scheme);
    static QString getSchemeName ();
    const QVector<QBrush> &getScheme ();
    static void setScheme (int scheme);

private:
    Scheme ();
    ~Scheme ();
    Scheme (const Scheme &);
    Scheme & operator= (const Scheme &);
};

#endif // !_SCHEME_HPP
