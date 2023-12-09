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

/* Removes the last element of the collection. */
int array_remove_last(struct Array* array);

/* Removes all the elements. */
int array_remove_all(struct Array* array);

/*
 * Returns a Boolean value indicating whether the sequence contains an element
 * that satisfies the given predicate.
 *
 * Parameters:
 *   where: A function pointer that takes an element of the sequence as its
 *          argument and returns a Boolean value that indicates whether the
 *          passed element represents a match.
 *
 * Discussion:
 *   This example shows how you can check an array for an expense less than $59.
 *
 *   ```
 *   bool where(const void* elem) {
 *     if (*(int*)elem < 59) {
 *       return true;
 *     }
 *     return false;
 *   }
 *
 *   struct Array array;
 *   array_init(&array, sizeof(int));
 *
 *   int expenses[] = {19358, 12333, 19348, 19306, 19306, 58};
 *   for (var i = 0; i < 6; i += 1) {
 *     array_append(&array, &expenses[i]);
 *   }
 *   array_contains(&array, where); // Returnes true
 *
 *   array_deinit(&array);
 *   ```
 */
bool array_contains(struct Array* array, bool (*where)(const void*));

/*
 * Returns the first index in which an element of the collection satisfies the
 * given predicate.
 *
 * Parameters:
 *   where: A function pointer that takes an element of the sequence as its
 *          argument and returns a Boolean value that indicates whether the
 *          passed element represents a match.
 *
 * Return Value:
 *  The index of the first element for which `where` returns true. If no
 *  elements in the collection satisfy the given predicate, returns -1.
 *
 * Disscussion:
 *   Here’s an example that finds a number that ends with the digit `3`:
 *
 *   ```
 *   bool where(const void* elem) {
 *     if (*(int*)elem % 10 == 3) {
 *       return true;
 *     }
 *     return false;
 *   }
 *
 *   struct Array array;
 *   array_init(&array, sizeof(int));
 *
 *   int input[] = {19358, 12333, 19348, 19306, 19306, 58};
 *   for (var i = 0; i < 6; i += 1) {
 *     array_append(&array, &input[i]);
 *   }
 *   array_first_index(&array, where_3); // Returns 1
 *
 *   array_deinit(&array);
 *   ```
 */
int array_first_index(struct Array* array, bool (*where)(const void*));

/* Sorts the collection in place. */
void array_sort(struct Array* array, int (*compare)(const void*, const void*));

#endif /* array_h */
