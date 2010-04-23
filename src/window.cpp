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

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QMenuBar>
#include <QMessageBox>
#include "window.hpp"
#include "buttongroup.hpp"
#include "board.hpp"
#include "fullscreenexitbutton.hpp"
#include "scheme.hpp"

Window::Window ()
    : QWidget()
{
    setWindowTitle("Color Flood");
    setWindowIcon(QIcon(":/images/icon_48x48.png"));

    int turns;
    board = new Board(this, &turns);
    buttonGroup = new ButtonGroup(this);

    QObject::connect(buttonGroup,
                     SIGNAL(flood(int)),
                     board,
                     SLOT(flood(int)));

    turnsLabel = new QLabel(this);
    turnsLabel->setAlignment(Qt::AlignRight);

    QObject::connect(board,
                     SIGNAL(turnsChanged(int)),
                     this,
                     SLOT(updateTurns(int)));

    updateTurns(turns);

    QPushButton *newGame = new QPushButton(tr("New game"), this);
    QObject::connect(newGame, SIGNAL(pressed()), board, SLOT(randomize()));

    QPushButton *help = new QPushButton(tr("Help"), this);
    QObject::connect(help, SIGNAL(pressed()), this, SLOT(help()));

    QHBoxLayout *lowerLayout = new QHBoxLayout;
    lowerLayout->addWidget(help);
    lowerLayout->addWidget(newGame);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(buttonGroup);
    vl->setAlignment(buttonGroup, Qt::AlignRight | Qt::AlignTop);
    vl->addWidget(turnsLabel);
    vl->setAlignment(turnsLabel, Qt::AlignRight | Qt::AlignBottom);
    vl->addLayout(lowerLayout);
    vl->setAlignment(lowerLayout, Qt::AlignRight | Qt::AlignTop);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(board);
    hl->setAlignment(board, Qt::AlignLeft);
    hl->addLayout(vl);

    setLayout(hl);

    // menu bar
    QMenuBar *bar = new QMenuBar(this);

    QObject::connect(bar->addAction(tr("Fullscreen mode")),
                     SIGNAL(triggered()),
                     this,
                     SLOT(fullScreenMode()));

    QObject::connect(bar->addAction(
                         Scheme::getSchemeName(Scheme::getNextScheme())),
                     SIGNAL(triggered()),
                     this,
                     SLOT(scheme()));

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

    if (!board->getSize())
        less->setEnabled(false);
    else if (board->getSize() == Board::NUM_SIZES - 1)
        more->setEnabled(false);

    new FullScreenExitButton(this);
    showFullScreen();
}

void Window::updateTurns (int turns)
{
    /*: number of turns */
    turnsLabel->setText(tr("Turns: %1/%2")
                        .arg(turns)
                        .arg(board->getNumTurnsOfSize(board->getSize())));
}

void Window::fullScreenMode ()
{
    showFullScreen();
}

void Window::lessCells ()
{
    int s = board->getSize() - 1;

    board->setSize(s);
    more->setEnabled(true);

    if (!s)
        less->setEnabled(false);
}

void Window::moreCells ()
{
    int s = board->getSize() + 1;

    board->setSize(s);
    less->setEnabled(true);

    if (s == Board::NUM_SIZES - 1)
        more->setEnabled(false);
}

void Window::scheme ()
{
    QAction *action = static_cast<typeof(action)>(QObject::sender());

    Scheme::setScheme(Scheme::getNextScheme());

    board->update();
    buttonGroup->update();

    action->setText(Scheme::getSchemeName(Scheme::getNextScheme()));
}

void Window::help ()
{
        QMessageBox box;
        box.setWindowTitle("Color Flood");
        box.setText(tr("The object of the game is to turn a board into one single color. Number of moves is limited. You start from top-left corner with one cell already flooded.\nGood luck!"));
        box.exec();
}
