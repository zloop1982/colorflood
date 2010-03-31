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
  field.cpp
  main.cpp
  window.cpp
  )

set(moc
  field.hpp
  window.hpp
  )

include(FindQt4)
find_package(Qt4 4.6.2 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

QT4_WRAP_CPP(moc_src ${moc})

add_executable(colorflood ${moc_src} ${src})
target_link_libraries(colorflood ${QT_LIBRARIES})
