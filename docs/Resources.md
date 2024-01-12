# Resources

All the fonts and images that are used are generated from files contained in the `resources/` directory.

## Directory structure

Inside `resources/` directory there are two directories:

- `fonts/`: Contains `ttf` fonts with various sizes
- `images/`: Contains images

In main directory there's `generate.sh` used to convert the files into code stored within `resources.h`

## Adding fonts

To add font, place it inside `fonts/` directory. Every subdirectory has a number meaning the size of the font. Don't worry about duplicating the files. Don't forget to place license for the font you're using, preferably with the same name as the font file. 

Font files have to follow snake_case as in words have to be separated with underscores, without whitespaces. To learn about snake_case go to [wikipedia](https://en.wikipedia.org/wiki/Snake_case)

After you run `generate.sh`, you should be able to use the font. If the font name is `fonts/10/font_name.ttf` it should be available as `FONT_NAME_10`.

## Adding images

Script will work with colored images but to get predictable result it's preferred to place 1bit or monochrome images inside the `images/` directory. Images also follow snake_case. 

After adding image for example `images/example_image.png` it should be available as `EXAMPLE_IMAGE_IMAGE` along with `EXAMPLE_IMAGE_IMAGE_WIDTH` and `EXAMPLE_IMAGE_IMAGE_HEIGHT` defines. (Maybe don't use word image in the file name please).