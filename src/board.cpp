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
#include <QPropertyAnimation>
#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif
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
    for (auto &cell: rv)
        out << cell.color << cell.flood;

    return out;
}

static QDataStream &operator>> (QDataStream &in, Board::CellVector &rv)
{
    Board::Cell r;

    rv.clear();

    for (; !in.atEnd() ;)
    {
        in >> r.color >> r.flood;
        rv << r;
    }

    rv.pop_back();

    return in;
}

Board::Board (QWidget *parent, bool liveWallpaper, int *turns)
    : QWidget{parent},
      finished{false},
      liveWallpaper{liveWallpaper}
{
    Q_ASSERT(parent);

    setFixedSize(boardWidthInPixels, boardWidthInPixels);

    // restore board size and board itself from settings

    qRegisterMetaType<CellVector>("Board::CellVector");
    qRegisterMetaTypeStreamOperators<CellVector>("Board::CellVector");

    QSettings settings;

    auto size = settings.value("board/size", SIZE_SMALL).toInt();

    if (size < SIZE_SMALL || size >= NUM_SIZES)
        size = SIZE_SMALL;

    this->size = (BoardSize)size;

    if (settings.contains("board/cells"))
        cells = settings.value("board/cells").value<CellVector>();

    this->turns = settings.value("board/turns", 0).toInt();

    if (cells.size() != boardWidthInCells[size] * boardWidthInCells[size])
        randomize();

	if(turns != nullptr)
    	*turns = this->turns;

    auto scheme = Scheme::instance().getScheme();
    animatedFlood = scheme.at(cells[0].color);
    animate = new QPropertyAnimation(this, "animatedFlood", this);
    animate->setDuration(150);
}

Board::~Board ()
{
    if (!finished)
    {
        QSettings settings;

        settings.setValue("board/size", size);
        settings.setValue("board/cells", QVariant::fromValue(cells));
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
    auto cell = Cell{0, false, false};

    cells.clear();
    cells = CellVector(boardWidthInCells[size] * boardWidthInCells[size], cell);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    auto numColors = Scheme::instance().getScheme().size();

    for (auto &cell : cells)
    {
        cell.color = qrand() % numColors;
    }

    turns = 0;
    finished = false;
    emit gameStateChanged(turns, finished, false);

    // flood from top-left corner
    cells[0].flood = true;
    floodNeighbours(cells[0].color, 0, 0);

    update();
}

int Board::getNumTurnsOfSize (BoardSize size)
{
    return turnsForSize[size];
}

QColor Board::getAnimatedFlood () const
{
    return animatedFlood;
}

int Board::getCellSize (BoardSize size)
{
    return boardWidthInPixels / boardWidthInCells[size];
}

void Board::setAnimatedFlood (QColor color)
{
    animatedFlood = color;
    update();
}

void Board::tryFloodRecurse (quint8 color, int x, int y)
{
    auto &cell = cells[x + y*boardWidthInCells[size]];

    if (!cell.flood && cell.color == color)
    {
        cell.flood = true;
        cell.fresh = true;
        floodNeighbours(color, x, y);
    }
}

void Board::floodNeighbours (quint8 color, int x, int y)
{
    auto s = boardWidthInCells[size];

    cells[x + y*s].color = color;

    if (x > 0)
        tryFloodRecurse(color, x - 1, y);

    if (y > 0)
        tryFloodRecurse(color, x, y - 1);

    if (x < s - 1)
        tryFloodRecurse(color, x + 1, y);

    if (y < s - 1)
        tryFloodRecurse(color, x, y + 1);
}

void Board::paintEvent (QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    auto cell = QRect(0, 0, getCellSize(size), getCellSize(size));
    auto &scheme = Scheme::instance().getScheme();
    auto animating = (QAbstractAnimation::Running == animate->state());

    for (auto y = 0; y < boardWidthInCells[size] ;y++)
    {
        auto n = y * boardWidthInCells[size];

        for (int x = 0; x < boardWidthInCells[size] ;x++, n++)
        {
            cell.moveTo(x * cell.width(), y * cell.height());

            if (cell.intersects(event->rect()))
            {
                painter.fillRect(cell, scheme.at(cells[n].color));

                if (animating && cells[n].flood && !cells[n].fresh)
                    painter.fillRect(cell, animatedFlood);
            }
        }
    }

    painter.end();
}

void Board::flood (int colorIndex)
{
    // don't fill with the same color over and over again
    if (colorIndex == cells[0].color)
        return;

    if (finished)
        return;

    turns++;

    auto oldColor = cells[0].color;

    // clear 'fresh' flag
    for (auto y = 0; y < boardWidthInCells[size] ;y++)
    {
        auto n = y * boardWidthInCells[size];

        for (auto x = 0; x < boardWidthInCells[size] ;x++, n++)
            cells[n].fresh = false;
    }

    // flood with new color
    for (auto y = 0; y < boardWidthInCells[size] ;y++)
    {
        auto n = y * boardWidthInCells[size];

        for (auto x = 0; x < boardWidthInCells[size] ;x++, n++)
        {
            if (cells[n].flood)
                floodNeighbours(colorIndex, x, y);
        }
    }

    auto scheme = Scheme::instance().getScheme();
    animate->setStartValue(scheme.at(oldColor));
    animate->setEndValue(scheme.at(colorIndex));
    animate->start();

    auto allFlooded = true;

    // check if all board flooded
    for (auto &cell : cells)
    {
        if (!cell.flood)
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
        msg = (tr("You win!\nYou have used %1 turns out of %2.")
               .arg(turns)
               .arg(getNumTurnsOfSize(size)));
    }
    else if (getNumTurnsOfSize(size) == turns)
    {
        finished = true;
        /*: fail message */
        msg = tr("You lose!");
    }

    emit gameStateChanged(turns, finished, allFlooded);

    if (finished)
    {
#ifdef Q_WS_MAEMO_5
        QMaemo5InformationBox::information(liveWallpaper ? nullptr : this,
                                           msg,
                                           QMaemo5InformationBox::NoTimeout);
#else
        QMessageBox box(this);
        box.setWindowTitle("Color Flood");
        box.setText(msg);
        box.exec();
#endif
        randomize();
    }
}
