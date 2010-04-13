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

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "window.hpp"
#include "colorbuttons.hpp"
#include "field.hpp"
#include "fullscreenexitbutton.hpp"

Window::Window ()
    : QWidget()
{
    setWindowTitle("Color Flood");

    setWindowState(windowState() | Qt::WindowFullScreen);

    new FullScreenExitButton(this);

    int turns;
    field = new Field(this, &turns);
    colorButtons = new ColorButtons(this);

    QObject::connect(colorButtons,
                     SIGNAL(flood(int)),
                     field,
                     SLOT(flood(int)));

    turnsLabel = new QLabel(this);

    QObject::connect(field,
                     SIGNAL(turnsChanged(int)),
                     this,
                     SLOT(updateTurns(int)));

    updateTurns(turns);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(turnsLabel);
    vl->setAlignment(turnsLabel, Qt::AlignTop);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(colorButtons);
    hl->setAlignment(colorButtons, Qt::AlignLeft);
    hl->addWidget(field);
    hl->setAlignment(field, Qt::AlignRight);

    vl->addLayout(hl);
    setLayout(vl);
}

void Window::updateTurns (int turns)
{
    /*: number of turns */
    turnsLabel->setText(tr("Turns: %1/%2")
                        .arg(turns)
                        .arg(field->getNumTurnsOfSize(field->getSize())));
}
