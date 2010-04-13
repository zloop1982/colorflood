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

#ifndef _FULLSCREENEXITBUTTON_HPP
#define _FULLSCREENEXITBUTTON_HPP

#include <QToolButton>
#include <QEvent>

class FullScreenExitButton : public QToolButton
{
    Q_OBJECT;

public:
    inline explicit FullScreenExitButton(QWidget *parent);

protected:
    inline bool eventFilter(QObject *obj, QEvent *ev);
};

FullScreenExitButton::FullScreenExitButton(QWidget *parent)
    : QToolButton(parent)
{
    Q_ASSERT(parent);

    // set the fullsize icon from Maemo's theme
    setIcon(QIcon::fromTheme(QLatin1String("general_fullsize")));

    // ensure that our size is fixed to our ideal size
    setFixedSize(sizeHint());

    // set the background to 0.5 alpha
    QPalette pal = palette();
    QColor backgroundColor = pal.color(backgroundRole());
    backgroundColor.setAlpha(128);
    pal.setColor(backgroundRole(), backgroundColor);
    setPalette(pal);

    // ensure that we're painting our background
    setAutoFillBackground(true);

    // when we're clicked, tell the parent to exit fullscreen
    connect(this, SIGNAL(clicked()), parent, SLOT(showNormal()));

    // install an event filter to listen for the parent's events
    parent->installEventFilter(this);
}

bool FullScreenExitButton::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != parent())
        return QToolButton::eventFilter(obj, ev);

    QWidget *parent = parentWidget();
    bool isFullScreen = parent->windowState() & Qt::WindowFullScreen;

    switch (ev->type()) {
    case QEvent::WindowStateChange:
        setVisible(isFullScreen);
        if (isFullScreen)
            raise();
        // fall through
    case QEvent::Resize:
        if (isVisible())
            move(parent->width() - width(),
                 parent->height() - height());
        break;
    default:
        break;
    }

    return QToolButton::eventFilter(obj, ev);
}

#endif // !_FULLSCREENEXITBUTTON_HPP
