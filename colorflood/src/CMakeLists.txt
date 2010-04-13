# Copyright 2010 Serge Ziryukin <ftrvxmtrx@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

set(src
  colorbuttons.cpp
  colorscheme.cpp
  field.cpp
  main.cpp
  window.cpp
  qrc_colorflood.cxx
  )

set(moc
  colorbuttons.hpp
  field.hpp
  fullscreenexitbutton.hpp
  window.hpp
  )

include(FindQt4)
find_package(Qt4 4.6.2 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

qt4_wrap_cpp(moc_src ${moc})
qt4_add_translation(colorflood.ts)
qt4_add_resources(resources colorflood.qrc)

add_definitions(-Wall -Wextra)
add_executable(colorflood ${moc_src} ${src} ${res})
target_link_libraries(colorflood ${QT_LIBRARIES})
