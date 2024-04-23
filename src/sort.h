/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef sort_h
#define sort_h

#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h> /* For size_t */

#define INS_THR 64

/* Byte-wise swap two items of size SIZE. */
#define SWAP(a, b, size)      \
  do {                        \
    size_t __size = (size);   \
    char *__a = (a);          \
    char *__b = (b);          \
    do {                      \
      char __tmp = *__a;      \
      *__a++ = *__b;          \
      *__b++ = __tmp;         \
    } while (--__size > 0);   \
  } while (0);

/**
 * The `sort()` function is a modified partition-exchange sort, or quicksort.
 *
 * The `sort()` function sort an array of `nel` objects, the initial member of
 * which is pointed to by `base`.  The size of each object is specified by
 * `width`.
 *
 * The contents of the array `base` are sorted in ascending order according to
 * a comparison function pointed to by `compare`, which requires two arguments
 * pointing to the objects being compared.
 *
 * The comparison function must return an integer less than, equal to, or
 * greater than zero if the first argument is considered to be respectively
 * less than, equal to, or greater than the second.
 */
void sort(
  void* base,
  size_t nel,
  size_t width,
  int (*compare)(const void*, const void*)
);

#endif /* sort_h */

