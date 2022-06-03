/*
  Keyboard.cpp

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

#include "ArduinoKeyboard.hpp"

#ifdef _USING_HID

#define MODIFIER_MASK(mod_kc) (1 << (mod_kc & 0x0F))

static const uint8_t _hidReportDescriptor[] PROGMEM = {

    // Keyboard
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x85, 0x02, // REPORT_ID (2)
    0x05, 0x07, // USAGE_PAGE (Keyboard)

    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)

    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs)

    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x73, // LOGICAL_MAXIMUM (115)
    0x05, 0x07, // USAGE_PAGE (Keyboard)

    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)
    0xc0, // END_COLLECTION
};

ArduinoKeyboard::ArduinoKeyboard() {
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
    releaseAll();
}

void ArduinoKeyboard::press(uint8_t keycode) {
    // If keycode >= E0 then it's a modifier key.
    if (keycode >= 0xE0) {
        // Create bitmask from the modifier keycode to set the corresponding bit in the modifier
        // byte.
        uint8_t bitmask = MODIFIER_MASK(keycode);
        _report.modifiers |= bitmask;
        return;
    }

    // Check if the key is already pressed so that we don't send the same keycode multiple times in
    // the same report.
    for (int i = 0; i < 6; i++) {
        if (_report.keys[i] == keycode) {
            return;
        }
    }

    // Place this keycode in the report in place of the first empty keycode.
    for (int i = 0; i < 6; i++) {
        if (_report.keys[i] == HID_KEY_NONE) {
            _report.keys[i] = keycode;
            return;
        }
    }
}

void ArduinoKeyboard::release(uint8_t keycode) {
    // If keycode >= E0 then it's a modifier key.
    if (keycode >= 0xE0) {
        // Create bitmask from the modifier keycode to unset the corresponding bit in the modifier
        // byte.
        uint8_t bitmask = ~MODIFIER_MASK(keycode);
        _report.modifiers &= bitmask;
        return;
    }

    // Loop through keycodes in report. If we find the specified key code, we clear it.
    for (int i = 0; i < 6; i++) {
        if (_report.keys[i] == keycode) {
            _report.keys[i] = HID_KEY_NONE;
        }
    }
}

void ArduinoKeyboard::setPressed(uint8_t keycode, bool pressed) {
    if (pressed) {
        press(keycode);
    } else {
        release(keycode);
    }
}

void ArduinoKeyboard::releaseAll() {
    for (int i = 0; i < 6; i++) {
        _report.keys[i] = 0;
    }
    _report.modifiers = 0;
}

void ArduinoKeyboard::sendReport() {
    HID().SendReport(2, &_report, sizeof(KeyReport));
}

ArduinoKeyboard _keyboard;

#endif
