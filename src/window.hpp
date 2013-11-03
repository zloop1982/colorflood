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

#pragma once

#include <QWidget>

class ButtonGroup;
class Board;
class QLabel;
class QAction;
class QBoxLayout;
class QPushButton;
class FullScreenToggleButton;

/// main window class
class Window : public QWidget
{
    Q_OBJECT;

public:
    Window ();

private slots:
    /// update game state
    void updateGameState (int turns, bool gameFinished, bool won);
    /// switch to next scheme
    void scheme ();
    /// less cells
    void lessCells ();
    /// more cells
    void moreCells ();
    /// show help
    void help ();
    /// switch right/left-handed mode
    void handMode (bool toggle = true);
    /// switch between portrait and landscape
    void orientationChanged ();

private:
    enum
    {
        HAND_RIGHT = 0,
        HAND_LEFT
    };

    static QString handToNextString (int hand)
        {
            return (HAND_RIGHT == hand ?
                    tr("Left-handed mode") :
                    tr("Right-handed mode"));
        }

    static bool isPortraitMode ();

    void updateBestResult (int newMinTurnsUsedToWin = 0);
    void updateGamesWonPlayed (bool played = false, bool won = false);

    void layoutLandscape ();
    void layoutPortrait ();

    ButtonGroup *buttonGroup;
    Board *board;
    QLabel *turnsLabel;
    QLabel *minTurnsWinLabel;
    QLabel *gamesWonPlayedLabel;
    QAction *less;
    QAction *more;
    QAction *hand;

    QBoxLayout *lowerLayout = nullptr;
    QBoxLayout *statsLayout = nullptr;
    QBoxLayout *hl = nullptr;
    QBoxLayout *vl = nullptr;

    QPushButton *newGame;
    FullScreenToggleButton *fsButton;

    int minTurnsUsedToWin;
    int gamesWon = 0;
    int gamesPlayed = 0;
    int currentHand;
    bool liveWallpaper;
};
