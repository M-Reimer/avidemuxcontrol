Avidemux Control
================

Introduction
------------

Avidemux Control is a Arduino project which allows to reprogram an iWit Volume Control device to be used as Jog/Shuttle device for Avidemux.

It does this by sending hotkeys based on your input. This means that absolutely nothing has to be changed on Avidemux and that it should work on every operating system supported by Avidemux as long as the hotkeys are the same.

For usage with Avidemux, you should use the rectangular device as the round one does not have noticeable notches. Having the notches really helps with moving through the frames.

![](https://raw.githubusercontent.com/wiki/M-Reimer/avidemuxcontrol/images/iwit.jpg)

Installation
------------

You need the following libraries from the Arduino library manager:

- EncoderStepCounter by Manuel Reimer
- IwitVolumeKnob by Manuel Reimer
- HID-Project by NicoHood

You'll also need some ISP programmer device to flash HoodLoader2. The IwitVolumeKnob GitHub page has [some instructions on how to do this](https://github.com/M-Reimer/IwitVolumeKnob#burning-the-bootloader).

After burning the bootloader, you can just upload Avidemux Control via the Arduino IDE.

Usage
-----

Shuttle feature:

- Short press the wheel to toggle between all frames and intra frames only shuttle mode. The LED is lit in "all frames mode" mode.
- Rotate the wheel to run through the frames. With LED lit all frames are selected. With LED not lit only intra frames are selected.

Jog mode:

- Press and hold the wheel. Then move one "notch" to the left/right for "slow seeking".
- Press and hold the wheel. Then move two "notches" to the left/right for "fast seeking".
