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

    Field (QWidget *parent, const QVector<QBrush> &brushes, FieldSize size);
    Field (QWidget *parent);
    ~Field ();

    FieldSize getSize () const;
    void randomize ();

    static int getNumRectsOfSize (FieldSize size);
    static int getNumTurnsOfSize (FieldSize size);

private:
    typedef struct
    {
        char brush;
        bool flood;
    }FieldRect;

    static const int rects[NUM_SIZES];
    static const int turns[NUM_SIZES];

    void init (const QVector<QBrush> &brushes, FieldSize size);
    static int getRectSize (FieldSize size);

    FieldSize          size;
    QVector<QBrush>    brushes;
    QVector<FieldRect> data;

protected:
    void paintEvent (QPaintEvent *event);
};

#endif // !_FIELD_HPP
