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
#include <string.h>
#include <stdlib.h>

#define var __auto_type

/*
 * Error code of BTree:
 * 0: NO ERROR
 * 1: due to malloc, check `errno`
 */

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
 *
 *   5) Nodes have lower and upper bounds on the number of keys they can 
 *      contain. We express these bounds in terms of a fixed integer t ≥ 2
 *      called the minimum degree of the B-tree:
 *     a. Every node other than the root must have at least t - 1 keys. Every
 *        internal node other than the root thus has at least t children. If the
 *        tree is nonempty, the root must have at least one key.
 *     b. Every node may contain at most 2t - 1 keys. Therefore, an internal
 *        node may have at most 2t children. We say that a node is full if it
 *        contains exactly 2t - 1 keys.
 */
struct _BTreeNodeKey {
  /* Actual value of the key. */
  void* value;
  /* Duplicate element count. */
  int count;
};

struct _BTreeNode {
  /* N keys. */
  struct _BTreeNodeKey* keys;
  /* N + 1 pointers to its children. */
  struct _BTreeNode** children;
  /* The number of keys currently stored in the node. */
  int n;
  /* The size of the subtree. */
  int size;
  /* A Boolean value indicating whether or not the node is a leaf. */
  bool is_leaf;
};

//static void _b_tree_node_key_init(struct _BTreeNodeKey* key, void* value) {
//
//}

/* At most 2*t children, at most 2*t - 1 keys. */
static struct _BTreeNode* _b_tree_node_init(int t) {
  var node = (struct _BTreeNode*)malloc(sizeof(struct _BTreeNode));
  if (node == NULL) {
    return NULL;
  }

  node -> is_leaf = true;
  node -> n = 0;
  node -> size = 0;

  if ((node -> children = malloc(2 * t * sizeof(struct _BTreeNode*))) == NULL) {
    return NULL;
  }
  node -> keys = malloc((2 * t - 1) * sizeof(struct _BTreeNodeKey));
  if (node -> keys == NULL) {
    return NULL;
  }
  return node;
}

static void _b_tree_node_deinit(struct _BTreeNode* node) {
  free((*node).children);
  free((*node).keys);
}

/*
 * x: a nonfull internal node
 * i: an index such that x.c_i is a full child of x
 *
 * The procedure then splits this child in two and adjusts x so that it has an 
 * additional child. To split a full root, we will first make the root a child
 * of a new empty root node, so that we can use _b_tree_split_child. The tree
 * thus grows in height by one; splitting is the only means by which the tree
 * grows.
 * The next figure illustrates this process. We split the full node y = x.c_i
 * about its median key S, which moves up into y's parent node x. Those keys in
 * y that are greater than the median key move into a new node z, which becomes
 * a new child of x.
 *
 *                                               ↙----------x.key_i-1
 *          ↙--------x.key_i-1                  |  ↙--------x.key_i
 *  x      |  ↙------x.key_i             x      | |  ↙------x.key_i+1
 *   +-----↓-↓-----+                      +-----↓-↓-↓-----+
 *   | . . N W . . |                      | . . N S W . . |
 *   +------|------+                      +------/-\------+
 *          |          ---------------->        /   \
 *  y=x.c_i |                          y=x.c_i /     \ z=x.c_i+1
 *  +-------↓-------+                    +----/--+ +--\----+
 *  | P Q R S T U V |                    | P Q R | | T U V |
 *  +---------------+                    +-------+ +-------+
 *
 *
 *                  Figure: Splitting a node with t = 4.
 *
 * _b_tree_split_child works by straightforward "cutting and pasting." Here, x
 * is the node being split, and y is x's ith child. Node y originally has 2t 
 * children (2t - 1 keys) but is reduced to t children (t - 1 keys) by this
 * operation. Node z takes the t largest children (t - 1 keys) from y, and z
 * becomes a new child of x, positioned just after y in x's table of children.
 * The median key of y moves up to become the key in x that separates y and z.
 */
static void _b_tree_split_child(struct _BTreeNode* x, int t, int i) {
  var z = _b_tree_node_init(t);
  var y = x -> children[i];
  z -> is_leaf = y -> is_leaf;
  z -> n = t - 1;
  /* Move t - 1 largest keys from y to z */
  memmove(z -> keys, y -> keys + t, (t - 1) * sizeof(struct _BTreeNodeKey));
  /* Movie t largest children from y to z */
  if (!y -> is_leaf) {
    memmove(z -> children, y -> children + t, t * sizeof(struct _BTreeNode*));
  }
  y -> n = t - 1;
  /* Make room for z (children) */
  memmove(
    x -> children + i + 1 + 1,
    x -> children + i + 1,
    (x -> n - i) * sizeof(struct _BTreeNode*)
  );
  x -> children[i + 1] = z;
  /* Make room for z (keys) */
  memmove(
    x -> keys + i + 1,
    x -> keys + i,
    (x -> n - i) * sizeof(struct _BTreeNodeKey)
  );
  x -> keys[i] = y -> keys[t - 1];
  // TODO: maintain subtree size
  x -> n += 1;
}

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
