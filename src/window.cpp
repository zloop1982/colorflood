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
#include <QSizePolicy>
#include <QApplication>
#include <QDesktopWidget>
#include <QTextBrowser>
#include <QMaemo5InformationBox>
#include "window.hpp"
#include "buttongroup.hpp"
#include "board.hpp"
#include "fullscreentogglebutton.hpp"
#include "scheme.hpp"

Window::Window ()
    : QWidget(),
      gamesWon(0),
      gamesPlayed(0)
{
    setWindowTitle("Color Flood");
    setWindowIcon(QIcon(":/images/icon_48x48.png"));
    setAttribute(Qt::WA_Maemo5AutoOrientation,true);
    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(orientationChanged()));

    int turns;
    board = new Board(this, &turns);
    buttonGroup = new ButtonGroup(this);

    QObject::connect(buttonGroup,
                     SIGNAL(flood(int)),
                     board,
                     SLOT(flood(int)));

    // turns
    turnsLabel = new QLabel(this);

    QObject::connect(board,
                     SIGNAL(gameStateChanged(int, bool, bool)),
                     this,
                     SLOT(updateGameState(int, bool, bool)));

    updateGameState(turns, false, false);

    // best result (minimal number of turns used to win)
    minTurnsWinLabel = new QLabel(this);
    // games won/played
    gamesWonPlayedLabel = new QLabel(this);

    updateBestResult();
    updateGamesWonPlayed();

    // 'fullscreen toggle'/'new game' buttons
    QPushButton *newGame = new QPushButton(tr("New game"), this);
    QObject::connect(newGame, SIGNAL(pressed()), board, SLOT(randomize()));

    FullScreenToggleButton *fsButton = new FullScreenToggleButton(this);

    // layouts
    lowerLayout = new QHBoxLayout;
    lowerLayout->addWidget(newGame);
    lowerLayout->addWidget(fsButton);

    statsLayout = new QVBoxLayout;
    statsLayout->addWidget(turnsLabel);
    statsLayout->addWidget(minTurnsWinLabel);
    statsLayout->addWidget(gamesWonPlayedLabel);

    vl = new QVBoxLayout;
    vl->addWidget(buttonGroup);
    vl->addLayout(statsLayout);
    vl->addLayout(lowerLayout);

    hl = new QHBoxLayout;
    hl->addWidget(board);
    hl->setAlignment(board, Qt::AlignLeft);
    hl->addLayout(vl);

    setLayout(hl);

    // rearrange layouting based on hand mode
    QSettings settings;
    currentHand = settings.value("hand", HAND_RIGHT).toInt();
    handMode(false);

    // menu bar
    QMenuBar *bar = new QMenuBar(this);

    QObject::connect(bar->addAction(tr("Help")),
                     SIGNAL(triggered()),
                     this,
                     SLOT(help()));

    QObject::connect(bar->addAction(
                         Scheme::getSchemeName(Scheme::getNextScheme())),
                     SIGNAL(triggered()),
                     this,
                     SLOT(scheme()));

    hand = bar->addAction(handToNextString(currentHand));

    QObject::connect(hand,
                     SIGNAL(triggered()),
                     this,
                     SLOT(handMode()));

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

    // start it fullscreen
    showFullScreen();
}

void Window::updateGameState (int turns, bool gameFinished, bool won)
{
    /*: number of turns */
    turnsLabel->setText(tr("Turns: %1/%2")
                        .arg(turns)
                        .arg(board->getNumTurnsOfSize(board->getSize())));

    if (gameFinished)
    {
        updateBestResult(turns);
        updateGamesWonPlayed(true, won);
    }
}

void Window::lessCells ()
{
    int s = board->getSize() - 1;

    board->setSize(s);
    more->setEnabled(true);
    updateBestResult();

    if (!s)
        less->setEnabled(false);
}

void Window::moreCells ()
{
    int s = board->getSize() + 1;

    board->setSize(s);
    less->setEnabled(true);
    updateBestResult();

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
    QString msg = tr("The object of the game is to "
                     "turn a board into one single color. "
                     "Number of moves is limited. "
                     "You start from top-left corner with one cell "
                     "already flooded.\nGood luck!");

    QMaemo5InformationBox box(this);
    QTextBrowser textBrowser;
    // cursorPositionChanged seems to be ugly, but how then?
    connect(&textBrowser, SIGNAL(cursorPositionChanged()), &box, SLOT(close()));
    textBrowser.setText(msg);
    textBrowser.setStyleSheet("QTextEdit { background: transparent }");
    textBrowser.setAlignment(Qt::AlignCenter);
    textBrowser.setFrameShape(QFrame::NoFrame);
    box.setWidget(&textBrowser);
    box.setTimeout(QMaemo5InformationBox::NoTimeout);
    box.exec();
}

void Window::handMode (bool toggle)
{
    if (toggle)
    {
        currentHand = (HAND_RIGHT == currentHand) ? HAND_LEFT : HAND_RIGHT;

        QSettings settings;
        settings.setValue("hand", currentHand);
        hand->setText(handToNextString(currentHand));
    }

    if (HAND_LEFT == currentHand)
    {
        statsLayout->setAlignment(turnsLabel, Qt::AlignLeft);
        statsLayout->setAlignment(minTurnsWinLabel, Qt::AlignLeft);
        statsLayout->setAlignment(gamesWonPlayedLabel, Qt::AlignLeft);

        vl->setAlignment(buttonGroup, Qt::AlignLeft | Qt::AlignTop);
        vl->setAlignment(statsLayout, Qt::AlignLeft | Qt::AlignBottom);
        vl->setAlignment(lowerLayout, Qt::AlignLeft | Qt::AlignBottom);

        hl->setDirection(QBoxLayout::RightToLeft);
    }
    else
    {
        statsLayout->setAlignment(turnsLabel, Qt::AlignRight);
        statsLayout->setAlignment(minTurnsWinLabel, Qt::AlignRight);
        statsLayout->setAlignment(gamesWonPlayedLabel, Qt::AlignRight);

        vl->setAlignment(buttonGroup, Qt::AlignRight | Qt::AlignTop);
        vl->setAlignment(statsLayout, Qt::AlignRight | Qt::AlignBottom);
        vl->setAlignment(lowerLayout, Qt::AlignRight | Qt::AlignBottom);

        hl->setDirection(QBoxLayout::LeftToRight);
    }
}

void Window::orientationChanged()
{
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    if (screenGeometry.width() > screenGeometry.height())
    {
        buttonGroup->setLandscape();
        vl->setDirection(QBoxLayout::TopToBottom);
        handMode(false);
    }
    else
    {
        buttonGroup->setPortrait();
        vl->setDirection(QBoxLayout::BottomToTop);
        hl->setDirection(QBoxLayout::TopToBottom);
    }
}

void Window::updateBestResult (int newMinTurnsUsedToWin)
{
    QSettings settings;
    Board::BoardSize size = board->getSize();
    QString property = QString("stats/minTurnsUsedToWin%1").arg(size);
    int maxTurns = board->getNumTurnsOfSize(size);

    if (newMinTurnsUsedToWin)
    {
        if (minTurnsUsedToWin > newMinTurnsUsedToWin)
        {
            minTurnsUsedToWin = newMinTurnsUsedToWin;
            settings.setValue(property, minTurnsUsedToWin);
        }
    }
    else
    {
        minTurnsUsedToWin = settings.value(property, maxTurns).toInt();
    }

    //: best number of turns
    minTurnsWinLabel->setText(tr("Best record: %1/%2")
                              .arg(minTurnsUsedToWin)
                              .arg(maxTurns));
}

void Window::updateGamesWonPlayed (bool played, bool won)
{
    QSettings settings;
    Board::BoardSize size = board->getSize();
    QString wonProperty = QString("stats/gamesWon%1").arg(size);
    QString playedProperty = QString("stats/gamesPlayed%1").arg(size);

    if (!played && !won)
    {
        gamesWon = settings.value(wonProperty, 0).toInt();
        gamesPlayed = settings.value(playedProperty, 0).toInt();
    }
    else
    {
        gamesPlayed++;

        if (won)
            gamesWon++;

        settings.setValue(wonProperty, gamesWon);
        settings.setValue(playedProperty, gamesPlayed);
    }

    //: games won/played
    gamesWonPlayedLabel->setText(tr("Games won: %1/%2")
                                 .arg(gamesWon)
                                 .arg(gamesPlayed));
}
