/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* binary_search START                                  /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 1.1                                         \ \ \_/__ \_\ \_  __  */
/* Date: December 25, 2023                               \ \_\/\_\/\____\/\_\ */
/*                                                        \/_/\/_/\/____/\/_/ */
/*===----------------------------------------------------------------------===*/

/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#include <stdbool.h>

#define var __auto_type

/*
 * The _binary_search() function searches an array of nel objects, the initial
 * member of which is pointed to by base, for a member that matches the
 * object pointed to by key.  The size (in bytes) of each member of the
 * array is specified by width.
 *
 * The contents of the array should be in ascending sorted order according
 * to the comparison function referenced by compare.  The compare routine is
 * expected to have two arguments which point to the key object and to an
 * array member, in that order.  It should return an integer which is less
 * than, equal to, or greater than zero if the key object is found,
 * respectively, to be less than, to match, or be greater than the array
 * member.
 *
 * The _binary_search() functions returns the first position in which the new
 * element cloud be inserted without changing the ordering.
 */
static int _binary_search(
  const void* key,
  const void* base,
  int nel,
  int width,
  int (*compare)(const void*, const void*)
) {
  var low = 0;
  var high = nel;
  while (low < high) {
    var mid = (low + high) / 2;
    if (compare(base + mid * width, key) < 0) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }
  return low;
}

/*
 * Returns the first position in which the new element cloud be inserted without
 * changing the ordering, or nel if no such element is found.
 */
int lower_bound(
  const void* key,
  const void* base,
  int nel,
  int width,
  int (*compare)(const void*, const void*)
) {
  return _binary_search(key, base, nel, width, compare);
}

/* 
 * Returns a Boolean value indicating whether the sorted sequence contains the
 * given element.
 */
bool binary_search(
  const void* key,
  const void* base,
  int nel,
  int width,
  int (*compare)(const void*, const void*)
) {
  var i = _binary_search(key, base, nel, width, compare);
  return i != nel && compare(base + i * width, key) == 0;
}

/*===----------------------------------------------------------------------===*/
/*             ___                            ___                             */
/*           /'___\                          /\_ \    __                      */
/*          /\ \__/   __      ___      __    \//\ \  /\_\    ___      __      */
/*          \ \ ,__\/'__`\  /' _ `\  /'_ `\    \ \ \ \/\ \ /' _ `\  /'_ `\    */
/*           \ \ \_/\ \L\.\_/\ \/\ \/\ \L\ \    \_\ \_\ \ \/\ \/\ \/\ \L\ \   */
/*            \ \_\\ \__/.\_\ \_\ \_\ \____ \   /\____\\ \_\ \_\ \_\ \____ \  */
/*             \/_/ \/__/\/_/\/_/\/_/\/___L\ \  \/____/ \/_/\/_/\/_/\/___L\ \ */
/* binary_search END                   /\____/                        /\____/ */
/*                                     \_/__/                         \_/__/  */
/*===----------------------------------------------------------------------===*/
