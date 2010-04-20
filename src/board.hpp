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

#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <QWidget>
#include <QVector>
#include <QBrush>

class QPaintEvent;
class QMouseEvent;

class Board : public QWidget
{
    Q_OBJECT;

public:
    typedef enum
    {
        SIZE_SMALL = 0,
        SIZE_NORMAL,
        SIZE_LARGE,
        NUM_SIZES
    }BoardSize;

    typedef struct
    {
        quint8 brush;
        bool   flood;
    }Cell;

    typedef QVector<Board::Cell> CellVector;

    Board (QWidget *parent, int *turns);
    ~Board ();

    BoardSize getSize () const;
    void setSize (int size);

    static int getWidthInCells (BoardSize size);
    static int getNumTurnsOfSize (BoardSize size);

private:
    static const int boardWidthInCells[NUM_SIZES];
    static const int turnsForSize[NUM_SIZES];

    static int getCellSize (BoardSize size);
    void tryFloodRecurse (quint8 brush, int x, int y);
    void floodNeighbours (quint8 brush, int x, int y);

    BoardSize  size;
    CellVector cells;
    int        turns;
    bool       finished;

protected:
    void paintEvent (QPaintEvent *event);

signals:
    void turnsChanged (int turns);

public slots:
    void randomize ();
    void flood (int brushIndex);
};

#endif // !_BOARD_HPP
