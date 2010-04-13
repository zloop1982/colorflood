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

#ifndef _FIELD_HPP
#define _FIELD_HPP

#include <QWidget>
#include <QVector>
#include <QBrush>

class QPaintEvent;
class QMouseEvent;

class Field : public QWidget
{
    Q_OBJECT;

public:
    typedef enum
    {
        SIZE_SMALL = 0,
        SIZE_NORMAL,
        SIZE_LARGE,
        NUM_SIZES
    }FieldSize;

    typedef struct
    {
        quint8 brush;
        bool   flood;
    }FieldRect;

    typedef QVector<Field::FieldRect> RectVector;

    Field (QWidget *parent, int *turns);
    ~Field ();

    FieldSize getSize () const;
    void setSize (int size);

    static int getNumRectsOfSize (FieldSize size);
    static int getNumTurnsOfSize (FieldSize size);

private:
    static const int numRects[NUM_SIZES];
    static const int numTurns[NUM_SIZES];

    static int getRectSize (FieldSize size);
    void tryFloodRecurse (quint8 brush, int x, int y);
    void floodNeighbours (quint8 brush, int x, int y);

    FieldSize  size;
    RectVector data;
    int        turns;
    bool       finished;

protected:
    void paintEvent (QPaintEvent *event);

signals:
    void turnsChanged (int turns);

public slots:
    void randomize ();
    void flood (int colorIndex);
};

#endif // !_FIELD_HPP
