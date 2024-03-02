# Adding watchface

Below are instructions on how to add your own watchface.

## Base AbstractFace interface

Every watchface **has to** extend `AbstractFace` interface.

`AbstractFace` consts of following methods:

- `constructor`: It has dependency injection on the display driver to be able to draw on the screen.
- `draw(ScreenInfo*)`: Method called by the `Screen` to draw your watchface. It's argument is `ScreenInfo` which consists of informations to be displayed. **Every variable has to be displayed, doesn't matter how but every one has to be displayed.** For example you can display hour as a sunset or battery as a bar at the top it doesn't matter as long as they're displayed. That's important for integrity of entire firmware. Displaying menu can be skipped for aesthetic purposes if `onMainOption` is true.
- `getName()`: This method provides name of the watchface to the `Menu`.

## Developing

Easiest way to start is to copy the `DefaultFace` and change the name and this guide will be describing how `DefaultFace` was made.

First things first, for simplicity, I've added five methods to further split the rendering. It's not enforced but it's useful.

You can add your own fonts and images by following instructions in [Resources](Resources.md) 

>    void drawTime(const DateTime &time);
> 
>    void drawBattery(const uint8_t &battery);
> 
>    void drawSteps(const unsigned int &steps);
> 
>    void drawMenuTitle(const std::string &title);
> 
>    void drawMenuDescription(const std::string &description);

Inside `draw()` method, first, I draw background for my watchface. To draw added images from resources, we use `_display->drawBitmap();` method.

### `drawBitmap()` method

> _display->drawBitmap(
> 
> position x on the screen,
> 
> position y on the screen,
> 
> resources::EXAMPLE_IMAGE,
> 
> EXAMPLE_IMAGE_WIDTH,
> 
> EXAMPLE_IMAGE_HEIGHT,
> 
> GxEPD_BLACK or GxEPD_WHITE // depends if you're doing dark or light mode 
> 
> );

### `ScreenInfo` struct

`ScreenInfo` consists of following fields:

> struct ScreenInfo {
> 
> DateTime time{}; // field time contains current datetime from RTC
> 
>uint32_t steps = 0; // Steps counter
> 
>uint8_t battery = 0; // Battery, 0-100
> 
>std::string title; // Menu title
> 
>std::string description; // Menu description
> 
>bool humanInSleep = false; // If human (that's hopefully you) is in sleep, optional, can be used to tweak watchface
> 
>}

### `drawTime()` method

Inside `drawTime()` we… well draw time. How you do it it's up to you, you can draw different bitmaps depending on time or different texts or just type number. 

As with any other text based draws we first set font:

> _display->setFont(&resources::DIGITAL_DISPLAY_REGULAR_50);
 
It's suggested to set font even if it didn't change from previous method for when we make mistakes.

> _display->setTextColor(GxEPD_BLACK);
 
Sets text color. Text color will depend if you're drawing with white on black background of black on white background. If one doesn't work, try another.

> _display->setCursor(5, 50 + 18);
 
Sets coordinates of where we're typing. Parameters are x and y coordinate. I don't know math I just do trial and error same with font size.

> _display->print();
 
This method is used to write text on the screen. Rest should be self explanatory.

### `drawBattery()` method

Again, how you draw it is up to you, I'm using icon from original Watchy firmware. All you have is the int variable giving you 0-100.

As most of the methods were explained already only one left is:

> _display->fillRect(X, Y, WIDTH, HEIGHT, GxEPD_BLACK/GxEPD_WHITE)
 
This is used to draw filled rectangle. 

### `drawSteps()` method

I am using steps icon from original Watchy firmware as well as printing number.

### `drawMenuTitle()`, `drawMenuDescription` methods

This methods draw menu title and description. Make sure you give it enough space so that it won't interfere with your watchface. Please don't cut it, I will standardise maximum length on later date. They are used for UX, interfacing with features.

> _display->getTextBounds()
 
This method gets width of to be drawn text so that I can center it.

## Enabling your watchface

To add your watchface you need to:

- Add it in `Screen.cpp` by adding `_faces.emplace_back(std::make_unique<YourFace>(display));` in the constructor. Add it at the end of the list.

You should be able to head to Settings option on the watch and select your watchface in the submenu.
