/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2024 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef red_black_tree_h
#define red_black_tree_h

#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RBT_FATAL_ERR_REMEM "Can't remove from an empty red black tree."
#define RBT_FATAL_ERR_INDOB "Index out of range."

enum RedBlackTreeColor {
  RBT_RED,
  RBT_BLACK
};

struct RedBlackTreeNode {
  /* The key. (Element size is stored in RedBlackTree) */
  void* key;
  /* 2 pointers to its children. */
  struct RedBlackTreeNode* children[2];
  /* Pointer to its parent */
  struct RedBlackTreeNode* p;
  /* Duplicate element count for the key. */
  Int64 count;
  /* Subtree size */
  Int64 size;
  /* Color of the node */
  enum RedBlackTreeColor color;
};

struct RedBlackTree {
  struct RedBlackTreeNode* root;
  struct RedBlackTreeNode* nil;
  
  UInt32 _width;
  
  /* The number of elements in the tree */
  Int64 count;
  
  /* A Boolean value indicating whether the tree is empty. */
  Bool is_empty;
  
  /*
   * A Boolean value indicating whether a RedBlackTree allows duplicate
   * elements.
   */
  Bool allow_duplicates;
  
  Int32 (*compare)(const void* lhs, const void* rhs);
};

/*----------------------------------------------------------------------------*/
struct RedBlackTree* red_black_tree_init(
  UInt32 width,
  Bool allow_duplicates,
  Int32 (*compare)(const void* lhs, const void* rhs)
);

void red_black_tree_deinit(struct RedBlackTree* tree);

void red_black_tree_insert(struct RedBlackTree* tree, const void* key);

void red_black_tree_remove(struct RedBlackTree* tree, const void* key);

void red_black_tree_select(struct RedBlackTree* tree, Int64 i, void* result);
/*----------------------------------------------------------------------------*/

#endif /* red_black_tree_h */
