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

#ifndef _COLORSCHEME_HPP
#define _COLORSCHEME_HPP

#include <QVector>
#include <QPair>
#include <QBrush>
#include <QString>

class ColorScheme
{
public:
    static ColorScheme & instance ()
        {
            static ColorScheme instance;
            return instance;
        }

    static int getNumSchemes ();
    static int getNextColorScheme ();
    static QString getSchemeName (int scheme);
    const QVector<QBrush> &getScheme (int scheme);
    static QString getSchemeName ();
    const QVector<QBrush> &getScheme ();
    static void setScheme (int scheme);

private:
    ColorScheme ();
    ~ColorScheme ();
    ColorScheme (const ColorScheme &);
    ColorScheme & operator= (const ColorScheme &);
};

#endif /* !_COLORSCHEME_HPP */
