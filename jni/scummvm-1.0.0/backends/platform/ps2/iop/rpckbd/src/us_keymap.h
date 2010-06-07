/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
#
# This file is dual licensed, with permission by the original author
# TyRaNiD, under both the Academic Free License version 2.0 and the GNU
# General Public License version 2 or later.
# 
# This means you can choose whether to use this code under the terms of
# the Academic Free License version 2.0, or under the terms of the GNU
# General Public License version 2 or later. As long as you comply to the
# terms of at least one of these, you are allowed to use the code as
# permitted by the respective license.
#
# $Id: us_keymap.h 45052 2009-10-14 00:36:54Z sunmax $
# USB Keyboard Driver for PS2
*/

#ifndef __US_KEYMAP_H__
#define __US_KEYMAP_H__

/* Default US keymap */

u8 us_keymap[PS2KBD_KEYMAP_SIZE] =
  {
    0,
    0,
    0,
    0,
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    10, /* line feed */
    0, /* Esc */
    0x7,/* BS */
    0x9, /* TAB */
    0x20,
    '-',
    '=',
    '[',
    ']',
    '\\',
    '#',
    ';',
    '\'',
    '`',
    ',',
    '.',
    '/',
    0, /* CL */
    0, // F1
    0, // F2
    0, // F3
    0, // F4
    0, // F5
    0, // F6
    0, // F7
    0, // F8
    0, // F9
    0, // F10
    0, // F11
    0, // F12
    0, // PrintScr
    0, // Scroll Lock
    0, // Pause
    0, // Insert
    0, // Home
    0, // Pg Up
    0, // Delete
    0, // End
    0, // Pg Down
    0, // Right
    0, // Left
    0, // Down
    0, // Up
    0, // Numlock
    '/', // Keypad
    '*',
    '-',
    '+',
    10,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '.',
    '\\',
    0,
    0,
    '=',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };

u8 us_shiftkeymap[PS2KBD_KEYMAP_SIZE] =
  {
    0,
    0,
    0,
    0,
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    10, /* line feed */
    0, /* Esc */
    0x7,/* BS */
    0x9, /* TAB */
    0x20,
    '_',
    '+',
    '{',
    '}',
    '|',
    '~',
    ':',
    '"',
    '~',
    '<',
    '>',
    '?',
    0, /* CL */
    0, // F1
    0, // F2
    0, // F3
    0, // F4
    0, // F5
    0, // F6
    0, // F7
    0, // F8
    0, // F9
    0, // F10
    0, // F11
    0, // F12
    0, // PrintScr
    0, // Scroll Lock
    0, // Pause
    0, // Insert
    0, // Home
    0, // Pg Up
    0, // Delete
    0, // End
    0, // Pg Down
    0, // Right
    0, // Left
    0, // Down
    0, // Up
    0, // Numlock
    '/', // Keypad
    '*',
    '-',
    '+',
    10,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '.',
    '\\',
    0,
    0,
    '=',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };

u8 us_keycap[PS2KBD_KEYMAP_SIZE] =
  {
    0,
    0,
    0,
    0,
    1, //a
    1, //b
    1, //c
    1, //d
    1, //e
    1, //f
    1,//g
    1,//h
    1,//i
    1,//j
    1,//k
    1,//l
    1,//m
    1,//n
    1,//o
    1,//p
    1,//q
    1,//r
    1,//s
    1,//t
    1,//u
    1,//v
    1,//w
    1,//x
    1,//y
    1,//z
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* line feed */
    0, /* Esc */
    0,/* BS */
    0, /* TAB */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* CL */
    0, // F1
    0, // F2
    0, // F3
    0, // F4
    0, // F5
    0, // F6
    0, // F7
    0, // F8
    0, // F9
    0, // F10
    0, // F11
    0, // F12
    0, // PrintScr
    0, // Scroll Lock
    0, // Pause
    0, // Insert
    0, // Home
    0, // Pg Up
    0, // Delete
    0, // End
    0, // Pg Down
    0, // Right
    0, // Left
    0, // Down
    0, // Up
    0, // Numlock
    0, // Keypad
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };

u8 us_special_keys[PS2KBD_KEYMAP_SIZE] = {

    0,
    0,
    0,
    0,
    0, //a
    0, //b
    0, //c
    0, //d
    0, //e
    0, //f
    0,//g
    0,//h
    0,//i
    0,//j
    0,//k
    0,//l
    0,//m
    0,//n
    0,//o
    0,//p
    0,//q
    0,//r
    0,//s
    0,//t
    0,//u
    0,//v
    0,//w
    0,//x
    0,//y
    0,//z
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* line feed */
    0x1B, /* Esc */
    0,/* BS */
    0, /* TAB */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* CL */
    1, // F1
    2, // F2
    3, // F3
    4, // F4
    5, // F5
    6, // F6
    7, // F7
    8, // F8
    9, // F9
    10, // F10
    11, // F11
    12, // F12
    32, // PrintScr
    33, // Scroll Lock
    34, // Pause
    35, // Insert
    36, // Home
    37, // Pg Up
    38, // Delete
    39, // End
    40, // Pg Down
    41, // Right
    42, // Left
    43, // Down
    44, // Up
    0, // Numlock
    0, // Keypad /
    0, // Keypad *
    0, // Keypad -
    0, // Keypad +
    0, // Keypad Enter
    39, // Keypad 1/End
    43, // Keypad 2/Down
    40, // Keypad 3/PageDn
    42, // Keypad 4/Left
    0, // Keypad 5
    41, // Keypad 6/Right
    36, // Keypad 7/Home
    44, // Keypad 8/Up
    37, // Keypad 9/PageUp
    35, // Keypad 0/Insert
    38, // Keypad ./Delete
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };

u8 us_control_map[PS2KBD_KEYMAP_SIZE] = {

    0,
    0,
    0,
    0,
    1, //a
    2, //b
    3, //c
    4, //d
    5, //e
    6, //f
    7,//g
    8,//h
    9,//i
    10,//j
    11,//k
    12,//l
    13,//m
    14,//n
    15,//o
    16,//p
    17,//q
    18,//r
    19,//s
    20,//t
    21,//u
    22,//v
    23,//w
    24,//x
    25,//y
    26,//z
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* line feed */
    0, /* Esc */
    0,/* BS */
    0, /* TAB */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* CL */
    0, // F1
    0, // F2
    0, // F3
    0, // F4
    0, // F5
    0, // F6
    0, // F7
    0, // F8
    0, // F9
    0, // F10
    0, // F11
    0, // F12
    0, // PrintScr
    0, // Scroll Lock
    0, // Pause
    0, // Insert
    0, // Home
    0, // Pg Up
    0, // Delete
    0, // End
    0, // Pg Down
    0, // Right
    0, // Left
    0, // Down
    0, // Up
    0, // Numlock
    0, // Keypad
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

u8 us_alt_map[PS2KBD_KEYMAP_SIZE] = {

    0,
    0,
    0,
    0,
    128, //a
    129, //b
    130, //c
    131, //d
    132, //e
    133, //f
    134,//g
    135,//h
    136,//i
    137,//j
    138,//k
    139,//l
    140,//m
    141,//n
    142,//o
    143,//p
    144,//q
    145,//r
    146,//s
    147,//t
    148,//u
    149,//v
    150,//w
    151,//x
    152,//y
    154,//z
    155,
    156,
    157,
    158,
    159,
    160,
    161,
    162,
    163,
    164,
    165, /* line feed */
    0, /* Esc */
    0,/* BS */
    0, /* TAB */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, /* CL */
    0, // F1
    0, // F2
    0, // F3
    0, // F4
    0, // F5
    0, // F6
    0, // F7
    0, // F8
    0, // F9
    0, // F10
    0, // F11
    0, // F12
    0, // PrintScr
    0, // Scroll Lock
    0, // Pause
    0, // Insert
    0, // Home
    0, // Pg Up
    0, // Delete
    0, // End
    0, // Pg Down
    0, // Right
    0, // Left
    0, // Down
    0, // Up
    0, // Numlock
    0, // Keypad
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };

#endif
