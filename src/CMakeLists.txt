# Copyright (C) 2010 Serge Ziryukin <ftrvxmtrx@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(src
  board.cpp
  button.cpp
  buttongroup.cpp
  main.cpp
  scheme.cpp
  qrc_colorflood.cxx
  window.cpp
  )

set(moc
  board.hpp
  button.hpp
  buttongroup.hpp
  fullscreentogglebutton.hpp
  window.hpp
  )

include(FindQt)
find_package(Qt 4.6.2 COMPONENTS QtCore QtGui REQUIRED)
find_package(Qt 4.6.2 COMPONENTS QtMaemo5)
include(${QT_USE_FILE})

qt4_wrap_cpp(moc_src ${moc})
qt4_add_resources(resources resources/colorflood.qrc)

add_definitions(-std=c++11 -Wall -Wextra -I${QT_INCLUDE_DIR}/QtMaemo5)
add_executable(colorflood ${moc_src} ${src} ${res})
target_link_libraries(colorflood ${QT_LIBRARIES})
if(Qt_QtMaemo5_FOUND)
  target_link_libraries(colorflood QtMaemo5)
endif(Qt_QtMaemo5_FOUND)

install(TARGETS colorflood DESTINATION bin)
install(FILES resources/images/icon_48x48.png DESTINATION share/icons/hicolor/48x48/apps RENAME colorflood.png)
install(FILES resources/colorflood.desktop DESTINATION share/applications/hildon)
