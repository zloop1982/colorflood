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

#include <QMetaType>
#include <QDataStream>
#include <QSettings>
#include <QTime>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include "board.hpp"
#include "scheme.hpp"

/// board dimensions in pixels
static const int boardWidthInPixels = 420;

const int Board::boardWidthInCells[Board::NUM_SIZES] = { 14, 21, 28 };
const int Board::turnsForSize[Board::NUM_SIZES] = { 25, 35, 50 };

// we declare out QVector<Cell> metatype
// and stream operators to save whole board to settings
Q_DECLARE_METATYPE(Board::CellVector);

static QDataStream &operator<< (QDataStream &out, const Board::CellVector &rv)
{
    for (QVector<Board::Cell>::const_iterator cell = rv.begin();
         cell != rv.end();
         cell++)
    {
        out << (*cell).brush;
        out << (*cell).flood;
    }

    return out;
}

static QDataStream &operator>> (QDataStream &in, Board::CellVector &rv)
{
    Board::Cell r;

    rv.clear();

    for (; !in.atEnd() ;)
    {
        in >> r.brush >> r.flood;
        rv << r;
    }

    rv.pop_back();

    return in;
}

Board::Board (QWidget *parent, int *turns)
    : QWidget (parent),
      finished(false)
{
    Q_ASSERT(parent);

    setFixedSize(boardWidthInPixels, boardWidthInPixels);

    // restore board size and board itself from settings

    qRegisterMetaType<CellVector>("Board::CellVector");
    qRegisterMetaTypeStreamOperators<CellVector>("Board::CellVector");

    QSettings settings;

    int size = settings.value("board/size", SIZE_SMALL).toInt();

    if (size < SIZE_SMALL || size >= NUM_SIZES)
        size = SIZE_SMALL;

    this->size = (BoardSize)size;

    if (settings.contains("board/cells"))
        cells = settings.value("board/cells").value<CellVector>();

    this->turns = settings.value("board/turns", 0).toInt();

    if (cells.size() != boardWidthInCells[size] * boardWidthInCells[size])
        randomize();

    *turns = this->turns;
}

Board::~Board ()
{
    if (!finished)
    {
        QSettings settings;

        settings.setValue("board/size", size);

        QVariant v;
        v.setValue(cells);
        settings.setValue("board/cells", v);

        settings.setValue("board/turns", turns);
    }
}

Board::BoardSize Board::getSize () const
{
    return size;
}

void Board::setSize (int size)
{
    Q_ASSERT(size >= 0 && size < NUM_SIZES);

    if (this->size == size)
        return;

    this->size = (BoardSize)size;
    randomize();
}

void Board::randomize ()
{
    Cell cell;

    cell.brush = 0;
    cell.flood = false;

    cells.clear();
    cells = CellVector(boardWidthInCells[size] * boardWidthInCells[size], cell);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    int numBrushes = Scheme::instance().getScheme().size();

    for (QVector<Cell>::iterator cell = cells.begin();
         cell != cells.end();
         cell++)
    {
        (*cell).brush = qrand() % numBrushes;
    }

    turns = 0;
    finished = false;
    emit turnsChanged(turns, finished);

    // flood from top-left corner
    cells[0].flood = true;
    floodNeighbours(cells[0].brush, 0, 0);

    update();
}

int Board::getWidthInCells (BoardSize size)
{
    return boardWidthInCells[size];
}

int Board::getNumTurnsOfSize (BoardSize size)
{
    return turnsForSize[size];
}

int Board::getCellSize (BoardSize size)
{
    return boardWidthInPixels / boardWidthInCells[size];
}

void Board::tryFloodRecurse (quint8 brush, int x, int y)
{
    Cell &cell = cells[x + y*boardWidthInCells[size]];

    if (!cell.flood && cell.brush == brush)
    {
        cell.flood = true;
        floodNeighbours(brush, x, y);
    }
}

void Board::floodNeighbours (quint8 brush, int x, int y)
{
    int s = boardWidthInCells[size];

    cells[x + y*s].brush = brush;

    if (x > 0)
        tryFloodRecurse(brush, x - 1, y);

    if (y > 0)
        tryFloodRecurse(brush, x, y - 1);

    if (x < s - 1)
        tryFloodRecurse(brush, x + 1, y);
        
    if (y < s - 1)
        tryFloodRecurse(brush, x, y + 1);
}

void Board::paintEvent (QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    QRect cell = QRect(0, 0, getCellSize(size), getCellSize(size));

    const QVector<QBrush> &scheme = Scheme::instance().getScheme();

    for (int y = 0; y < boardWidthInCells[size] ;y++)
    {
        int n = y * boardWidthInCells[size];

        for (int x = 0; x < boardWidthInCells[size] ;x++, n++)
        {
            cell.moveTo(x * cell.width(), y * cell.height());

            if (cell.intersects(event->rect()))
                painter.fillRect(cell, scheme.at(cells[n].brush));
        }
    }

    painter.end();
}

void Board::flood (int brushIndex)
{
    // don't fill with the same brush over and over again
    if (brushIndex == cells[0].brush)
        return;

    if (finished)
        return;

    turns++;

    // flood with new brush
    for (int y = 0; y < boardWidthInCells[size] ;y++)
    {
        int n = y * boardWidthInCells[size];

        for (int x = 0; x < boardWidthInCells[size] ;x++, n++)
        {
            if (cells[n].flood)
                floodNeighbours(brushIndex, x, y);
        }
    }

    repaint();

    bool allFlooded = true;

    // check if all board flooded
    for (QVector<Board::Cell>::const_iterator cell = cells.begin();
         cell != cells.end();
         cell++)
    {
        if (!(*cell).flood)
        {
            allFlooded = false;
            break;
        }
    }

    QString msg;

    if (allFlooded)
    {
        finished = true;
        /*: win message */
        msg = (tr("You won!\nYou used %1 turns out of %2.")
               .arg(turns)
               .arg(getNumTurnsOfSize(size)));
    }
    else if (getNumTurnsOfSize(size) == turns)
    {
        finished = true;
        /*: fail message */
        msg = tr("You lost!");
    }

    emit turnsChanged(turns, finished);

    if (finished)
    {
        QMessageBox box;
        box.setWindowTitle("Color Flood");
        box.setText(msg);
        box.exec();
        randomize();
    }
}
