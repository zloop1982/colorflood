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
#include "window.hpp"

ButtonGroup::ButtonGroup (QWidget *parent)
    : QGroupBox(parent)
{
    Q_ASSERT(parent);

    const QVector<QColor> &scheme = Scheme::instance().getScheme();

    for (int i = 0; i < scheme.size(); i++)
    {
        Button *button = new Button(this, i);
        group.addButton(button, i);
    }

    QObject::connect(&group, SIGNAL(buttonClicked(int)), this, SIGNAL(flood(int)));

    rearrangeButtons(Window::isPortraitMode());
}

void ButtonGroup::rearrangeButtons (bool portraitMode)
{
    const QVector<QColor> &scheme = Scheme::instance().getScheme();
    QGridLayout *newLayout = new QGridLayout;

    delete layout();

    for (int i = 0; i < scheme.size(); i++)
    {
        int x = portraitMode ? 0 : (i - (i % 3)) / 3;
        int y = portraitMode ? i : (i % 3);

        newLayout->addWidget(group.button(i), x, y);
    }

    newLayout->setSpacing(24);
    setLayout(newLayout);
}
