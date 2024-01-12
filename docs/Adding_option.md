# Adding options/features

Features in TinyWatchy are called options. They're instantiated in the `TinyWatchy.cpp` so you can Dependency Inject anything you need.

## Base AbstractOption interface

Every watchface **has to** extend `AbstractOption` interface.

`AbstractOption` consts of following methods:

- `virtual std::string getTitle() = 0;`: Used to get title of the option. It's supposed to be static. Length will be standardised at later date.
- `virtual std::string getDescription() = 0;`: Used to get description aka actual information/menu from your feature. This is basically all the output you have. Be concise, watchfaces don't have that much space. Length will be standardised at later date.
- `virtual void onNextButtonPressed() = 0;`: Handler for next button press. It won't be called unless your option is selected (if you allow such a thing). You can use it to do whatever you like. Can be submenu, doesn't have to be.
- `virtual void onPrevButtonPressed() = 0;`: As above just button for prevoius option.
- `virtual bool onSelectButtonPressed() = 0;`: Handler for select button. If you return true, `_level` variable that is provided by `AbstractOption` will increase and `next`/`prev`/`back` handlers will activate. You can return false and just execute simple action like it's done in `NTPOption`
- `virtual void onBackButtonPressed() = 0;`: Back button handler. It's called only if `_level` is higher than 0 so, only if your option is selected. The level *will decrease* if you like it or not but you can use it to control your submenu or return description to normal state or cancel pending operation.

What you do inside them is entirely up to you with limitations, don't draw on the screen, don't interrupt too much, don't modify `_level` even though you can, leave it to `Menu`.

Simplest example option is `MenuOption.h` which simply displays default option and does nothing. `NTPOption` is bit more complicated executing simple action on select. `AboutOption` is most complicated, handling many features and having submenu.

## Enabling option

To add your option to the rest of the firmware you need to add it to `TinyWatchy.cpp` inside the constructor. Add your option by adding line

> _menu->appendOption(new YourOption);
 
In between Menu and About options. 
