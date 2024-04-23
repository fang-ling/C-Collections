/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2024 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef array_h
#define array_h

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h> /* For printing error messages */

#include <stddef.h> /* For size_t */

#include "sort.h"

#define var __auto_type

#define ARRAY_MULTIPLE_FACTOR 2
#define ARRAY_RESIZE_FACTOR   4

#define ARRAY_FATAL_ERR_MALLOC "malloc() return a NULL pointer, check errno"
#define ARRAY_FATAL_ERR_REALLO "realloc() return a NULL pointer, check errno"
#define ARRAY_FATAL_ERR_REMEM  "Can't remove last element from an empty array"

struct Array {
  void* _storage;
  
  /**
   * The number of elements in the array.
   */
  size_t count;
  
  /*
   * The total number of elements that the array can contain without
   * allocating new storage.
   *
   * Every array reserves a specific amount of memory to hold its contents.
   * When you add elements to an array and that array begins to exceed its
   * reserved capacity, the array allocates a larger region of memory and
   * copies its elements into the new storage. The new storage is a multiple
   * of the old storage's size. This exponential growth strategy means that
   * appending an element happens in constant time, averaging the performance
   * of many append operations. Append operations that trigger reallocation
   * have a performance cost, but they occur less and less often as the array
   * grows larger.
   */
  size_t _capacity;
  
  /* The size of stored Element type. */
  size_t _width;
  
  /**
   * A Boolean value indicating whether the array is empty.
   *
   * When you need to check whether your array is empty, use the `is_empty`
   * property instead of checking that the `count` property is equal to zero.
   */
  bool is_empty;
};

/**
 * Creates an empty array.
 *
 * `array_init()` allocates and initializes a Array structure.
 *
 * - Parameters:
 *   - width: The size of stored Element type.
 *
 * - Returns: A pointer to the array initialized to be empty is returned. If the
 * allocation fails, it returns NULL.
 */
struct Array* array_init(size_t width);

/**
 * Destroys an array.
 *
 * `array_deinit()` frees the components of the Array, and the structure
 * itself. If `array` is a NULL pointer, no operation is performed.
 *
 * - Parameters:
 *   - array: The array to be deinitialized.
 */
void array_deinit(struct Array* array);

/**
 * Adds a new element at the end of the array.
 *
 * Use this function to append a single element to the end of a mutable array.
 *
 * Because arrays increase their allocated capacity using an exponential 
 * strategy, appending a single element to an array is an _O(1)_ operation when
 * averaged over many calls to the `array_append()` function.
 * When an array has additional capacity, appending an element is _O(1)_. When
 * an array needs to reallocate storage before appending, appending is _O(n)_,
 * where _n_ is the length of the array.
 *
 * - Parameters:
 *   - new_element: The element to append to the array.
 */
void array_append(struct Array* array, void* new_element);

/**
 * Removes the last element of the array.
 *
 * The array must not be empty. This function does not destroy the removed
 * element. If the stored type is a reference type, it is the caller's
 * responsibility to handle its deallocation.
 */
void array_remove_last(struct Array* array);

/**
 * Removes all elements from the array.
 */
void array_remove_all(struct Array* array);

/**
 * Sorts the array in place.
 *
 * The contents of the array are sorted in ascending order according to
 * a comparison function pointed to by `compare`, which requires two arguments
 * pointing to the objects being compared.
 *
 * The comparison function must return an integer less than, equal to, or
 * greater than zero if the first argument is considered to be respectively
 * less than, equal to, or greater than the second.
 */
void array_sort(struct Array* array, int (*compare)(const void*, const void*));

#endif /* array_h */
