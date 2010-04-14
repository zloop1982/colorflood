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

#include <QMetaType>
#include <QDataStream>
#include <QSettings>
#include <QTime>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include "field.hpp"
#include "colorscheme.hpp"

static const int fieldWidth = 420;

const int Field::numRects[Field::NUM_SIZES] = { 14, 21, 28 };
const int Field::numTurns[Field::NUM_SIZES] = { 25, 35, 50 };

// we declare out QVector<FieldRect> metatype
// and stream operators to save whole field to settings
Q_DECLARE_METATYPE(Field::RectVector);

static QDataStream &operator<< (QDataStream &out, const Field::RectVector &rv)
{
    for (QVector<Field::FieldRect>::const_iterator rect = rv.begin();
         rect != rv.end();
         rect++)
    {
        out << (*rect).brush;
        out << (*rect).flood;
    }

    return out;
}

static QDataStream &operator>> (QDataStream &in, Field::RectVector &rv)
{
    Field::FieldRect r;

    rv.clear();

    for (; !in.atEnd() ;)
    {
        in >> r.brush >> r.flood;
        rv << r;
    }

    rv.pop_back();

    return in;
}

Field::Field (QWidget *parent, int *turns)
    : QWidget (parent),
      finished(false)
{
    Q_ASSERT(parent);

    setFixedSize(fieldWidth, fieldWidth);

    // restore field size and field itself from settings

    qRegisterMetaType<RectVector>("Field::RectVector");
    qRegisterMetaTypeStreamOperators<RectVector>("Field::RectVector");

    QSettings settings;

    int size = settings.value("field/size", SIZE_SMALL).toInt();

    if (size < SIZE_SMALL || size >= NUM_SIZES)
        size = SIZE_SMALL;

    this->size = (FieldSize)size;

    if (settings.contains("field/data"))
        data = settings.value("field/data").value<RectVector>();

    this->turns = settings.value("field/turns", 0).toInt();

    if (data.size() != numRects[size] * numRects[size])
        randomize();

    *turns = this->turns;
}

Field::~Field ()
{
    if (!finished)
    {
        QSettings settings;

        settings.setValue("field/size", size);

        QVariant v;
        v.setValue(data);
        settings.setValue("field/data", v);

        settings.setValue("field/turns", turns);
    }
}

Field::FieldSize Field::getSize () const
{
    return size;
}

void Field::setSize (int size)
{
    Q_ASSERT(size >= 0 && size < NUM_SIZES);

    if (this->size == size)
        return;

    this->size = (FieldSize)size;
    randomize();
}

void Field::randomize ()
{
    FieldRect rect;
    rect.flood = false;

    data.clear();
    data = RectVector(numRects[size] * numRects[size], rect);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    int numBrushes = ColorScheme::instance().getScheme().size();

    for (QVector<FieldRect>::iterator rect = data.begin();
         rect != data.end();
         rect++)
    {
        (*rect).brush = qrand() % numBrushes;
    }

    turns = 0;
    finished = false;
    emit turnsChanged(turns);

    // flood from top-left
    data[0].flood = true;
    floodNeighbours(data[0].brush, 0, 0);

    update();
}

int Field::getNumRectsOfSize (FieldSize size)
{
    return numRects[size];
}

int Field::getNumTurnsOfSize (FieldSize size)
{
    return numTurns[size];
}

int Field::getRectSize (FieldSize size)
{
    return fieldWidth / numRects[size];
}

void Field::tryFloodRecurse (quint8 brush, int x, int y)
{
    FieldRect &rect = data[x + y*numRects[size]];

    if (!rect.flood && rect.brush == brush)
    {
        rect.flood = true;
        floodNeighbours(brush, x, y);
    }
}

void Field::floodNeighbours (quint8 brush, int x, int y)
{
    int s = numRects[size];

    data[x + y*s].brush = brush;

    if (x > 0)
        tryFloodRecurse(brush, x - 1, y);

    if (y > 0)
        tryFloodRecurse(brush, x, y - 1);

    if (x < s - 1)
        tryFloodRecurse(brush, x + 1, y);
        
    if (y < s - 1)
        tryFloodRecurse(brush, x, y + 1);
}

void Field::paintEvent (QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    QRect rect = QRect(0, 0, getRectSize(size), getRectSize(size));

    const QVector<QBrush> &scheme = ColorScheme::instance().getScheme();

    for (int y = 0; y < numRects[size] ;y++)
    {
        int n = y * numRects[size];

        for (int x = 0; x < numRects[size] ;x++, n++)
        {
            rect.moveTo(x * rect.width(), y * rect.height());

            if (rect.intersects(event->rect()))
                painter.fillRect(rect, scheme.at(data[n].brush));
        }
    }

    painter.end();
}

void Field::flood (int colorIndex)
{
    // don't fill with the same color over and over again
    if (colorIndex == data[0].brush)
        return;

    if (finished)
        return;

    emit turnsChanged(++turns);

    // flood with new color
    for (int y = 0; y < numRects[size] ;y++)
    {
        int n = y * numRects[size];

        for (int x = 0; x < numRects[size] ;x++, n++)
        {
            if (data[n].flood)
                floodNeighbours(colorIndex, x, y);
        }
    }

    repaint();

    bool allFlooded = true;

    // check if all field flooded
    for (QVector<Field::FieldRect>::const_iterator rect = data.begin();
         rect != data.end();
         rect++)
    {
        if (!(*rect).flood)
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
        msg = tr("You won!");
    }
    else if (getNumTurnsOfSize(size) == turns)
    {
        finished = true;
        /*: fail message */
        msg = tr("You lost!");
    }

    if (finished)
    {
        QMessageBox box;
        box.setWindowTitle("Color Flood");
        box.setText(msg);
        box.exec();
        randomize();
    }
}
