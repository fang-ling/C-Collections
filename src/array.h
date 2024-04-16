/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef array_h
#define array_h

struct Array {
  void* _storage;
  
  /* The number of elements in the array. */
  int count;
  
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
  int capacity;
  
  /* The size of stored Element type. */
  int element_size;
  
  /* A Boolean value indicating whether or not the array is empty. */
  bool is_empty;
};

/* Creates an empty array. */
int array_init(struct Array* array, int element_size);

/* Destroys an array. */
void array_deinit(struct Array* array);

/* Returns the element at the specified position. */
int array_get(struct Array* array, int index, void* element);

/* Replaces the element at the specified position. */
int array_set(struct Array* array, int index, void* element);

/* Adds a new element at the end of the array. */
int array_append(struct Array* array, void* element);

/*
 * Inserts a new element at the specified position.
 *
 * - Complexity:
 *   O(n), where n is the length of the array.
 */
int array_insert(struct Array* array, void* element, int i);

/* Removes the last element of the collection. */
int array_remove_last(struct Array* array);

/*
 * Removes the element at the specified position.
 *
 * - Complexity:
 *   O(n), where n is the length of the array.
 */
int array_remove_at(struct Array* array, int i);

/* Removes all the elements. */
int array_remove_all(struct Array* array);

/*
 * Returns a Boolean value indicating whether the sequence contains the given
 * element.
 */
bool array_contains(
  struct Array* array,
  void* key,
  bool (*equal)(const void*, const void*)
);

/*
 * Returns the first index where the specified value appears in the collection.
 */
int array_first_index(
  struct Array* array,
  void* key,
  bool (*equal)(const void*, const void*)
);

/* Sorts the collection in place. */
void array_sort(struct Array* array, int (*compare)(const void*, const void*));

/* Exchanges the values at the specified indices of the collection. */
int array_swap_at(struct Array* array, int i, int j);

/*
 * Returns a Boolean value indicating whether two arrays contain the same
 * elements in the same order.
 */
bool array_equal(
  struct Array* lhs,
  struct Array* rhs/*,
  int (*elem_compare)(const void*, const void*)*/
);

/* Appends the elements of an array to this array. */
void array_combine(struct Array* array, struct Array* other);

#endif /* array_h */
