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

#pragma once

#include <QPushButton>
#include <QPainterPath>

/// color button class
class Button : public QPushButton
{
    Q_OBJECT;

public:
    Button (QWidget *parent, int colorIndex);

private:
    /// color index
    const int colorIndex;

    static const int buttonWidthInPixels = 64;
    static const int buttonCornerRadiusInPixels = 12;

    static QPainterPath &getPath ()
        {
            static QPainterPath path;

            if (path.isEmpty())
            {
                path.addRoundedRect(1,
                                    1,
                                    buttonWidthInPixels - 2,
                                    buttonWidthInPixels - 2,
                                    buttonCornerRadiusInPixels,
                                    buttonCornerRadiusInPixels);
            }

            return path;
        }

protected:
    void paintEvent (QPaintEvent *event);
};
