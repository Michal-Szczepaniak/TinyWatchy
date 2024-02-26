#!/bin/bash

rm resources.h 1>/dev/null 2>/dev/null
touch resources.h

# shellcheck disable=SC2129
echo -e "#ifndef TINYWATCHY_RESOURCES_H" >> resources.h
echo -e "#define TINYWATCHY_RESOURCES_H" >> resources.h
echo -e '\nnamespace resources {\n' >> resources.h

echo "Processing images"
cd images/
bash convertImages.sh
cd ../

echo -e ''
echo "Processing fonts"
cd fonts/
bash convertFonts.sh
cd private/
bash convertFonts.sh
cd ../../

echo -e '};\n' >> resources.h
echo -e '#endif //TINYWATCHY_RESOURCES_H' >> resources.h

mv resources.h ../src/