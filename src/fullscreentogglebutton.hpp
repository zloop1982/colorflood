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

#include <QToolButton>

class FullScreenToggleButton : public QToolButton
{
    Q_OBJECT;

public:
    inline explicit FullScreenToggleButton (QWidget *parent);

private slots:
    inline void toggleFullscreen ();
};

FullScreenToggleButton::FullScreenToggleButton(QWidget *parent)
    : QToolButton(parent)
{
    Q_ASSERT(parent);

    // set the fullsize icon from Maemo's theme
    setIcon(QIcon::fromTheme(QLatin1String("general_fullsize")));

    // ensure that our size is fixed to our ideal size
    setFixedSize(sizeHint());

    // set the background to 0.5 alpha
    auto pal = palette();
    auto backgroundColor = pal.color(backgroundRole());
    backgroundColor.setAlpha(128);
    pal.setColor(backgroundRole(), backgroundColor);
    setPalette(pal);

    // ensure that we're painting our background
    setAutoFillBackground(true);

    // when we're clicked, toggle fullscreen
    connect(this, SIGNAL(clicked()), this, SLOT(toggleFullscreen()));
}

void FullScreenToggleButton::toggleFullscreen ()
{
    auto parent = parentWidget();

    if (parent->isFullScreen())
        parent->showNormal();
    else
        parent->showFullScreen();
}
