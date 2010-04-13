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

#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include "colorbuttons.hpp"
#include "colorscheme.hpp"

ColorButtons::ColorButtons (QWidget *parent)
    : QGroupBox(parent)
{
    Q_ASSERT(parent);

    const QVector<QBrush> &scheme = ColorScheme::instance().getScheme();
    QVBoxLayout *layout = new QVBoxLayout;

    for (int i = 0; i < scheme.size(); i++)
    {
        QPixmap pixmap(64, 64);
        QPainter painter;
        painter.begin(&pixmap);
        painter.fillRect(pixmap.rect(), scheme.at(i));
        painter.end();

        QPushButton *button = new QPushButton(pixmap, "", this);
        button->setFixedSize(48, 48);
        layout->addWidget(button);
        group.addButton(button, i);
    }

    QObject::connect(&group,
                     SIGNAL(buttonClicked(int)),
                     this,
                     SIGNAL(flood(int)));

    setLayout(layout);
}
