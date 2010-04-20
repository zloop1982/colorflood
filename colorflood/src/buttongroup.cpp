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
#include <QGridLayout>
#include "buttongroup.hpp"
#include "scheme.hpp"
#include "button.hpp"

ButtonGroup::ButtonGroup (QWidget *parent)
    : QGroupBox(parent)
{
    Q_ASSERT(parent);

    const QVector<QBrush> &scheme = Scheme::instance().getScheme();
    QGridLayout *layout = new QGridLayout;

    for (int i = 0; i < scheme.size(); i++)
    {
        Button *button = new Button(this, i);
        layout->addWidget(button, (i - (i % 3)) / 3, i % 3);
        group.addButton(button, i);
    }

    QObject::connect(&group,
                     SIGNAL(buttonClicked(int)),
                     this,
                     SIGNAL(flood(int)));

    layout->setSpacing(24);
    setLayout(layout);
}
