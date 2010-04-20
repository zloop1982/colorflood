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

#ifndef _BUTTON_HPP
#define _BUTTON_HPP

#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT;

public:
    Button (QWidget *parent, int brush);

private:
    const int brush;

protected:
    void paintEvent (QPaintEvent *event);
};

#endif // !_BUTTON_HPP
