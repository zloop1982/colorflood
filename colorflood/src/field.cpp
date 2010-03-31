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

#include <QtGui>
#include "field.hpp"

static const int fieldWidth = 420;

const int Field::rects[Field::NUM_SIZES] = { 14, 21, 28 };
const int Field::turns[Field::NUM_SIZES] = { 25, 35, 50 };

Field::Field (QWidget *parent)
    : QWidget (parent)
{
    // FIXME -- restore saved state
}

Field::Field (QWidget *parent, const QVector<QBrush> &brushes, FieldSize size)
    : QWidget (parent)
{
    init(brushes, size);
}

Field::~Field ()
{
    // FIXME -- save state
}

Field::FieldSize Field::getSize () const
{
    return size;
}

/*
=================
randomize
=================
*/
void Field::randomize ()
{
    Field::FieldRect rect;
    rect.flood = false;

    data.clear();
    data = QVector<FieldRect> (rects[size] * rects[size], rect);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    for (QVector<Field::FieldRect>::iterator rect = data.begin();
         rect != data.end();
         rect++)
    {
        (*rect).brush = qrand() % brushes.size();
    }
}

int Field::getNumRectsOfSize (FieldSize size)
{
    return rects[size];
}

int Field::getNumTurnsOfSize (FieldSize size)
{
    return turns[size];
}

void Field::init (const QVector<QBrush> &brushes, FieldSize size)
{
    this->size = size;
    this->brushes = brushes;

    setFixedSize(fieldWidth, fieldWidth);
    randomize();
}

int Field::getRectSize (FieldSize size)
{
    return fieldWidth / rects[size];
}

void Field::paintEvent (QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    QRect rect = QRect(0, 0, getRectSize(size), getRectSize(size));

    for (int y = 0; y < rects[size] ;y++)
    {
        int n = y * rects[size];

        for (int x = 0; x < rects[size] ;x++, n++)
        {
            rect.moveTo(x * rect.width(), y * rect.height());

            if (rect.intersects(event->rect()))
                painter.fillRect(rect, brushes.at(data[n].brush));
        }
    }

    painter.end();
}
