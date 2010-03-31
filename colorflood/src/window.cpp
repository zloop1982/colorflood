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

#include <QtGui>
#include "window.hpp"
#include "field.hpp"

Window::Window ()
    : QWidget()
{
    setWindowTitle(tr("Color flood"));

    setWindowState(windowState() | Qt::WindowFullScreen);

    QVector<QBrush> brushes;

#if 1
    // standart color scheme
    brushes << QBrush(QColor(0x00, 0x00, 0xff)); // blue
    brushes << QBrush(QColor(0xff, 0x00, 0x00)); // red
    brushes << QBrush(QColor(0x00, 0xff, 0x00)); // green
    brushes << QBrush(QColor(0xff, 0xff, 0x00)); // yellow
    brushes << QBrush(QColor(0xff, 0x00, 0xff)); // magenta
    brushes << QBrush(QColor(0x80, 0x00, 0x80)); // purple
#else
    // color-blind color scheme
    brushes << QBrush(QColor(0x00, 0x00, 0x00));
    brushes << QBrush(QColor(0x31, 0x31, 0x31), Qt::Dense1Pattern);
    brushes << QBrush(QColor(0x62, 0x62, 0x62), Qt::Dense3Pattern);
    brushes << QBrush(QColor(0x93, 0x93, 0x93), Qt::CrossPattern);
    brushes << QBrush(QColor(0xc4, 0xc4, 0xc4));
    brushes << QBrush(QColor(0xff, 0xff, 0xff));
#endif

    field = new Field(this, brushes, Field::SIZE_LARGE);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(field);
    layout->setAlignment(field, Qt::AlignRight);
    setLayout(layout);
}
