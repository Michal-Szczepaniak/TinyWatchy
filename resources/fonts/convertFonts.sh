#!/bin/bash

fontconvert="../../.pio/libdeps/esp32dev/Adafruit GFX Library/fontconvert/fontconvert"
if [ -e "$fontconvert" ]; then
    echo "fontconvert exists"
else
    echo "fontconvert does not exist, creating it..."
    cd ../../.pio/libdeps/esp32dev/Adafruit\ GFX\ Library/fontconvert/
    make -j$(nproc)
    cd ../../../../../resources/fonts/
    if [ ! -e "$fontconvert" ]; then
        echo "Couldn't compile fontconvert, exiting"
        exit 1
    fi
fi

for d in *
do
    if [[ $d == *".sh"* ]] || [[ $d == *".h"* ]]; then
        continue
    fi

    if [ -f "$d" ]; then
        continue
    fi

    echo "Processing directory $d"
    for f in "$d"/*
    do
        if [[ $f != *".ttf"* ]]; then
            continue
        fi
        echo "Processing file $f"
        baseFilename="$(basename -- $f)"
        filename="${baseFilename%.*}"
        filenameUpper="${filename^^}"
        ./"$fontconvert" $f $d | sed 's/\w*Bitmaps/'${filenameUpper}'_'${d}'_BITMAPS/' | sed 's/\w*Glyphs/'${filenameUpper}'_'${d}'_GLYPHS/' | sed 's/GFXfont \w*/GFXfont '${filenameUpper}'_'${d}'/' | grep -v '// Approx.*' | sed 's/PROGMEM //' >> ../resources.h
    done
done
