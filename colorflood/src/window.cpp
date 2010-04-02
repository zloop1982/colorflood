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
    setWindowTitle(tr("Color Flood"));

    //setWindowState(windowState() | Qt::WindowFullScreen);

    QPushButton *button = new QPushButton("randomize", this);
    field = new Field(this);

    QObject::connect(button, SIGNAL(pressed()), this, SLOT(randomize()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(button);
    layout->setAlignment(button, Qt::AlignLeft);
    layout->addWidget(field);
    layout->setAlignment(field, Qt::AlignRight);
    setLayout(layout);
}

void Window::randomize ()
{
    field->randomize();
}
