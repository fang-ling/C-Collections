/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef deque_h
#define deque_h

struct Deque {
  struct Array _head;
  struct Array _tail;

  /* The number of elements in the array. */
  int count;

  /* The size of stored Element type. */
  int element_size;

  /* A Boolean value indicating whether or not the array is empty. */
  bool is_empty;
};

/* Creates an empty deque. */
int deque_init(struct Deque* deque, int element_size);

/* Destroys a deque. */
void deque_deinit(struct Deque* deque);

/* Returns the element at the specified position. */
int deque_get(struct Deque* deque, int index, void* element);

/* Replaces the element at the specified position. */
int deque_set(struct Deque* deque, int index, void* element);

/* Adds a new element at the end of the deque. */
int deque_append(struct Deque* deque, void* element);

/* Adds a new element at the front of the deque. */
int deque_prepend(struct Deque* deque, void* element);

/* Removes the last element of the collection. */
int deque_remove_last(struct Deque* deque);

/* Removes the first element of the collection. */
int deque_remove_first(struct Deque* deque);

#endif /* deque_h */
