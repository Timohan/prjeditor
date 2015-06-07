#!/bin/bash
VERSION="0.1-1"
mkdir -p prj-editor_$VERSION/usr/local/bin
cp ../../build-prj-editor-Desktop_Qt_5_4_1_GCC_64bit-Release/Prj-Editor prj-editor_$VERSION/usr/local/bin
mkdir -p prj-editor_$VERSION/usr/share/prj-editor/
cp ../xml/highlights.xml prj-editor_$VERSION/usr/share/prj-editor/
chown -R root:root prj-editor_$VERSION/usr
mkdir -p prj-editor_$VERSION/DEBIAN
cp control prj-editor_$VERSION/DEBIAN
dpkg-deb --build prj-editor_$VERSION
chmod +x prj-editor_$VERSION.deb
rm -rf prj-editor_$VERSION

