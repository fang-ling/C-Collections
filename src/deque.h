/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2024 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef deque_h
#define deque_h

#include "types.h"

#include <string.h>

#include "array.h"

#define DEQUE_FATAL_ERR_REMFT "Can't remove first element from an empty deque"
#define DEQUE_FATAL_ERR_OUTOB "Index out of range"

struct Deque {
  struct Array* _head;
  struct Array* _tail;

  /*
   * Let head.count = h, tail.count = t
   *
   *  h                               h-1       0
   *  +--------+--------+-...-+--------+--------+
   *  |//tail//|        |-...-|        |\\head\\|
   *  +--------+--------+-...-+--------+--------+
   *
   *  head stores items in reversed order.
   *
   *  deque.append() causes tail.append()
   *  deque.remove_last() causes tail.remove_last()
   *  deque.prepend() causes head.append()
   *  deque.remove_first() causes head.remove_last()
   *
   *  remove from empty head or tail causes reblance.
   */

  /**
   * The number of elements in the deque.
   */
  Int64 count;

  /* The size of stored Element type. */
  UInt32 _width;

  /**
   * A Boolean value indicating whether the deque is empty.
   *
   * When you need to check whether your deque is empty, use the `is_empty`
   * property instead of checking that the `count` property is equal to zero.
   */
  Bool is_empty;
};

/*----------------------------------------------------------------------------*/
/**
 * Creates an empty deque.
 *
 * `deque_init()` allocates and initializes a Deque structure.
 *
 * - Parameters:
 *   - width: The size of stored Element type.
 *
 * - Returns: A pointer to the deque initialized to be empty is returned. If the
 * allocation fails, it returns NULL.
 */
struct Deque* deque_init(UInt32 width);

/**
 * Destroys a deque.
 *
 * `deque_deinit()` frees the components of the Deque, and the structure
 * itself. If `deque` is a NULL pointer, no operation is performed.
 *
 * - Parameters:
 *   - deque: The deque to be deinitialized.
 */
void deque_deinit(struct Deque* deque);

/**
 * Adds a new element at the end of the deque.
 *
 * - Parameters:
 *   - new_element: The element to append to the deque.
 */
void deque_append(struct Deque* deque, void* new_element);

/* Returns the element at the specified position. */
void deque_get(struct Deque* deque, Int64 index, void* element);

/**
 * Removes the first element of the deque.
 *
 * The deque must not be empty. This function does not destroy the removed
 * element. If the stored type is a reference type, it is the caller's
 * responsibility to handle its deallocation.
 */
void deque_remove_first(struct Deque* deque);

/**
 * Removes all elements from the deque.
 */
void deque_remove_all(struct Deque* deque);
/*----------------------------------------------------------------------------*/

#endif /* deque_h */
