#!/bin/bash

for f in *
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]] || [[ $f == *".txt"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    echo "Processing $f"

    filename="${f%.*}"
    fne="${filename^^}_IMAGE"

    identify -ping -format '#define '${fne}'_WIDTH %w' $f >> ../resources.h
    echo -e '' >> ../resources.h

    identify -ping -format '#define '${fne}'_HEIGHT %h' $f >> ../resources.h
    echo -e '\n' >> ../resources.h

    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap eink-2color.png -depth 1 gray:- | xxd -i -n $fne | sed 's/unsigned/const unsigned/g' | sed '/_len = /d' >> ../resources.h
    echo -e '' >> ../resources.h

done
