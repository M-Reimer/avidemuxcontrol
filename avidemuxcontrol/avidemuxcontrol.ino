/*
    Avidemux Control - USB rotary encoder controller
    Copyright (C) 2019 Manuel Reimer <manuel.reimer@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "IwitVolumeKnob.h"
#include "HID-Project.h"


/****************************/
/* Shuttle feature handling */
/****************************/

unsigned long lasttime;
void HandleShuttle(char aPos) {
  unsigned long delay = 0;

  if (aPos == 0)
    lasttime = millis();
  else if (abs(aPos) == 1)
    delay = 80;
  else
    delay = 300;

  if (millis() - lasttime >= delay) {
    lasttime = millis();
    if (aPos == 1)
      Keyboard.write(KEY_UP_ARROW);
    else if (aPos == 2) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.write(KEY_UP_ARROW);
      Keyboard.release(KEY_LEFT_CTRL);
    }
    else if (aPos == -1)
      Keyboard.write(KEY_DOWN_ARROW);
    else if (aPos == -2) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.write(KEY_DOWN_ARROW);
      Keyboard.release(KEY_LEFT_CTRL);
    }
  }
  Serial.print("Shuttle: ");
  Serial.println((int)aPos);

  IwitKnob.setLed(abs(aPos) == 2);
}

/************************/
/* Jog feature handling */
/************************/

const char JOG_LEFT = 1;
const char JOG_RIGHT = 2;
const char JOG_NEUTRAL = 3;
void HandleJog(char aDir, bool aSingleFrame) {
  if (aDir == JOG_LEFT)
    Keyboard.write(aSingleFrame ? KEY_LEFT_ARROW : KEY_DOWN_ARROW);
  else if (aDir == JOG_RIGHT)
    Keyboard.write(aSingleFrame ? KEY_RIGHT_ARROW : KEY_UP_ARROW);

  IwitKnob.setLed(aSingleFrame);
}

/****************/
/* Backend code */
/****************/

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  IwitKnob.begin();
}

bool lastbtn = false;
bool singleframe = false;
unsigned long lastsingleframetime;
bool wasshuttle = false;
signed char lastpos = 0;
void loop() {
  if (USBDevice.isSuspended()) {
    lastbtn = false;
    singleframe = false;
    wasshuttle = false;
    lastpos = 0;
    IwitKnob.reset();
    IwitKnob.setLed(false);
    return;
  }

  // Handle button
  bool btn = IwitKnob.getButton();
  if (btn != lastbtn) {
    // Button got pressed
    if (btn)
      IwitKnob.reset(); // Reset knob state
    // Button released
    else {
      if (wasshuttle) {
        IwitKnob.reset();
        wasshuttle = false;
      }
      else
        singleframe = !singleframe;

      if (singleframe)
        lastsingleframetime = millis();
    }
  }
  lastbtn = btn;

  signed char pos = IwitKnob.getPosition();
  if (pos != lastpos) {
    Serial.print("Pos: ");
    Serial.println(pos);
  }
  lastpos = pos;
  // Knob down --> Shuttle
  if (btn) {
    if (pos > 2) {
      IwitKnob.setPosition(2);
      pos = 2;
    }
    if (pos < -2) {
      IwitKnob.setPosition(-2);
      pos = -2;
    }

    if (pos != 0) {
      wasshuttle = true;
      singleframe = false;
    }

    HandleShuttle(pos);
  }
  // Knob not pressed --> Jog
  else {
    if (pos != 0) {
      lastsingleframetime = millis();
      HandleJog((pos < 0) ? JOG_LEFT : JOG_RIGHT, singleframe);
      IwitKnob.reset();
    }
    else
      HandleJog(JOG_NEUTRAL, singleframe);

    //
    if (singleframe && millis() - lastsingleframetime >= 10000)
      singleframe = false;
  }
}
