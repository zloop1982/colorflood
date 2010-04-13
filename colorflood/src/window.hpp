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

#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <QWidget>

class ColorButtons;
class Field;
class QLabel;

class Window : public QWidget
{
    Q_OBJECT;

public:
    Window ();

private slots:
    void updateTurns (int turns);
    void toggleFullscreen ();

private:
    ColorButtons *colorButtons;
    Field *field;
    QLabel *turnsLabel;
};

#endif // !_WINDOW_HPP
