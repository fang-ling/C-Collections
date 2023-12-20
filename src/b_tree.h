/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef b_tree_h
#define b_tree_h

struct BTree {
  struct _BTreeNode* root;

  /* The number of elements in the B-tree. */
  int count;
  /* The size of stored Element type. */
  int element_size;
  /* A fixed integer t ≥ 2 called the minimum degree of the B-tree. */
  int t;

  int (*compare)(const void*, const void*);

  /* A Boolean value indicating whether or not the array is empty. */
  bool is_empty;
};

/* Creates an empty BTree. */
void b_tree_init(
  struct BTree* tree,
  int t,
  int element_size,
  int (*compare)(const void*, const void*)
);

/* Destroys a BTree. */
void b_tree_deinit(struct BTree* tree);

/* Adds a new element in the B-Tree. */
void b_tree_insert(struct BTree* tree, void* key);

bool b_tree_contains(struct BTree* tree, void* key);

#endif /* b_tree_h */
