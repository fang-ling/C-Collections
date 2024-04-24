/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* Deque START                                          /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 2.0                                         \ \ \_/__ \_\ \_  __  */
/* Date: April 24, 2024                                  \ \_\/\_\/\____\/\_\ */
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

#include "deque.h"

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

/*
 * This function assumes that either the head or the tail is empty, but not
 * both.
 */
static void _deque_rebalance(struct Array* empty, struct Array* full) {
  var count = empty->count + full->count;
  var half_count = count / 2;
  /* TODO: use memcpy */
  /* copy the first half to empty */
  var i = 0ll;
  for (i = half_count - 1; i >= 0; i -= 1) { /* Important: append backwords */
    array_append(empty, full->_storage + i * full->_width);
  }
  /* shift the second half to the front of full */
  memmove(
    full->_storage,
    full->_storage + half_count * full->_width,
    (count - half_count) * full->_width
  );
  full->count = count - half_count;
}

/* Check that the specified `index` is valid, i.e. `0 ≤ index < count`. */
static void _deque_check_index(struct Deque* deque, Int64 index) {
  if (index >= deque->count || index < 0) {
    fprintf(stderr, DEQUE_FATAL_ERR_OUTOB);
    abort();
  }
}

/* MARK: - Creating and Destroying an Array */

struct Deque* deque_init(UInt32 width) {
  struct Deque* deque;
  if ((deque = malloc(sizeof(struct Deque))) == NULL) {
    return NULL;
  }
  
  deque->count = 0;
  deque->is_empty = true;
  deque->_width = width;

  deque->_head = array_init(width);
  deque->_tail = array_init(width);
  
  if (deque->_head == NULL || deque->_tail == NULL) {
    return NULL;
  }
  
  return deque;
}

void deque_deinit(struct Deque* deque) {
  if (deque == NULL) {
    return;
  }
  
  array_deinit(deque->_head);
  array_deinit(deque->_tail);
  
  free(deque);
}

/* MARK: - Accessing Elements */

/* Returns the element at the specified position. */
void deque_get(struct Deque* deque, Int64 index, void* element) {
  _deque_check_index(deque, index);
  
  /* Calculate real index */
  if (index >= deque->_head->count) { /* in tail */
    index -= deque->_head->count;
    memcpy(
      element,
      deque->_tail->_storage + deque->_width * index,
      deque->_width
    );
  } else { /* in head */
    index = deque->_head->count - 1 - index;
    memcpy(
      element,
      deque->_head->_storage + deque->_width * index,
      deque->_width
    );
  }
}
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
/* MARK: - Adding Elements */

void deque_append(struct Deque* deque, void* new_element) {
  deque->count += 1;
  deque->is_empty = false;
  array_append(deque->_tail, new_element);
}

///* Adds a new element at the front of the deque. */
//int deque_prepend(struct Deque* deque, void* element) {
//  (*deque).count += 1;
//  (*deque).is_empty = false;
//  return array_append(&(*deque)._head, element);
//}
//
/* MARK: - Removing Elements */
//

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
/* Removes the first element of the collection. */
void deque_remove_first(struct Deque* deque) {
  if (deque->is_empty) {
    fprintf(stderr, DEQUE_FATAL_ERR_REMFT);
    abort();
  }

  deque->count -= 1;
  if (deque->count == 0) {
    deque->is_empty = true;
  }

  /* Special fix for reblance */
  if (deque->count == 0) { /* either head or tail is empty */
    if (deque->_head->is_empty) {
      array_remove_last(deque->_tail);
      return;
    } else {
      array_remove_last(deque->_head);
      return;
    }
  }

  if (deque->_head->is_empty && deque->_tail->is_empty) {
    /* never happen */
  } else if (!deque->_head->is_empty) {
    /* Normal case */
  } else {
    /* Bad case: rebalance needed */
    _deque_rebalance(deque->_head, deque->_tail);
  }
  array_remove_last(deque->_head);
}

void deque_remove_all(struct Deque* deque) {
  deque->count = 0;
  deque->is_empty = true;
  array_remove_all(deque->_head);
  array_remove_all(deque->_tail);
}

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
