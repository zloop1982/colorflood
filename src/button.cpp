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

#include <QPainter>
#include "button.hpp"
#include "scheme.hpp"

Button::Button (QWidget *parent, int brush)
    : QPushButton(parent),
      brush(brush)
{
    setFixedSize(64, 64);
}

void Button::paintEvent (QPaintEvent * /* event */)
{
    QPainter painter;
    painter.begin(this);

    const QVector<QBrush> &scheme = Scheme::instance().getScheme();
    painter.fillRect(0, 0, 64, 64, scheme.at(brush));

    painter.end();
}
