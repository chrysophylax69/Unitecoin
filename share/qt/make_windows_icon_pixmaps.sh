#!/bin/bash
# create multiresolution windows icon
ICON_SRC=/home/mindfox/Desktop/Pictures/unite2.png
convert ${ICON_SRC} -resize 16x16 bitcoin16.png
convert ${ICON_SRC} -resize 32x32 bitcoin32.png
convert ${ICON_SRC} -resize 48x48 bitcoin48.png
convert ${ICON_SRC} -resize 64x64 bitcoin64.png
convert ${ICON_SRC} -resize 128x128 bitcoin128.png
convert ${ICON_SRC} -resize 256x256 bitcoin256.png

