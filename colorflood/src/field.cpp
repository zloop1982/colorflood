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
#include "colorscheme.hpp"

static const int fieldWidth = 420;

const int Field::rects[Field::NUM_SIZES] = { 14, 21, 28 };
const int Field::turns[Field::NUM_SIZES] = { 25, 35, 50 };

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

Field::Field (QWidget *parent)
    : QWidget (parent)
{
    setFixedSize(fieldWidth, fieldWidth);

    // restore field size and field itself from settings

    qRegisterMetaType<RectVector>("Field::RectVector");
    qRegisterMetaTypeStreamOperators<RectVector>("Field::RectVector");

    QSettings settings;

    int size = settings.value("field/fieldSize", SIZE_SMALL).toInt();

    if (size < SIZE_SMALL || size >= NUM_SIZES)
        size = SIZE_SMALL;

    this->size = (FieldSize)size;

    if (settings.contains("field/data"))
        data = settings.value("field/data").value<RectVector>();

    if (data.size() != rects[size] * rects[size])
        randomize();
}

Field::~Field ()
{
    QSettings settings;

    settings.setValue("field/size", size);

    QVariant v;
    v.setValue(data);
    settings.setValue("field/data", v);
}

Field::FieldSize Field::getSize () const
{
    return size;
}

void Field::randomize ()
{
    FieldRect rect;
    rect.flood = false;

    data.clear();
    data = RectVector(rects[size] * rects[size], rect);

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    int numBrushes = ColorScheme::instance().getScheme().size();

    for (QVector<FieldRect>::iterator rect = data.begin();
         rect != data.end();
         rect++)
    {
        (*rect).brush = qrand() % numBrushes;
    }

    update();
}

int Field::getNumRectsOfSize (FieldSize size)
{
    return rects[size];
}

int Field::getNumTurnsOfSize (FieldSize size)
{
    return turns[size];
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

    const QVector<QBrush> &scheme = ColorScheme::instance().getScheme();

    for (int y = 0; y < rects[size] ;y++)
    {
        int n = y * rects[size];

        for (int x = 0; x < rects[size] ;x++, n++)
        {
            rect.moveTo(x * rect.width(), y * rect.height());

            if (rect.intersects(event->rect()))
                painter.fillRect(rect, scheme.at(data[n].brush));
        }
    }

    painter.end();
}
