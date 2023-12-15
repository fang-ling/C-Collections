/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* BTree START                                          /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 1.0                                         \ \ \_/__ \_\ \_  __  */
/* Date: December 15, 2023                               \ \_\/\_\/\____\/\_\ */
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

#include <stdbool.h>

/*
 * B-tree properties:
 *   1) Every node x has the following attributes:
 *     a. x.n, the number of keys currently stored in node x
 *     b. the x.n keys themselves, x.key_1, x.key_2, ..., x.keyx_n, stored in 
 *        nondecreasing order, so that x.key_1 ≤ x.key_2 ≤ ... ≤ x_keyx_n
 *     c. x.leaf, a boolean value that is TRUE if x is a leaf and FALSE if x is
 *        an internal node
 *   2) Each internal node x also contains x.n+1 pointers x.c_1, x.c_2, ...,
 *      x.c_n+1 to its children. Leaf nodes have no children, and so their c_i
 *      attributes are undefined
 */
struct BTreeNodeKey {
  /* Actual value of the key. */
  void* value;
  /* Duplicate element count. */
  int count;
};

struct BTreeNode {
  /* The number of keys currently stored in the node. */
  int n;
  /* N keys. */
  struct BTreeNodeKey* keys;
  /* A Boolean value indicating whether or not the node is a leaf. */
  bool is_leaf;
  /* N + 1 pointers to its children. */
  struct BTreeNode** children;
};

/*===----------------------------------------------------------------------===*/
/*             ___                            ___                             */
/*           /'___\                          /\_ \    __                      */
/*          /\ \__/   __      ___      __    \//\ \  /\_\    ___      __      */
/*          \ \ ,__\/'__`\  /' _ `\  /'_ `\    \ \ \ \/\ \ /' _ `\  /'_ `\    */
/*           \ \ \_/\ \L\.\_/\ \/\ \/\ \L\ \    \_\ \_\ \ \/\ \/\ \/\ \L\ \   */
/*            \ \_\\ \__/.\_\ \_\ \_\ \____ \   /\____\\ \_\ \_\ \_\ \____ \  */
/*             \/_/ \/__/\/_/\/_/\/_/\/___L\ \  \/____/ \/_/\/_/\/_/\/___L\ \ */
/* BTree END                           /\____/                        /\____/ */
/*                                     \_/__/                         \_/__/  */
/*===----------------------------------------------------------------------===*/
