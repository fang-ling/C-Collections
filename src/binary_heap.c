/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* binary_heap   START                                  /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 1.0                                         \ \ \_/__ \_\ \_  __  */
/* Date: March 9, 2024                                   \ \_\/\_\/\____\/\_\ */
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

/*
 * Error code of BinaryHeap:
 * 0: NO ERROR
 * 7: Can't access largest element from an empty heap
 */

#include <stdbool.h>
#include <string.h>

#include "array.h"

#define var __auto_type

struct BinaryHeap {
  struct Array _storage;
  
  /* The number of elements in the heap. */
  int count;
  
  /* The size of stored Element type. */
  int element_size;
  
  int (*compare)(const void*, const void*);
  
  /* A Boolean value indicating whether or not the heap is empty. */
  bool is_empty;
};

#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i) (2 * (i) + 1)
#define RIGHT(i) (2 * (i) + 2)

/* MARK: - (Private) Maintenance of the BinaryHeap property */

/* Maintain the max heap property from node i all the way up to root. */
static void max_heapify_up(struct BinaryHeap* heap, int i) {
  if (i == 0) { /* We are at root node now. */
    return;
  }
  /* _storage[parent(i)] <= _storage[i] */
  if (
    (*heap).compare(
      (*heap)._storage._storage + PARENT(i) * (*heap).element_size,
      (*heap)._storage._storage + i * (*heap).element_size
    ) <= 0
  ) {
    /* Exchange i with parent(i) */
    array_swap_at(&(*heap)._storage, PARENT(i), i);
    /*
     * Current node is satisfy max heap property, but maybe not its
     * parent. Fix this recursively.
     */
    max_heapify_up(heap, PARENT(i));
  }
}

/* Maintain the max heap property from node i all the way down to leaf. */
static void max_heapify_down(struct BinaryHeap* heap, int i) {
  if (i >= (*heap).count / 2) { /* No children, we are done */
    return;
  }
  var j = -1;
  /* Find i's largest children j. */
  if (LEFT(i) < (*heap).count && RIGHT(i) < (*heap).count) {
    /* Both children exist */
    j = (*heap).compare(
          (*heap)._storage._storage + LEFT(i) * (*heap).element_size,
          (*heap)._storage._storage + RIGHT(i) * (*heap).element_size
    ) > 0 ? LEFT(i) : RIGHT(i);
  } else if (LEFT(i) < (*heap).count && RIGHT(i) >= (*heap).count) {
    /* Left only */
    j = LEFT(i);
  } else if (LEFT(i) >= (*heap).count && RIGHT(i) < (*heap).count) {
    /* Right only */
    j = RIGHT(i);
  }
  /*
   * If i is smaller than j (i's largest child), we are violate the max
   * heap property. Fix it by swap i.key with j.key and then recur on j.
   */
  if ( /* _storage[i] < _storage[j]) */
    (*heap).compare(
      (*heap)._storage._storage + PARENT(i) * (*heap).element_size,
      (*heap)._storage._storage + i * (*heap).element_size
    ) <= 0
  ) {
    array_swap_at(&(*heap)._storage, i, j);
    max_heapify_down(heap, j);
  }
}

/* MARK: - Creating and Destroying a Heap */

/* Creates an empty heap. */
void binary_heap_init(
  struct BinaryHeap* heap,
  int element_size,
  int (*compare)(const void*, const void*)
) {
  array_init(&(*heap)._storage, element_size);
  (*heap).element_size = element_size;
  (*heap).is_empty = true;
  (*heap).count = 0;
  (*heap).compare = compare;
}

/* Destroys a heap. */
void binary_heap_deinit(struct BinaryHeap* heap) {
  array_deinit(&(*heap)._storage);
  (*heap).count = 0;
  (*heap).is_empty = true;
}

/* MARK: - Accessing Elements */

/* Returns the largest element in a heap in constant time. */
int binary_heap_max(struct BinaryHeap* heap, void* result) {
  if ((*heap).is_empty) {
    return 7;
  }
  return array_get(&(*heap)._storage, 0, result);
}

/* MARK: - Adding Elements */

/* 
 * Inserts a new item into heap.
 * Complexity: O(lg n), where n is the length of the heap
 */
void binary_heap_insert(struct BinaryHeap* heap, void* new_element) {
  array_append(&(*heap)._storage, new_element);
  (*heap).count = (*heap)._storage.count;
  (*heap).is_empty = false;
  max_heapify_up(heap, (*heap).count - 1);
}

/* MARK: - Removing Elements */

/* Removes the max item in the heap. */
void binary_heap_remove_max(struct BinaryHeap* heap) {
  array_swap_at(&(*heap)._storage, 0, (*heap).count - 1);
  /* It's necessary to first do remove to update `count` */
  array_remove_last(&(*heap)._storage);
  (*heap).count = (*heap)._storage.count;
  (*heap).is_empty = (*heap)._storage.is_empty;
  max_heapify_down(heap, 0);
}

/*===----------------------------------------------------------------------===*/
/*             ___                            ___                             */
/*           /'___\                          /\_ \    __                      */
/*          /\ \__/   __      ___      __    \//\ \  /\_\    ___      __      */
/*          \ \ ,__\/'__`\  /' _ `\  /'_ `\    \ \ \ \/\ \ /' _ `\  /'_ `\    */
/*           \ \ \_/\ \L\.\_/\ \/\ \/\ \L\ \    \_\ \_\ \ \/\ \/\ \/\ \L\ \   */
/*            \ \_\\ \__/.\_\ \_\ \_\ \____ \   /\____\\ \_\ \_\ \_\ \____ \  */
/*             \/_/ \/__/\/_/\/_/\/_/\/___L\ \  \/____/ \/_/\/_/\/_/\/___L\ \ */
/* binary_heap   END                   /\____/                        /\____/ */
/*                                     \_/__/                         \_/__/  */
/*===----------------------------------------------------------------------===*/
