/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* string        START                                  /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 1.0                                         \ \ \_/__ \_\ \_  __  */
/* Date: April 15, 2024                                  \ \_\/\_\/\____\/\_\ */
/*                                                        \/_/\/_/\/____/\/_/ */
/*===----------------------------------------------------------------------===*/

/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2024 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#include <stdbool.h>

#include "array.h"

#define var __auto_type

static char const WKQ_UTF8_LEN[] = {
/* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 3, 4
};
#define WKQUTF8LEN(c) WKQ_UTF8_LEN[((c) & 0xFF) >> 4]

struct String {
  /* A buffer of a string’s contents as a collection of UTF-8 code units. */
  struct Array utf8;
  /* The number of characters in a string. */
  int count;
  /* A Boolean value indicating whether a string has no characters. */
  bool is_empty;
};

/* MARK: - Creating and Destroying a String */

/* Creates an empty string. */
void string_init(struct String* str) {
  (*str).count = 0;
  (*str).is_empty = true;
  array_init(&(*str).utf8, sizeof(char));
}

/*
 * Creates a string from the null-terminated character sequence (C-string) 
 * pointed by s.
 */
void string_init_c_string(struct String* str, const char* s) {
  string_init(str);
  
  var _s = s;
  while (*_s != '\0') {
    var code_len = WKQUTF8LEN(*_s);
    (*str).count += 1;
    while (code_len--) {
      array_append(&(*str).utf8, (void*)_s);
      _s += 1;
    }
  }
  if ((*str).count > 0) {
    (*str).is_empty = false;
  }
}

/* Destroys a string. */
void string_deinit(struct String* str) {
  array_deinit(&(*str).utf8);
  (*str).count = 0;
  (*str).is_empty = true;
}

/* MARK: - Appending Strings and Characters */

/* 
 * Appends the given string to this string.
 * Never do string_append(&str, &str);
 */
void string_append(struct String* str, struct String* other) {
  (*str).count += (*other).count;
  (*str).is_empty = (*str).is_empty && (*other).is_empty;
  array_combine(&(*str).utf8, &(*other).utf8);
}

/* 
 * Appends the null-terminated character sequence (C-string) pointed by other
 * to this string.
 */
void string_append_c_string(struct String* str, const char* other) {
  struct String s;
  string_init_c_string(&s, other);
  
  string_append(str, &s);
  
  string_deinit(&s);
}

/*===----------------------------------------------------------------------===*/
/*             ___                            ___                             */
/*           /'___\                          /\_ \    __                      */
/*          /\ \__/   __      ___      __    \//\ \  /\_\    ___      __      */
/*          \ \ ,__\/'__`\  /' _ `\  /'_ `\    \ \ \ \/\ \ /' _ `\  /'_ `\    */
/*           \ \ \_/\ \L\.\_/\ \/\ \/\ \L\ \    \_\ \_\ \ \/\ \/\ \/\ \L\ \   */
/*            \ \_\\ \__/.\_\ \_\ \_\ \____ \   /\____\\ \_\ \_\ \_\ \____ \  */
/*             \/_/ \/__/\/_/\/_/\/_/\/___L\ \  \/____/ \/_/\/_/\/_/\/___L\ \ */
/* string        END                   /\____/                        /\____/ */
/*                                     \_/__/                         \_/__/  */
/*===----------------------------------------------------------------------===*/
