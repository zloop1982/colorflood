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

#include <QPainter>
#include "button.hpp"
#include "scheme.hpp"

Button::Button (QWidget *parent, int brushIndex)
    : QPushButton(parent),
      brushIndex(brushIndex)
{
    setFixedSize(64, 64);
}

void Button::paintEvent (QPaintEvent * /* event */)
{
    QPainter painter;
    painter.begin(this);

    const QVector<QBrush> &scheme = Scheme::instance().getScheme();
    painter.fillRect(0, 0, 64, 64, scheme.at(brushIndex));

    painter.end();
}
