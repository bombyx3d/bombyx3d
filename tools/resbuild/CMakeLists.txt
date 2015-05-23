#
# Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

project(ResBuild)

include(cmake/CompileUIFiles.cmake)

find_package(Qt5Widgets REQUIRED)
find_package(QT5Gui REQUIRED)
find_package(QT5Core REQUIRED)
find_package(QT5Xml REQUIRED)
find_package(Qt5OpenGL REQUIRED)
set(QT_LIBRARIES Qt5::OpenGL Qt5::Widgets Qt5::Gui Qt5::Xml Qt5::Core)

set(CMAKE_AUTOMOC YES)
set(CMAKE_AUTOUIC YES)
set(CMAKE_AUTORCC YES)
set(CMAKE_INCLUDE_CURRENT_DIR YES)

set(ui_files
    builders/BinaryFileBuilder.ui
    BuildProgressWidget.ui
    MainWindow.ui
    NewRuleDialog.ui
)

set(source_files
    ${ui_files}
    res/resources.qrc
    builders/BinaryFileBuilder.cpp
    builders/BinaryFileBuilder.h
    BuildDirectory.cpp
    BuildDirectory.h
    Builder.cpp
    Builder.h
    BuildProgressWidget.cpp
    BuildProgressWidget.h
    FileUtil.cpp
    FileUtil.h
    MainWindow.cpp
    MainWindow.h
    NewRuleDialog.cpp
    NewRuleDialog.h
    main.cpp
    Project.cpp
    Project.h
    Rule.cpp
    Rule.h
)

compile_ui_files(compiled_ui_files ${ui_files})

source_group("UI Files" FILES ${compiled_ui_files})
z_set_source_groups(${source_files})

add_executable(ResBuild ${compiled_ui_files} ${source_files})

set_target_properties(ResBuild PROPERTIES CXX_STANDARD 11)
target_link_libraries(ResBuild libsqlite3 ${QT_LIBRARIES})
