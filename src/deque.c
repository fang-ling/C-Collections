/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* Deque START                                          /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 1.1                                         \ \ \_/__ \_\ \_  __  */
/* Date: December 11, 2023                               \ \_\/\_\/\____\/\_\ */
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

//#include <stdbool.h>
//#include <string.h>
//
//#include "array.h"
//
//#define var __auto_type
//
///*
// * Error code of Deque:
// * 0: NO ERROR
// * 1: due to malloc, check `errno`
// * 2: Array index is out of range
// * 3: Negative Array index is out of range
// * 5: due to realloc, check `errno`
// * 6: Can't remove last element from an empty collection
// */
//
//struct Deque {
//  struct Array _head;
//  struct Array _tail;
//
//  /*
//   * Let head.count = h, tail.count = t
//   *
//   *  h                               h-1       0
//   *  +--------+--------+-...-+--------+--------+
//   *  |//tail//|        |-...-|        |\\head\\|
//   *  +--------+--------+-...-+--------+--------+
//   *
//   *  head stores items in reversed order.
//   *
//   *  deque.append() causes tail.append()
//   *  deque.remove_last() causes tail.remove_last()
//   *  deque.prepend() causes head.append()
//   *  deque.remove_first() causes head.remove_last()
//   *
//   *  remove from empty head or tail causes reblance.
//   */
//
//  /* The number of elements in the array. */
//  int count;
//
//  /* The size of stored Element type. */
//  int element_size;
//
//  /* A Boolean value indicating whether or not the array is empty. */
//  bool is_empty;
//};
//
///* MARK: - Creating and Destroying an Array */
//
///* Creates an empty deque. */
//int deque_init(struct Deque* deque, int element_size) {
//  (*deque).count = 0;
//  (*deque).is_empty = true;
//  (*deque).element_size = element_size;
//
//  var err = 0;
//  if ((err = array_init(&(*deque)._head, element_size)) != 0) {
//    return err;
//  }
//  if ((err = array_init(&(*deque)._tail, element_size)) != 0) {
//    return err;
//  }
//  return err;
//}
//
///* Destroys a deque. */
//void deque_deinit(struct Deque* deque) {
//  array_deinit(&(*deque)._head);
//  array_deinit(&(*deque)._tail);
//}
//
///* MARK: - Accessing Elements */
//
///* Check that the specified `index` is valid, i.e. `0 ≤ index < count`. */
//static int _deque_check_index(struct Deque* deque, int index) {
//  if (index >= (*deque).count) {
//    return 2;
//  } else if (index < 0) {
//    return 3;
//  }
//  return 0;
//}
//
///* Returns the element at the specified position. */
//int deque_get(struct Deque* deque, int index, void* element) {
//  var err = _deque_check_index(deque, index);
//  if (err != 0) {
//    return err;
//  }
//  /* Calculate real index */
//  if (index >= (*deque)._head.count) { /* in tail */
//    index -= (*deque)._head.count;
//    memcpy(
//      element,
//      (*deque)._tail._storage + (*deque).element_size * index,
//      (*deque).element_size
//    );
//  } else { /* in head */
//    index = (*deque)._head.count - 1 - index;
//    memcpy(
//      element,
//      (*deque)._head._storage + (*deque).element_size * index,
//      (*deque).element_size
//    );
//  }
//
//  return err;
//}
//
///* Replaces the element at the specified position. */
//int deque_set(struct Deque* deque, int index, void* element) {
//  var err = _deque_check_index(deque, index);
//  if (err != 0) {
//    return err;
//  }
//  /* Calculate real index */
//  if (index >= (*deque)._head.count) { /* in tail */
//    index -= (*deque)._head.count;
//    memcpy(
//      (*deque)._tail._storage + (*deque).element_size * index,
//      element,
//      (*deque).element_size
//    );
//  } else { /* in head */
//    index = (*deque)._head.count - 1 - index;
//    memcpy(
//      (*deque)._head._storage + (*deque).element_size * index,
//      element,
//      (*deque).element_size
//    );
//  }
//
//  return err;
//}
//
///* MARK: - Adding Elements */
//
///* Adds a new element at the end of the deque. */
//int deque_append(struct Deque* deque, void* element) {
//  (*deque).count += 1;
//  (*deque).is_empty = false;
//  return array_append(&(*deque)._tail, element);
//}
//
///* Adds a new element at the front of the deque. */
//int deque_prepend(struct Deque* deque, void* element) {
//  (*deque).count += 1;
//  (*deque).is_empty = false;
//  return array_append(&(*deque)._head, element);
//}
//
///* MARK: - Removing Elements */
//
///*
// * This function assumes that either the head or the tail is empty, but not 
// * both.
// */
//static int _rebalance(struct Array* empty, struct Array* full) {
//  var count = (*empty).count + (*full).count;
//  var half_count = count / 2;
//  var err = 0;
//  /* TODO: use memcpy */
//  /* copy the first half to empty */
//  var i = 0;
//  for (i = half_count - 1; i >= 0; i -= 1) { /* Important: append backwords */
//    err = array_append(empty, (*full)._storage + i * (*full).element_size);
//  }
//  /* shift the second half to the front of full */
//  memmove(
//    (*full)._storage,
//    (*full)._storage + half_count * (*full).element_size,
//    (count - half_count) * (*full).element_size
//  );
//  (*full).count = count - half_count;
//  return err;
//}
//
///* Removes the last element of the collection. */
//int deque_remove_last(struct Deque* deque) {
//  if ((*deque).is_empty) {
//    return 6;
//  }
//
//  (*deque).count -= 1;
//  if ((*deque).count == 0) {
//    (*deque).is_empty = true;
//  }
//
//  /* Special fix for reblance */
//  if ((*deque).count == 0) { /* either head or tail is empty */
//    if ((*deque)._head.is_empty) {
//      return array_remove_last(&(*deque)._tail);
//    } else {
//      return array_remove_last(&(*deque)._head);
//    }
//  }
//
//  var err = 0;
//  if ((*deque)._head.is_empty && (*deque)._tail.is_empty) {
//    /* never happen */
//  } else if (!(*deque)._tail.is_empty) {
//    /* Normal case */
//  } else {
//    /* Bad case: rebalance needed */
//    _rebalance(&(*deque)._tail, &(*deque)._head);
//  }
//  err = array_remove_last(&(*deque)._tail);
//
//  return err;
//}
//
///* Removes the first element of the collection. */
//int deque_remove_first(struct Deque* deque) {
//  if ((*deque).is_empty) {
//    return 6;
//  }
//
//  (*deque).count -= 1;
//  if ((*deque).count == 0) {
//    (*deque).is_empty = true;
//  }
//
//  /* Special fix for reblance */
//  if ((*deque).count == 0) { /* either head or tail is empty */
//    if ((*deque)._head.is_empty) {
//      return array_remove_last(&(*deque)._tail);
//    } else {
//      return array_remove_last(&(*deque)._head);
//    }
//  }
//
//  var err = 0;
//  if ((*deque)._head.is_empty && (*deque)._tail.is_empty) {
//    /* never happen */
//  } else if (!(*deque)._head.is_empty) {
//    /* Normal case */
//  } else {
//    /* Bad case: rebalance needed */
//    _rebalance(&(*deque)._head, &(*deque)._tail);
//  }
//  err = array_remove_last(&(*deque)._head);
//
//  return err;
//}
//
///* Removes all the elements. */
//int deque_remove_all(struct Deque* deque) {
//  (*deque).count = 0;
//  (*deque).is_empty = true;
//  array_remove_all(&(*deque)._head);
//  array_remove_all(&(*deque)._tail);
//
//  return 0;
//}

/*===----------------------------------------------------------------------===*/
/*             ___                            ___                             */
/*           /'___\                          /\_ \    __                      */
/*          /\ \__/   __      ___      __    \//\ \  /\_\    ___      __      */
/*          \ \ ,__\/'__`\  /' _ `\  /'_ `\    \ \ \ \/\ \ /' _ `\  /'_ `\    */
/*           \ \ \_/\ \L\.\_/\ \/\ \/\ \L\ \    \_\ \_\ \ \/\ \/\ \/\ \L\ \   */
/*            \ \_\\ \__/.\_\ \_\ \_\ \____ \   /\____\\ \_\ \_\ \_\ \____ \  */
/*             \/_/ \/__/\/_/\/_/\/_/\/___L\ \  \/____/ \/_/\/_/\/_/\/___L\ \ */
/* Deque END                           /\____/                        /\____/ */
/*                                     \_/__/                         \_/__/  */
/*===----------------------------------------------------------------------===*/
