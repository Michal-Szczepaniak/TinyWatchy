# Adding options/features

Features in TinyWatchy are called options. They're instantiated in the `TinyWatchy.cpp` so you can Dependency Inject anything you need.

## Base AbstractOption interface

Every watchface **has to** extend `AbstractOption` interface.

`AbstractOption` consts of following methods:

- `virtual std::string getTitle() = 0;`: Used to get title of the option. It's supposed to be static. Length will be standardised at later date.
- `virtual std::string getDescription(const StackPage& stackPage) = 0;`: Used to get description aka actual information/menu from your feature. This is basically all the output you have. Be concise, maximum 20 characters. StackPage contains property selected which you can use.
- `virtual void onNextButtonPressed() = 0;`: Handler for next button press. It won't be called unless your option is selected (if you allow such a thing). You can use it to do whatever you like. Can be submenu, doesn't have to be.
- `virtual void onPrevButtonPressed() = 0;`: As above just button for previous option.
- `virtual bool onSelectButtonPressed(const StackPage& stackPage) = 0;`: Handler for select button. If you return true, `selected` variable that is provided by `StackPage` will be set to true and `next`/`prev`/`back` handlers will activate. You can return false and just execute simple action like it's done in `NTPOption`
- `virtual void onBackButtonPressed() = 0;`: Back button handler. It's called only if your option is selected. The user will go back whether you like it or not but you can use it to return description to normal state or cancel pending operation.

What you do inside them is entirely up to you with limitations, don't draw on the screen, don't interrupt too much.

Simplest example option is `MenuOption.h` which simply displays default option and does nothing. `NTPOption` is bit more complicated executing simple action on select. 

## Enabling option

To add your option to the rest of the firmware you need to add it to `Menu`. 

In `Menu.h` add your class as property like 
```
    WatchfaceOption _watchfaceOption;
    DriftOption _driftOption;
    UiOption _uiOption;
```
Then add it somewhere in `_pages`. `_pages` represents the menu structure. You can also add submenu.

If you need some other classes dependency injected, do it in `Menu.cpp` inside `Menu` initializer list.
