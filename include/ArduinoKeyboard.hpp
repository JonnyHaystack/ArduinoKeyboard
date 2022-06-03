/*
  Keyboard.h

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef KEYBOARD_h
#define KEYBOARD_h

#include "HID.h"

#ifndef _USING_HID

#warning "Using legacy HID core (non pluggable)"

#else

#include "keycodes.h"

// Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
} KeyReport;

class ArduinoKeyboard {
  public:
    ArduinoKeyboard();
    void press(uint8_t keycode);
    void release(uint8_t keycode);
    void setPressed(uint8_t keycode, bool pressed);
    void releaseAll();
    void sendReport();

  private:
    KeyReport _report;
};

extern ArduinoKeyboard _keyboard;

#endif
#endif
