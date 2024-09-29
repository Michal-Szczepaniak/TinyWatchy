#!/bin/bash

for f in $(find . -name '*.png');
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink_2color.png" ]] || [[ $f == *".txt"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    echo "Processing $f"

    baseFilename="$(basename -- $f)"
    filename="${baseFilename%.*}"
    fne="${filename^^}_IMAGE"

    identify -ping -format '#define '${fne}'_WIDTH %w' $f >> ../resources.h
    echo -e '' >> ../resources.h

    identify -ping -format '#define '${fne}'_HEIGHT %h' $f >> ../resources.h
    echo -e '\n' >> ../resources.h

    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink_2color.png -negate -depth 1 ${fne}
    xxd -i ${fne} | sed 's/unsigned/const unsigned/g' | sed '/_len = /d' >> ../resources.h
    echo -e '' >> ../resources.h
    rm ${fne}

done
