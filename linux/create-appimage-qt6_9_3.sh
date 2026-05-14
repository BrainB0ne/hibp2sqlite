#!/bin/bash

# make sure to place linuxdeploy-plugin-qt-x86_64.AppImage in the same folder.
export LD_LIBRARY_PATH=/usr/local/Qt-6.9.3/lib:$LD_LIBRARY_PATH
export QMAKE=/usr/local/Qt-6.9.3/bin/qmake

./linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage --plugin qt

