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

#include <QColor>
#include <QGridLayout>
#include "buttongroup.hpp"
#include "scheme.hpp"
#include "button.hpp"

ButtonGroup::ButtonGroup (QWidget *parent)
    : QGroupBox(parent)
{
    Q_ASSERT(parent);

    const QVector<QColor> &scheme = Scheme::instance().getScheme();
    QGridLayout *layout = new QGridLayout;

    for (int i = 0; i < scheme.size(); i++)
    {
        Button *button = new Button(this, i);
        group.addButton(button, i);
        layout->addWidget(group.button(i), (i - (i % 3)) / 3, (i % 3));
    }

    layout->setSpacing(24);
    setLayout(layout);

    QObject::connect(&group, SIGNAL(buttonClicked(int)), this, SIGNAL(flood(int)));
}
