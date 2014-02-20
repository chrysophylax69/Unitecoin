#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/bitcoin_testnet.png
ICON_DST=../../src/qt/res/icons/bitcoin_testnet.ico
convert ${ICON_SRC} -resize 16x16 bitcoin-16_testnet.png
convert ${ICON_SRC} -resize 32x32 bitcoin-32_testnet.png
convert ${ICON_SRC} -resize 48x48 bitcoin-48_testnet.png
convert ${ICON_SRC} -resize 64x64 bitcoin-64_testnet.png
convert ${ICON_SRC} -resize 256x256 bitcoin-256_testnet.png
convert bitcoin-16_testnet.png bitcoin-32_testnet.png bitcoin-48_testnet.png bitcoin-64_testnet.png bitcoin-256_testnet.png ${ICON_DST}

