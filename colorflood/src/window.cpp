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
#include <QMenuBar>
#include <QMessageBox>
#include "window.hpp"
#include "colorbuttons.hpp"
#include "field.hpp"
#include "fullscreenexitbutton.hpp"
#include "colorscheme.hpp"

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

    QPushButton *help = new QPushButton(tr("Help"), this);
    QObject::connect(help, SIGNAL(pressed()), this, SLOT(help()));

    QHBoxLayout *lowerLayout = new QHBoxLayout;
    lowerLayout->addWidget(help);
    lowerLayout->addWidget(newGame);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(colorButtons);
    vl->setAlignment(colorButtons, Qt::AlignRight | Qt::AlignTop);
    vl->addWidget(turnsLabel);
    vl->setAlignment(turnsLabel, Qt::AlignRight | Qt::AlignBottom);
    vl->addLayout(lowerLayout);
    vl->setAlignment(lowerLayout, Qt::AlignRight | Qt::AlignTop);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(field);
    hl->setAlignment(field, Qt::AlignLeft);
    hl->addLayout(vl);

    setLayout(hl);

    /* menu bar */
    QMenuBar *bar = new QMenuBar(this);

    QObject::connect(bar->addAction(tr("Fullscreen mode")),
                     SIGNAL(triggered()),
                     this,
                     SLOT(fullScreenMode()));

    QObject::connect(bar->addAction(
                         ColorScheme::getSchemeName(
                             ColorScheme::getNextColorScheme())),
                     SIGNAL(triggered()),
                     this,
                     SLOT(colorScheme()));

    less = bar->addAction(tr("Less cells"));

    QObject::connect(less,
                     SIGNAL(triggered()),
                     this,
                     SLOT(lessCells()));

    more = bar->addAction(tr("More cells"));

    QObject::connect(more,
                     SIGNAL(triggered()),
                     this,
                     SLOT(moreCells()));

    if (!field->getSize())
        less->setEnabled(false);
    else if (field->getSize() == Field::NUM_SIZES - 1)
        more->setEnabled(false);

    new FullScreenExitButton(this);
    showFullScreen();
}

void Window::updateTurns (int turns)
{
    /*: number of turns */
    turnsLabel->setText(tr("Turns: %1/%2")
                        .arg(turns)
                        .arg(field->getNumTurnsOfSize(field->getSize())));
}

void Window::fullScreenMode ()
{
    showFullScreen();
}

void Window::lessCells ()
{
    int s = field->getSize() - 1;

    field->setSize(s);
    more->setEnabled(true);

    if (!s)
        less->setEnabled(false);
}

void Window::moreCells ()
{
    int s = field->getSize() + 1;

    field->setSize(s);
    less->setEnabled(true);

    if (s == Field::NUM_SIZES - 1)
        more->setEnabled(false);
}

void Window::colorScheme ()
{
    QAction *action = static_cast<typeof(action)>(QObject::sender());

    ColorScheme::setScheme(ColorScheme::getNextColorScheme());

    field->update();
    colorButtons->update();

    action->setText(ColorScheme::getSchemeName(
                        ColorScheme::getNextColorScheme()));
}

void Window::help ()
{
        QMessageBox box;
        box.setWindowTitle("Color Flood");
        box.setText(tr("The object of the game is to turn a board into one single color. Number of moves is limited. You start from top-left corner with one cell already flooded.\nGood luck!"));
        box.exec();
}
