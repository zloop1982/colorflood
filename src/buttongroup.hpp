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

#ifndef _BUTTONGROUP_HPP
#define _BUTTONGROUP_HPP

#include <QGroupBox>
#include <QButtonGroup>

/// button group widget class
class ButtonGroup : public QGroupBox
{
    Q_OBJECT;

public:
    ButtonGroup (QWidget *parent);
    void setPortrait();
    void setLandscape();

signals:
    /// flood with specific color
    void flood (int colorIndex);

private:
    QButtonGroup group;
};

#endif // !_BUTTONGROUP_HPP
