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

/*-
 * Copyright (c) 1992, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
static Int32 _wkq_strtoll(
  const char* __restrict nptr,
  char** __restrict endptr,
  int base,
  Int64* result
) {
  const char *s;
  UInt64 acc;
  char c;
  UInt64 cutoff;
  Int32 neg, any, cutlim;

  /*
   * Skip white space and pick up leading +/- sign if any.
   * If base is 0, allow 0x for hex and 0 for octal, else
   * assume decimal; if base is already 16, allow 0x.
   */
  s = nptr;
  do {
    c = *s++;
  } while (isspace((unsigned char)c));
  if (c == '-') {
    neg = 1;
    c = *s++;
  } else {
    neg = 0;
    if (c == '+')
      c = *s++;
  }
  if ((base == 0 || base == 16) &&
      c == '0' && (*s == 'x' || *s == 'X') &&
      ((s[1] >= '0' && s[1] <= '9') ||
      (s[1] >= 'A' && s[1] <= 'F') ||
      (s[1] >= 'a' && s[1] <= 'f'))) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0)
    base = c == '0' ? 8 : 10;
  acc = any = 0;
  if (base < 2 || base > 36)
    goto noconv;

  /*
   * Compute the cutoff value between legal numbers and illegal
   * numbers.  That is the largest legal value, divided by the
   * base.  An input number that is greater than this value, if
   * followed by a legal input character, is too big.  One that
   * is equal to this value may be valid or not; the limit
   * between valid and invalid numbers is then based on the last
   * digit.  For instance, if the range for quads is
   * [-9223372036854775808..9223372036854775807] and the input base
   * is 10, cutoff will be set to 922337203685477580 and cutlim to
   * either 7 (neg==0) or 8 (neg==1), meaning that if we have
   * accumulated a value > 922337203685477580, or equal but the
   * next digit is > 7 (or 8), the number is too big, and we will
   * return a range error.
   *
   * Set 'any' if any `digits' consumed; make it negative to indicate
   * overflow.
   */
  cutoff = neg ? (UInt64) - (WKQ_INT64_MIN + WKQ_INT64_MAX) + WKQ_INT64_MAX
      : WKQ_INT64_MAX;
  cutlim = cutoff % base;
  cutoff /= base;
  for ( ; ; c = *s++) {
    if (c >= '0' && c <= '9')
      c -= '0';
    else if (c >= 'A' && c <= 'Z')
      c -= 'A' - 10;
    else if (c >= 'a' && c <= 'z')
      c -= 'a' - 10;
    else
      break;
    if (c >= base)
      break;
    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
      any = -1;
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0) {
    acc = neg ? WKQ_INT64_MIN : WKQ_INT64_MAX;
    return 1; /*errno = ERANGE;*/
  } else if (!any) {
noconv:
    return -1; /*errno = EINVAL;*/
  } else if (neg)
    acc = -acc;
  if (endptr != NULL)
    *endptr = (char *)(any ? s - 1 : nptr);
  *result = acc;
  return 0;
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

/* MARK: - Converting Strings */

Int32 string_to_int64(struct String* string, Int32 base, Int64* result) {
  /* Optimization needed */
  var buf = malloc((string->count + 1) * sizeof(char));
  
  string_c_string(string, buf);
  var ret = _wkq_strtoll(buf, NULL, base, result);
  /*
   * 0: OK
   * 1: The given string was out of range; the value converted has been clamped.
   * 2: The value of base is not supported or no conversion could be performed
   *    (the last feature is not portable across all platforms).
   */
  free(buf);
  
  return ret;
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
