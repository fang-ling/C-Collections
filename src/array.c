/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* Array START                                          /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 2.0                                         \ \ \_/__ \_\ \_  __  */
/* Date: April 22, 2024                                  \ \_\/\_\/\____\/\_\ */
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

#include "array.h"

/*
 * Error code of Array:
 * 0: NO ERROR
 * 1: due to malloc, check `errno`
 * 2: Array index is out of range
 * 3: Negative Array index is out of range
 * 5: due to realloc, check `errno`
 * 6: Can't remove last element from an empty collection
 */

static void _array_init(struct Array* array, Int64 count, UInt32 width) {
  /* Rounding up to next power of 2 */
  var capacity = count - 1;
  capacity |= capacity >> 1;
  capacity |= capacity >> 2;
  capacity |= capacity >> 4;
  capacity |= capacity >> 8;
  capacity |= capacity >> 16;
  capacity += 1;
  
  if (count == 0) {
    array->_storage = NULL;
  } else {
    array->_storage = malloc(capacity * width);
    if (array->_storage == NULL) {
      fprintf(stderr, ARRAY_FATAL_ERR_MALLOC);
      abort();
    }
  }
  array->_width = width;
  array->_capacity = capacity;
  array->count = count;
  array->is_empty = count == 0 ? true : false;
}

/* Check that the specified `index` is valid, i.e. `0 ≤ index < count`. */
static void _array_check_index(struct Array* array, Int64 index) {
  if (index >= array->count || index < 0) {
    fprintf(stderr, ARRAY_FATAL_ERR_OUTOB);
    abort();
  }
}

/* MARK: - Creating and Destroying an Array */

struct Array* array_init(UInt32 width) {
  struct Array* array;
  if ((array = malloc(sizeof(struct Array))) == NULL) {
    return NULL;
  }
  _array_init(array, 0, width);
  return array;
}

void array_deinit(struct Array* array) {
  if (array == NULL) {
    return;
  }
  
  free(array->_storage);
  array->count = 0;
  array->_width = 0;
  array->_capacity = 0;
  array->is_empty = true;
  
  free(array);
}

/* MARK: - Accessing Elements */

/* Returns the element at the specified position. */
void array_get(struct Array* array, Int64 index, void* element) {
  _array_check_index(array, index);
  
  memcpy(
    element,
    array->_storage + array->_width * index,
    array->_width
  );
}

/* Replaces the element at the specified position. */
void array_set(struct Array* array, Int64 index, void* element) {
  _array_check_index(array, index);

  memcpy(
    array->_storage + array->_width * index,
    element,
    array->_width
  );
}

/* MARK: - Adding Elements */

void array_append(struct Array* array, void* new_element) {
  if (array->_capacity == 0) {
    array->_storage = realloc(array->_storage, 1 * array->_width);
    array->_capacity = 1;
  }
  if (array->count == array->_capacity) {
    array->_capacity *= ARRAY_MULTIPLE_FACTOR;
    var new_size = array->_capacity * array->_width;
    array->_storage = realloc(array->_storage, new_size);
    if (array->_storage == NULL) {
      fprintf(stderr, ARRAY_FATAL_ERR_REALLO);
      abort();
    }
  }
  array->count += 1;
  array->is_empty = false;
  memcpy(
    array->_storage + (array->count - 1) * array->_width,
    new_element, 
    array->_width
  );
}

///* 
// * Inserts a new element at the specified position.
// *
// * - Complexity:
// *   O(n), where n is the length of the array.
// */
//int array_insert(struct Array* array, void* element, int i) {
//  if (i == array->count) { /* this method is equivalent to append(_:) */
//    return array_append(array, element);
//  }
//  var err = _check_index(array, i);
//  if (err != 0) {
//    return err;
//  }
//  /* Dumb append to make room for the new element. Update is_empty & count. */
//  if ((err = array_append(array, element)) != 0) {
//    return err;
//  }
//  /*
//   * [1, 2, 3, 4, 5]  <----- insert(100, at: 3)
//   *
//   *  0  1  2  3  4    5
//   * [1, 2, 3, 4, 5, 100]  <---- after append, count = 6
//   *           \--/  <---- shift right by one (i ..< count - 1)
//   *                                          (move length = count - 1 - i)
//   *
//   * Then copy new_element at index 3.
//   *
//   *    0  1  2    3  4  5
//   * = [1, 2, 3, 100, 4, 5]
//   */
//  memmove(
//    array->_storage + (i + 1) * array->_width,
//    array->_storage + i * array->_width,
//    (array->count - 1 - i) * array->_width
//  );
//  memcpy(
//    array->_storage + i * array->_width,
//    element,
//    array->_width
//  );
//  return err;
//}

/* MARK: - Removing Elements */

void array_remove_last(struct Array* array) {
  if (array->is_empty) {
    fprintf(stderr, ARRAY_FATAL_ERR_REMEM);
    abort();
  }
  array->count -= 1;
  if (array->count == 0) {
    array->is_empty = true;
  }
  if (array->count * ARRAY_RESIZE_FACTOR <= array->_capacity) {
    array->_capacity /= ARRAY_MULTIPLE_FACTOR;
    var new_size = array->_capacity * array->_width;
    array->_storage = realloc(array->_storage, new_size);
    if (array->_storage == NULL && new_size != 0) { /* Linux fix */
      fprintf(stderr, ARRAY_FATAL_ERR_REALLO);
      abort();
    }
  }
}

///* 
// * Removes the element at the specified position.
// *
// * - Complexity:
// *   O(n), where n is the length of the array.
// */
//int array_remove_at(struct Array* array, int i) {
//  var err = 0;
//  if ((err = _check_index(array, i)) != 0) {
//    return err;
//  }
//
//  if (i == array->count - 1) {
//    return array_remove_last(array);
//  }
//
//  /*
//   *    0    1    2    3    4    5    6
//   * [1.1, 1.5, 2.9, 1.2, 1.5, 1.3, 1.2]   <---  remove(at: 2)
//   *                 \----------------/  <---  shift left by one (i+1 ..< count)
//   *                                     <--- move length = count - i - 1
//   * [1.1, 1.5, 1.2, 1.5, 1.3, 1.2, 1.2]
//   *
//   * then, call remove_last()
//   *
//   * [1.1, 1.5, 1.2, 1.5, 1.3, 1.2]
//   */
//  memmove(
//    array->_storage + i * array->width,
//    array->_storage + (i + 1) * array->width,
//    (array->count - i - 1) * array->width
//  );
//  return array_remove_last(array);
//}

void array_remove_all(struct Array* array) {
  free(array->_storage);
  array->_storage = NULL;
  array->count = 0;
  array->_capacity = 0;
  array->is_empty = true;
}

/* MARK: - Finding Elements */

/*
 * Returns a Boolean value indicating whether the sequence contains the given 
 * element.
 */
//bool array_contains(
//  struct Array* array,
//  void* key,
//  bool (*equal)(const void*, const void*)
//) {
//  var buf = malloc(array->width);
//  var i = 0;
//  for (i = 0; i < array->count; i += 1) {
//    array_get(array, i, buf);
//    if (equal(buf, key)) {
//      free(buf);
//      return true;
//    }
//  }
//  free(buf);
//  return false;
//}
//
///*
// * Returns the first index where the specified value appears in the collection.
// */
//int array_first_index(
//  struct Array* array,
//  void* key,
//  bool (*equal)(const void*, const void*)
//) {
//  var buf = malloc(array->width);
//  var i = 0;
//  for (i = 0; i < array->count; i += 1) {
//    array_get(array, i, buf);
//    if (equal(buf, key)) {
//      free(buf);
//      return i;
//    }
//  }
//  free(buf);
//  return -1;
//}

/* MARK: - Reordering an Array’s Elements */

void array_sort(
  struct Array* array,
  Int32 (*compare)(const void*, const void*)
) {
  if (array->count <= 1) {
    return;
  }
  sort(array->_storage, array->count, array->_width, compare);
}
//
///* Exchanges the values at the specified indices of the collection. */
//int array_swap_at(struct Array* array, int i, int j) {
//  var err = 0;
//  if ((err = _check_index(array, i)) != 0) {
//    return err;
//  }
//  if ((err = _check_index(array, j)) != 0) {
//    return err;
//  }
//  if (i == j) {
//    return 0;
//  }
//  var width = array->width;
//  var buf = malloc(width);
//  memcpy(buf, array->_storage + i * width, width);
//  memcpy(array->_storage + i * width, array->_storage + j * width, width);
//  memcpy(array->_storage + j * width, buf, width);
//  return 0;
//}

/* MARK: - Comparing Arrays */

/*
 * Returns a Boolean value indicating whether two arrays contain the same
 * elements in the same order.
 */
//bool array_equal(
//  struct Array* lhs,
//  struct Array* rhs/*,
//  int (*elem_compare)(const void*, const void*)*/
//) {
//  if ((*lhs).count != (*rhs).count) {
//    return false;
//  }
//  if (
//    memcmp(
//      (*lhs)._storage,
//      (*rhs)._storage,
//      (*lhs).count * (*lhs).width
//    ) != 0
//  ) {
//    return false;
//  }
//  return true;
//}

/* MARK: - Combining Arrays */

/* 
 * Appends the elements of an array to this array.
 * NEVER do array_combine(&array, &array);
 */
//void array_combine(struct Array* array, struct Array* other) {
//  if (array->width != (*other).width) {
//    return;
//  }
//  var original_count = array->count;
//  array->is_empty = array->is_empty && (*other).is_empty;
//  
//  /* Dummy append for allocating spaces */
//  var i = 0;
//  for (i = 0; i < (*other).count; i += 1) {
//    if (array->capacity == 0) {
//      array->_storage = realloc(array->_storage, 1 * array->width);
//      array->capacity = 1;
//    }
//    if (array->count == array->capacity) {
//      array->capacity *= ARRAY_MULTIPLE_FACTOR;
//      var new_size = array->capacity * array->width;
//      array->_storage = realloc(array->_storage, new_size);
//    }
//    array->count += 1;
//  }
//  
//  memcpy(
//    array->_storage + original_count * array->width,
//    (*other)._storage,
//    (*other).count * (*other).width
//  );
//}

/*===----------------------------------------------------------------------===*/
/*             ___                            ___                             */
/*           /'___\                          /\_ \    __                      */
/*          /\ \__/   __      ___      __    \//\ \  /\_\    ___      __      */
/*          \ \ ,__\/'__`\  /' _ `\  /'_ `\    \ \ \ \/\ \ /' _ `\  /'_ `\    */
/*           \ \ \_/\ \L\.\_/\ \/\ \/\ \L\ \    \_\ \_\ \ \/\ \/\ \/\ \L\ \   */
/*            \ \_\\ \__/.\_\ \_\ \_\ \____ \   /\____\\ \_\ \_\ \_\ \____ \  */
/*             \/_/ \/__/\/_/\/_/\/_/\/___L\ \  \/____/ \/_/\/_/\/_/\/___L\ \ */
/* Array END                           /\____/                        /\____/ */
/*                                     \_/__/                         \_/__/  */
/*===----------------------------------------------------------------------===*/
