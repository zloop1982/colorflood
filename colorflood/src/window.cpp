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
#include <QSettings>
#include "window.hpp"
#include "colorbuttons.hpp"
#include "field.hpp"
#include "fullscreenexitbutton.hpp"

Window::Window ()
    : QWidget()
{
    setWindowTitle("Color Flood");
    setWindowIcon(QIcon(":/images/icon_48x48.png"));

    int turns;
    field = new Field(this, &turns);
    colorButtons = new ColorButtons(this);

    QObject::connect(colorButtons,
                     SIGNAL(flood(int)),
                     field,
                     SLOT(flood(int)));

    turnsLabel = new QLabel(this);
    turnsLabel->setAlignment(Qt::AlignRight);

    QObject::connect(field,
                     SIGNAL(turnsChanged(int)),
                     this,
                     SLOT(updateTurns(int)));

    updateTurns(turns);

    QPushButton *newGame = new QPushButton(tr("New game"), this);
    QObject::connect(newGame, SIGNAL(pressed()), field, SLOT(randomize()));

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(colorButtons);
    vl->setAlignment(colorButtons, Qt::AlignRight | Qt::AlignTop);
    vl->addWidget(turnsLabel);
    vl->setAlignment(turnsLabel, Qt::AlignRight | Qt::AlignBottom);
    vl->addWidget(newGame);
    vl->setAlignment(newGame, Qt::AlignRight | Qt::AlignTop);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(field);
    hl->setAlignment(field, Qt::AlignLeft);
    hl->addLayout(vl);

    setLayout(hl);

    QSettings settings;

    if (settings.value("fullscreen", true).toBool())
        showFullScreen();

    new FullScreenExitButton(this);
}

Window::~Window ()
{
    bool isFullscreen = windowState() & Qt::WindowFullScreen;

    QSettings settings;
    settings.setValue("fullscreen", isFullscreen);
}

void Window::updateTurns (int turns)
{
    /*: number of turns */
    turnsLabel->setText(tr("<font size=\"16\">Turns: %1/%2</font>")
                        .arg(turns)
                        .arg(field->getNumTurnsOfSize(field->getSize())));
}
