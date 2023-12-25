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

#include "binary_search.h"

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
 *     b. the x.n keys themselves, x.key_0, x.key_1, ..., x.keyx_n-1, stored in
 *        nondecreasing order, so that x.key_0 < x.key_1 < ... < x_keyx_n-1
 *     c. x.leaf, a boolean value that is TRUE if x is a leaf and FALSE if x is
 *        an internal node
 *   2) Each internal node x also contains x.n+1 pointers x.c_0, x.c_1, ...,
 *      x.c_n to its children. Leaf nodes have no children, and so their c_i
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
struct _BTreeNode {
  /* N keys. (Element size is stored in BTree) */
  void* keys;
  /* Duplicate element count for each key. */
  int* key_counts;
  /* N + 1 pointers to its children. */
  struct _BTreeNode** children;
  /* The number of keys currently stored in the node. */
  int n;
  /* A Boolean value indicating whether or not the node is a leaf. */
  bool is_leaf;
};

/* At most 2*t children, at most 2*t - 1 keys. */
static struct _BTreeNode* _b_tree_node_init(int t, int element_size) {
  var node = (struct _BTreeNode*)malloc(sizeof(struct _BTreeNode));
  if (node == NULL) {
    return NULL;
  }

  node -> is_leaf = true;
  node -> n = 0;

  if ((node -> children = calloc(2 * t, sizeof(struct _BTreeNode*))) == NULL) {
    return NULL;
  }
  if ((node -> keys = malloc((2 * t - 1) * element_size)) == NULL) {
    return NULL;
  }
  if ((node -> key_counts = malloc((2 * t - 1) * element_size)) == NULL) {
    return NULL;
  }
  return node;
}

/*
 * x: a nonfull internal node
 * i: an index such that x.c_i is a full child of x
 * width: a.k.a. element_size
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
 *                  Figure: Splitting a node with t = 4.
 *
 * _b_tree_split_child works by straightforward "cutting and pasting." Here, x
 * is the node being split, and y is x's ith child. Node y originally has 2t 
 * children (2t - 1 keys) but is reduced to t children (t - 1 keys) by this
 * operation. Node z takes the t largest children (t - 1 keys) from y, and z
 * becomes a new child of x, positioned just after y in x's table of children.
 * The median key of y moves up to become the key in x that separates y and z.
 */
static void _b_tree_split_child(struct _BTreeNode* x, int t, int i, int width) {
  var z = _b_tree_node_init(t, width);
  var y = x -> children[i];
  z -> is_leaf = y -> is_leaf;
  z -> n = t - 1;
  /* 
   * Move t - 1 largest keys from y to z
   * Example: in Figure: (t = 4)
   * ind: 0    1    2    3    4    5    6
   * y: ['P', 'Q', 'R', 'S', 'T', 'U', 'V']
   * z will have 'T', 'U', 'V', and 'T' has an index of t.
   */
  memmove(z -> keys, y -> keys + t * width, (t - 1) * width);
  /* Move the corresponding key_counts */
  memmove(z -> key_counts, y -> key_counts + t * width, (t - 1) * width);
  /* Movie t largest children from y to z */
  if (!y -> is_leaf) {
    memmove(z -> children, y -> children + t, t * sizeof(struct _BTreeNode*));
  }
  y -> n = t - 1;
  /* Make room (children) in x */
  memmove(
    x -> children + i + 1 + 1,
    x -> children + i + 1,
    (x -> n - i) * sizeof(struct _BTreeNode*)
  );
  x -> children[i + 1] = z;
  /* Make room (keys) in x; (void* not auto-scaled) */
  memmove(
    x -> keys + (i + 1) * width,
    x -> keys + i * width,
    (x -> n - i) * width
  );
  memcpy(x -> keys + i * width, y -> keys + (t - 1) * width, width);
  /* Make room (key_counts) in x */
  memmove(
    x -> key_counts + i + 1,
    y -> key_counts + i,
    (x -> n - i) * sizeof(int)
  );
  x -> key_counts[i] = y -> key_counts[t - 1];

  x -> n += 1;
}

/*
 * The auxiliary recursive procedure _b_tree_insert_nonfull inserts key k into
 * node x, which is assumed to be nonfull when the procedure is called. The
 * operation of b_tree_insert and the recursive operation of
 * _b_tree_insert_nonfull guarantee that this assumption is true.
 */
static void _b_tree_insert_nonfull(
  struct _BTreeNode* x,
  void* k,
  int t,
  int width,
  int (*compare)(const void*, const void*)
) {
  /* Find the place for k: i may be -1 or key[i] is the rightmost key <= k. */
  var i = lower_bound(k, x -> keys, x -> n, width, compare) - 1;
  if (x -> is_leaf) {
    /* The case in which x is a leaf node. Insert key k into x directly. */
    if (compare(k, x -> keys + i * width) == 0) { /* Insert a duplicate key */
      x -> key_counts[i] += 1;
      return;
    }
    /* 
     * Move keys[i+1..<n] to keys[i+2...n], make room for new k.
     * Example: k = 'D', we get i = 1.
     *   0    1    2    3
     * ['A', 'B', 'E', 'G']
     *        ↑
     *        i
     * Then, (i + 1) is the number of element not moved. => # moved = n - (i+1)
     */
    memmove(
      x -> keys + (i + 2) * width,
      x -> keys + (i + 1) * width,
      (x -> n - i - 1) * width
    );/* FIXME: May need move key_counts also ? */
    /*
     * Now keys[i+1] is the right place for k,
     */
    memcpy(x -> keys + (i + 1) * width, k, width);
    x -> n += 1;
  } else {
    i += 1;
    /* Detect whether the recursion would descend to a full child */
    if (x -> children[i] -> n == 2 * t - 1) {
      _b_tree_split_child(x, t, i, width);
      /* does key k go into child i or child i+1? */
      if (compare(k, x -> keys + i * width)) {
        i += 1;
      }
    }
    _b_tree_insert_nonfull(x -> children[i], k, t, width, compare);
  }
}

/*
 * We insert a key k into a B-tree T of height h in a single pass down the tree.
 * The _b_tree_insert procedure uses _b_tree_split_child to guarantee that the
 * recursion never descends to a full node.
 */
static void _b_tree_insert(
  struct _BTreeNode** root, /* We are modifying of value of root! */
  void* k,
  int t,
  int width,
  int (*compare)(const void*, const void*)
) {
  var r = *root;
  if (r -> n == 2 * t - 1) {
    /* s will be the new root */
    var s = _b_tree_node_init(t, width);
    *root = s;
    s -> is_leaf = false;
    s -> children[0] = r;
    _b_tree_split_child(s, t, 0, width);
    _b_tree_insert_nonfull(s, k, t, width, compare);
  } else {
    _b_tree_insert_nonfull(r, k, t, width, compare);
  }
}

/*
 * 1. If the key k is in node x and x is a leaf, delete the key k from x.
 */
static void _b_tree_remove_subtree(
  struct _BTreeNode* x,
  void* k,
  int t,
  int width,
  int (*compare)(const void*, const void*)
) {
  /* Find the place for k: i may be -1 or key[i] is the rightmost key <= k. */
  var i = lower_bound(k, x -> keys, x -> n, width, compare) - 1;
  if (i != x -> n && compare(k, x -> keys + i * width)) { /* k is in node x */
    if (x -> key_counts[i] > 1) { /* delete dupicate element */
      x -> key_counts[i] -= 1;
      return;
    }
    /*
     * Case 1: Delete key k from node x which is a leaf. Do nothing if node x
     * does not contain key k.
     */
    if (x -> is_leaf) {
      x -> n -= 1;
      memmove(
        x -> keys + (i + 1 - 1) * width,
        x -> keys + (i + 1) * width,
        (x -> n - i) * width
      );
      memmove(
        x -> key_counts + i + 1 - 1,
        x -> key_counts + i + 1,
        (x -> n - i) * sizeof(int)
      );

    }
  }
}

static struct _BTreeNode* _b_tree_search(
  struct _BTreeNode* x,
  void* k,
  int width,
  int* result_i,
  int (*compare)(const void*, const void*)
) {
  var i = 0;
  var low = 0;
  var high = x -> n - 1;
  while (low <= high) {
    var mid = (low + high) / 2;
    if (compare(k, x -> keys + mid * width) == 0) {
      break;
    } else if (compare(k, x -> keys + mid * width) > 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
    i = mid;
  }

  if (i < x -> n && compare(k, x -> keys + i * width) == 0) {
    *result_i = i;
    return x;
  } else if (x -> is_leaf) {
    *result_i = -1;
    return NULL;
  } else {
    return _b_tree_search(x -> children[i], k, width, result_i, compare);
  }
}

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

/* MARK: - Creating and Destroying a B-Tree */

/* Creates an empty BTree. */
void b_tree_init(
  struct BTree* tree,
  int t,
  int element_size,
  int (*compare)(const void*, const void*)
) {
  (*tree).root = _b_tree_node_init(t, element_size);
  (*tree).t = t;
  (*tree).element_size = element_size;
  (*tree).compare = compare;

  (*tree).count = 0;
  (*tree).is_empty = true;
}

/* Destroys a BTree. (postorder tree traversal) */
static void _b_tree_deinit(struct _BTreeNode* node) {
  if (node != NULL) {
    var i = 0;
    for (i = 0; i < node -> n; i += 1) {
      _b_tree_deinit(node -> children[i]);
    }
    free(node -> keys);
    free(node -> key_counts);
    free(node -> children);
    free(node);
  }
}

/* Destroys a BTree. */
void b_tree_deinit(struct BTree* tree) {
  _b_tree_deinit((*tree).root);
}

/* MARK: - Adding Elements */

/* Adds a new element in the B-Tree. */
void b_tree_insert(struct BTree* tree, void* key) {
  /* FIXME: update count & is_empty */
  _b_tree_insert(
    &(*tree).root,
    key,
    (*tree).t,
    (*tree).element_size,
    (*tree).compare
  );
}

/* MARK: - Finding Elements */

bool b_tree_contains(struct BTree* tree, void* key) {
  var i = 0;
  if (
    _b_tree_search(
      (*tree).root,
      key,
      (*tree).element_size,
      &i,
      (*tree).compare
    ) == NULL
  ) {
    return false;
  }
  return true;
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
