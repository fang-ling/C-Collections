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

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * The maximum length of a string and can be modified by the user according to
 * their specific requirements.
 */
#define STRING_MAX_LENGTH 0

#define var __auto_type

static char const WKQ_UTF8_LEN[] = {
/* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 3, 4
};
#define WKQUTF8LEN(c) WKQ_UTF8_LEN[((c) & 0xFF) >> 4]

struct String {
  /* A buffer of a string’s contents as a collection of UTF-8 code units. */
  char utf8[STRING_MAX_LENGTH];
  int utf8_count;
  /* The number of characters in a string. */
  int count;
  /* A Boolean value indicating whether a string has no characters. */
  bool is_empty;
};

/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2001 Mike Barcroft <mike@FreeBSD.org>
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
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

/*
 * Find the first occurrence of find in s, where the search is limited to the
 * first slen characters of s.
 */
static char* wkq_strnstr(const char* s, const char* find, int slen) {
  char c, sc;
  int len;
  
  if ((c = *find++) != '\0') {
    len = (int)strlen(find);
    do {
      do {
        if (slen-- < 1 || (sc = *s++) == '\0')
          return (NULL);
      } while (sc != c);
      if (len > slen)
        return (NULL);
    } while (strncmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}

/* MARK: - Creating and Destroying a String */

/* Creates an empty string. */
void string_init(struct String* str) {
  (*str).count = 0;
  (*str).is_empty = true;
  (*str).utf8_count = 0;
  /* array_init(&(*str).utf8, sizeof(char)); */
}

/*
 * Creates a string from the null-terminated character sequence (C-string) 
 * pointed by s.
 */
void string_init_c_string(struct String* str, const char* s) {
  string_init(str);
  
  var _s = s;
  var t = (*str).utf8;
  while (*_s != '\0') {
    var code_len = WKQUTF8LEN(*_s);
    (*str).count += 1;
    while (code_len--) {
      /* array_append(&(*str).utf8, (void*)_s); */
      *t = *_s;
      (*str).utf8_count += 1;
      _s += 1;
      t += 1;
    }
  }
  if ((*str).count > 0) {
    (*str).is_empty = false;
  }
}

/* Destroys a string. (You don't need to) */
/*void string_deinit(struct String* str) {
  array_deinit(&(*str).utf8);
  (*str).count = 0;
  (*str).is_empty = true;
}*/

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

/* MARK: - Splitting a String */

/*
 * Returns an array containing substrings from the string that have been divided
 * by the given separator.
 * It's caller's responsibility to free the strings stored in the result.
 */
/*
 *                    1                 2    3             4
 * Assuming: str = "#zyy#jasdjq2n3oasd#zyy##zyy#adn972929#zyy#"
 *           separator = "#zyy#"
 *      will return 5 substrings.
 */
/*void string_components_c_string(
  struct String* str,
  const char* separator,
  struct Array* result
) {
  var separator_len = (int)strlen(separator);
  
  char* tofree;
  var s = (char*)malloc(sizeof(char) * ((*str).utf8.count + 1 + separator_len));
  tofree = s;
  memcpy(s, (*str).utf8._storage, sizeof(char) * (*str).utf8.count);*/
  /* Append the dummy separator */
  /*memcpy(s + (*str).utf8.count, separator, separator_len);
  s[(*str).utf8.count + separator_len] = '\0';
  
  var substr_end = 0;
  var last_len = 0;
  var remaining_len = (*str).utf8.count + separator_len;
  
  char* i_p;
  while ((i_p = wkq_strnstr(s, separator, remaining_len)) != NULL) {
    remaining_len -= last_len;
    
    substr_end = (int)(i_p - s);
    s[substr_end] = '\0';
    last_len = (int)strlen(s);
    
    struct String new_str;
    string_init_c_string(&new_str, s);
    array_append(result, &new_str);*/
    /* Let the caller to call string_deinit(&new_str); */
    /*s += last_len;
    s += separator_len;
    remaining_len -= separator_len;
  }
  
  free(tofree);
}*/

/*void string_components(
  struct String* str,
  struct String* separator,
  struct Array* result
) {
  
}*/


/* MARK: - Getting C Strings */

/* Returns a representation of the string as a C string using utf-8 encoding. */
/*void string_c_string(struct String* str, char* result) {
  memcpy(result, (*str).utf8._storage, (*str).utf8.count);
  result[(*str).utf8.count] = '\0';
}*/

/* MARK: - Getting Substrings */

/* Accesses a contiguous substring of the string’s elements. */
void string_substring(
  struct String* str,
  int start,
  int end,
  struct String* substring
) {
  end += 1;
  
  var start_index = 0;
  while (start--) {
    start_index += WKQUTF8LEN((*str).utf8[start_index]);
  }
  var end_index = 0;
  while (end--) {
    end_index += WKQUTF8LEN((*str).utf8[start_index]);
  }
  
  var buf = (char*)malloc(sizeof(char) * (end_index - start_index + 1));
  memcpy(buf, (*str).utf8 + start_index, end_index - start_index - 1);
  buf[end_index - start_index] = '\0';
  string_init_c_string(substring, buf);
  free(buf);
}

/* MARK: - Comparing Strings */

/* 
 * Returns an integer greater than, equal to, or less than 0, according as the
 * string lhs is greater than, equal to, or less than the string rhs.
 */
int string_compare(const void* lhs, const void* rhs) {
  var a = *(struct String*)lhs;
  var b = *(struct String*)rhs;
  a.utf8[a.utf8_count] = '\0';
  b.utf8[b.utf8_count] = '\0';
  return strcmp(a.utf8, b.utf8);
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
