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

#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <QWidget>

class ButtonGroup;
class Board;
class QLabel;
class QAction;

/// main window class
class Window : public QWidget
{
    Q_OBJECT;

public:
    Window ();

private slots:
    /// update number of turns
    void updateTurns (int turns, bool gameFinished);
    /// switch to next scheme
    void scheme ();
    /// less cells
    void lessCells ();
    /// more cells
    void moreCells ();
    /// show help
    void help ();

private:
    void updateBestResult (int newMinTurnsUsedToWin = 0);
    void updateGamesWonPlayed (bool played = false, bool won = false);

    ButtonGroup *buttonGroup;
    Board *board;
    QLabel *turnsLabel;
    QLabel *minTurnsWinLabel;
    QLabel *gamesWonPlayedLabel;
    QAction *less;
    QAction *more;

    int minTurnsUsedToWin;
    int gamesWon;
    int gamesPlayed;
};

#endif // !_WINDOW_HPP
