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
#include <QVector>
#include <QColor>

class QPaintEvent;
class QMouseEvent;
class QPropertyAnimation;

/// board class
class Board : public QWidget
{
    Q_OBJECT;
    Q_PROPERTY(QColor animatedFlood
               READ getAnimatedFlood
               WRITE setAnimatedFlood);

public:
    /// board sizes
    typedef enum
    {
        SIZE_SMALL = 0, ///< 14x14, 25 turns
        SIZE_NORMAL,    ///< 21x21, 35 turns
        SIZE_LARGE,     ///< 28x28, 50 turns
        NUM_SIZES,
    }BoardSize;

    /// cell type
    typedef struct
    {
        quint8 color; ///< color index
        bool   flood; ///< 'cell filled' flag
        bool   fresh; ///< 'just filled' flag
    }Cell;

    using CellVector = QVector<Board::Cell>;

    Board (QWidget *parent, bool liveWallpaper, int *turns);
    ~Board ();

    /// get board size
    BoardSize getSize () const;
    /// set new board size and randomize cells
    void setSize (int size);

    static int getNumTurnsOfSize (BoardSize size);

    void setAnimatedFlood (QColor color);
    QColor getAnimatedFlood () const;

private:
    /// number of cells for specific board size
    static const int boardWidthInCells[NUM_SIZES];
    /// number of turns for specific board size
    static const int turnsForSize[NUM_SIZES];

    /// get cell size in pixels for specific board size
    static int getCellSize (BoardSize size);
    /// flood recursively if a cell isn't filled yet
    void tryFloodRecurse (quint8 color, int x, int y);
    /// flood neighbouring cells
    void floodNeighbours (quint8 color, int x, int y);

    BoardSize  size;          ///< board size
    CellVector cells;         ///< cells data
    int        turns;         ///< number of turns used
    bool       finished;      ///< 'game finished' flag
    bool       liveWallpaper; ///< LiveWallpaper mode
    QColor     animatedFlood;
    QPropertyAnimation *animate;

protected:
    void paintEvent (QPaintEvent *event);

signals:
    void gameStateChanged (int turns, bool gameFinished, bool won);

public slots:
    /// randomize cells
    void randomize ();
    /// flood with specific color
    void flood (int colorIndex);
};
