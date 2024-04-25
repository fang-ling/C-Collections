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

#include "string.h"

static char const WKQ_UTF8_LEN[] = {
/* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 3, 4
};
#define WKQUTF8LEN(c) WKQ_UTF8_LEN[((c) & 0xFF) >> 4]

struct String* _string_init(UInt32* _utf8, Int32* _utf8_length, Int64 count) {
  struct String* string;
  if ((string = malloc(sizeof(struct String))) == NULL) {
    return NULL;
  }
  
  string->_utf8_capacity = count;
  string->count = count;
  
  if (string->_utf8_capacity > 0) {
    string->is_empty = false;
  }
  
  string->_utf8 = malloc(sizeof(UInt32) * string->_utf8_capacity);
  string->_utf8_length = malloc(sizeof(Int32) * string->_utf8_capacity);
  
  memcpy(string->_utf8, _utf8, sizeof(UInt32) * count);
  memcpy(string->_utf8_length, _utf8_length, sizeof(Int32) * count);
  
  return string;
}

/* MARK: - Creating and Destroying a String */

struct String* string_init(const char* s) {
  struct String* string;
  if ((string = malloc(sizeof(struct String))) == NULL) {
    return NULL;
  }
  
  /* Calculate capacity & is_empty */
  string->_utf8_capacity = 0;
  var _s = s;
  while (*_s != '\0') {
    string->_utf8_capacity += 1;
    _s += WKQUTF8LEN(*_s);
  }
  if (string->_utf8_capacity > 0) {
    string->is_empty = false;
  }
  
  string->_utf8 = malloc(sizeof(UInt32) * string->_utf8_capacity);
  string->_utf8_length = malloc(sizeof(UInt32) * string->_utf8_capacity);
  
  _s = s;
  string->count = 0;
  while (*_s != '\0') {
    var len = WKQUTF8LEN(*_s);
    string->_utf8[string->count] = 0;
    string->_utf8_length[string->count] = len;
    var i = 0;
    for (i = 0; i < len; i += 1) {
      string->_utf8[string->count] |= (((UInt32)(UInt8)*_s) << (i * 8));
      _s += 1;
    }
    string->count += 1;
  }

  return string;
}

void string_deinit(struct String* string) {
  free(string->_utf8);
  free(string->_utf8_length);
  free(string);
}

/* MARK: - Appending Strings and Characters */

/* 
 * Appends the given string to this string.
 * Never do string_append(&str, &str);
 */
/*void string_append(struct String* str, struct String* other) {
  (*str).count += (*other).count;
  (*str).is_empty = (*str).is_empty && (*other).is_empty;
  array_combine(&(*str).utf8, &(*other).utf8);
  
}*/

/* 
 * Appends the null-terminated character sequence (C-string) pointed by other
 * to this string.
 */
/*void string_append_c_string(struct String* str, const char* other) {
  struct String s;
  string_init_c_string(&s, other);
  
  string_append(str, &s);
  
  string_deinit(&s);
}*/

/* MARK: - Getting Substrings */

/* 
 * Returns a new contiguous substring of the string.
 *
 * It's caller's responsibility to free the returned value.
 *
 * Returns NULL if the start is negative, or end is larger than the length of
 * this string object, or start is larger than end.
 */
struct String* string_substring(
  struct String* string,
  Int64 start,
  Int64 end
) {
  if (start < 0 || end > string->count || start > end) {
    return NULL;
  }
  
  var _utf8 = string->_utf8 + start;
  var _utf8_length = string->_utf8_length + start;
  return _string_init(_utf8, _utf8_length, end - start);
}

/* MARK: - Splitting a String */

/*
 * Returns an array containing substrings from the string that have been divided
 * by the given separator.
 * It's caller's responsibility to free the strings stored in the result.
 */
/* Example:
 *                    1                 2    3             4
 * Assuming: str = "#zyy#jasdjq2n3oasd#zyy##zyy#adn972929#zyy#"
 *           separator = "#zyy#"
 *      will return 5 substrings.
 */
void string_components(
  struct String* string,
  struct String* separator,
  struct Array* result
) {
  var last_index = 0ll;
  
  if (separator->count != 0) {
    var i = 0;
    var length = string->count - separator->count + 1;
    for (i = 0; i < length; i += 1) {
      var j = 0;
      while (
             j < separator->count &&
             string->_utf8[i + j] == separator->_utf8[j]
             ) {
               j += 1;
             }
      if (j == separator->count) { /* Find a match at i */
        var substring = string_substring(string, last_index, i);
        array_append(result, &substring);
        last_index += separator->count + substring->count;
      }
    }
  }
  /* Add the remaining substring */
  var substring = string_substring(string, last_index, string->count);
  array_append(result, &substring);
}

/* MARK: - Getting C Strings */

void string_c_string(struct String* string, char* result) {
  var i = 0;
  for (i = 0; i < string->count; i += 1) {
    var j = 0;
    for (j = 0; j < string->_utf8_length[i]; j += 1) {
      *result = (UInt8)(string->_utf8[i] >> (j * 8));
      result += 1;
    }
  }
  *result = '\0';
}

/* MARK: - Comparing Strings */

/* 
 * Returns an integer greater than, equal to, or less than 0, according as the
 * string lhs is greater than, equal to, or less than the string rhs.
 */
Int32 string_compare_ascii(const void* lhs, const void* rhs) {
  var a = (struct String*)lhs;
  var b = (struct String*)rhs;
  
  var lim = a->count < b->count ? a->count : b->count;
  
  /* Find the first mismatch */
  var i = 0;
  for (i = 0; i < lim; i += 1) {
    if (a->_utf8[i] != b->_utf8[i]) {
      break;
    }
  }
  if (i == lim) { /* no mismatch founded */
    return (Int32)(a->count - b->count);
  } else {
    return ((Int32)a->_utf8[i]) - ((Int32)b->_utf8[i]);
  }
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
