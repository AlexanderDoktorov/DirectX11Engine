#pragma once
#include "Device.h"

enum Button
{
    BUTTON_A = 0x41,
    BUTTON_B = 0x42,
    BUTTON_C = 0x43,
    BUTTON_D = 0x44,
    BUTTON_E = 0x45,
    BUTTON_F = 0x46,
    BUTTON_G = 0x47,
    BUTTON_H = 0x48,
    BUTTON_I = 0x49,
    BUTTON_J = 0x4A,
    BUTTON_K = 0x4B,
    BUTTON_L = 0x4C,
    BUTTON_M = 0x4D,
    BUTTON_N = 0x4E,
    BUTTON_O = 0x4F,
    BUTTON_P = 0x50,
    BUTTON_Q = 0x51,
    BUTTON_R = 0x52,
    BUTTON_S = 0x53,
    BUTTON_T = 0x54,
    BUTTON_U = 0x55,
    BUTTON_V = 0x56,
    BUTTON_W = 0x57,
    BUTTON_X = 0x58,
    BUTTON_Y = 0x59,
    BUTTON_Z = 0x5A
};

class Keyboard : public Device
{
public:

    Keyboard() : Device(HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_KEYBOARD)
    {

    }

};