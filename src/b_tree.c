/*===----------------------------------------------------------------------===*/
/*                                                        ___   ___           */
/* BTree START                                          /'___\ /\_ \          */
/*                                                     /\ \__/ \//\ \         */
/* Author: Fang Ling (fangling@fangl.ing)              \ \ ,__\  \ \ \        */
/* Version: 1.0                                         \ \ \_/__ \_\ \_  __  */
/* Date: January 18, 2024                                \ \_\/\_\/\____\/\_\ */
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
 * 2: No such element in the collection
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
  if ((node -> key_counts = calloc((2 * t - 1), sizeof(int))) == NULL) {
    return NULL;
  }
  return node;
}

static void _b_tree_node_deinit(struct _BTreeNode* node) {
  free(node -> keys);
  free(node -> key_counts);
  free(node -> children);
  free(node);
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
  memmove(z -> key_counts, y -> key_counts + t * width, (t - 1) * sizeof(int));
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
    );
    memmove(
      x -> key_counts + i + 2,
      x -> key_counts + i + 1,
      (x -> n - i - 1) * sizeof(int)
    );
    /*
     * Now keys[i+1] is the right place for k,
     */
    memcpy(x -> keys + (i + 1) * width, k, width);
    x -> key_counts[i + 1] = 1;
    x -> n += 1;
  } else {
    i += 1;
    /* Detect whether the recursion would descend to a full child */
    if (x -> children[i] -> n == 2 * t - 1) {
      _b_tree_split_child(x, t, i, width);
      /* does key k go into child i or child i+1? */
      if (compare(k, x -> keys + i * width) > 0) {
        i += 1;
      }
    }
    _b_tree_insert_nonfull(x -> children[i], k, t, width, compare);
  }
}

static struct _BTreeNode* _b_tree_search(
  struct _BTreeNode* x,
  void* k,
  int width,
  int* result_i,
  int (*compare)(const void*, const void*)
) {
  if (x -> n < 1) { /* Empty subtree */
    return NULL;
  }
  var i = lower_bound(k, x -> keys, x -> n, width, compare);

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

/*
 * We insert a key k into a B-tree T of height h in a single pass down the tree.
 * The _b_tree_insert procedure uses _b_tree_split_child to guarantee that the
 * recursion never descends to a full node.
 */
static bool _b_tree_insert(
  struct _BTreeNode** root, /* We are modifying of value of root! */
  void* k,
  int t,
  int width,
  bool allow_duplicates,
  int (*compare)(const void*, const void*)
) {
  var r = *root;
  /* Before split child, check duplicate key */
  var i = 0;
  struct _BTreeNode* dup;
  if ((dup = _b_tree_search(r, k, width, &i, compare)) != NULL) {
    if (!allow_duplicates) {
      return false;
    }
    dup -> key_counts[i] += 1;
    return true;
  }
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
  return true;
}

static struct _BTreeNode* _b_tree_predecessor(struct _BTreeNode* x) {
  if (x -> is_leaf) {
    return x;
  }
  return _b_tree_predecessor(x -> children[x -> n]);
}

static struct _BTreeNode* _b_tree_successor(struct _BTreeNode* x) {
  if (x -> is_leaf) {
    return x;
  }
  return _b_tree_successor(x -> children[0]);
}

static void _b_tree_remove_from_subtree(
  struct _BTreeNode* x,
  void* k,
  int t,
  int width,
  int (*compare)(const void*, const void*)
) {
  /* Find k: i may be n or key[i] == k. */
  var i = lower_bound(k, x -> keys, x -> n, width, compare);
  /* k is in node x */
  if (i < x -> n && compare(k, x -> keys + i * width) == 0) {
    /*
     * Case 1: Delete key k from node x which is a leaf. Do nothing if node x
     * does not contain key k.
     */
    if (x -> is_leaf) {
      x -> n -= 1;
      /*
       * Key k is in position i, so shift all keys greater than key[i] one 
       * position to the left.
       */
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
    } else {
      /* Case 2: Remove key k = x.key[i] from internal node x. */
      var y = x -> children[i];
      if (y -> n >= t) { /* Case 2a: y has at least t keys. */
        var y_pre = _b_tree_predecessor(y);
        var k_prime = malloc(width);
        memcpy(k_prime, y_pre -> keys + (y_pre -> n - 1) * width, width);
        var k_count_prime = y_pre -> key_counts[y_pre -> n - 1];
        _b_tree_remove_from_subtree(y, k_prime, t, width, compare);
        memcpy(x -> keys + i * width, k_prime, width);
        x -> key_counts[i] = k_count_prime;
        free(k_prime);
      } else { /* y has fewer than t keys */
        var z = x -> children[i + 1];
        /* Symmetrically, examine the child z that follows key k in node x. */
        if (z -> n >= t) {
          /* Case 2b: z has at least t keys. Symmetric to case 2a. */
          var z_suc = _b_tree_successor(z);
          var k_prime = malloc(width);
          memcpy(k_prime, z_suc -> keys + 0 * width, width);
          var k_count_prime = z_suc -> key_counts[0];
          _b_tree_remove_from_subtree(z, k_prime, t, width, compare);
          memcpy(x -> keys + i * width, k_prime, width);
          x -> key_counts[i] = k_count_prime;
          free(k_prime);
        } else {
          /*
           * Case 2c: Both y and z have t - 1 keys.
           * Merge ket a and all of z into y, so that x loses both k and the
           * pointer to z. y then contains 2t - 1 keys. Free z and recursively
           * key k from y.
           */
          /* x -> key[i] == k, merge key k into y */
          memmove(y -> keys + y -> n * width, x -> keys + i * width, width);
          y -> key_counts[y -> n] = x -> key_counts[i];
          /* merge z into y */
          memmove(y -> keys + (y -> n + 1) * width, z -> keys, z -> n * width);
          memmove(
            y -> key_counts + y -> n + 1,
            z -> key_counts,
            z -> n * sizeof(int)
          );
          /* If y and z are not leaves, copy z's child pointers. */
          if (!y -> is_leaf) {
            memmove(
              y -> children + y -> n + 1,
              z -> children,
              (z -> n + 1) * sizeof(struct _BTreeNode*)
            );
          }
          y -> n += z -> n + 1; /* update count */
          /* Remove k and z from x. */
          /*
           * i = 2, n = 5
           * 0 1 2 3 4
           * A B C D E
           *      \---/ n - i - 1 = 5 - 2 - 1 = 2.
           */
          memmove(
            x -> keys + i * width,
            x -> keys + (i + 1) * width,
            (x -> n - i - 1) * width
          );
          memmove(
            x -> key_counts + i,
            x -> key_counts + i + 1,
            (x -> n - i - 1) * sizeof(int)
          );
          /* FIXME: May miscalculate the move length? */
          memmove(
            x -> children + i + 1,
            x -> children + i + 2,
            (x -> n - i - 1) * sizeof(struct _BTreeNode*)
          );
          x -> n -= 1;
          _b_tree_node_deinit(z);
          /* Recursively remove key k from y. */
          _b_tree_remove_from_subtree(y, k, t, width, compare);
        }
      }
    }
  } else {
    /* Case 3: */
    var old_child = x -> children[i];
    /* Ensure that a given child of a node has at least t keys. */
    var child = x -> children[i];
    if (child -> n < t) {
      struct _BTreeNode* left_sibling = NULL;
      var left_n = 0;
      if (i > 0) { /* does have a left sibling */
        left_sibling = x -> children[i - 1];
        left_n = left_sibling -> n;
      }
      if (left_n >= t) {
        /*
         * Case 3a: x's left sibling has at least t keys.
         * Move a key from x into child, move a key from left_sibling up into x,
         * and move a child pointer from left_sibling into child.
         */
        memmove(child -> keys + 1 * width, child -> keys, child -> n * width);
        memmove(
          child -> key_counts + 1,
          child -> key_counts,
          child -> n * sizeof(int)
        );
        memmove(
          child -> children + 1,
          child -> children,
          (child -> n + 1) * sizeof(struct _BTreeNode*)
        );
        child -> n += 1;
        /* Move a key from x into child */
        memcpy(child -> keys + 0 * width, x -> keys + (i - 1) * width, width);
        child -> key_counts[0] = x -> key_counts[i - 1];
        memcpy(
          x -> keys + (i - 1) * width,
          left_sibling -> keys + (left_n - 1) * width,
          width
        );
        x -> key_counts[i - 1] = left_sibling -> key_counts[left_n - 1];
        /* Move appropriate child pointer from left_sibling into child. */
        if (!child -> is_leaf) {
          child -> children[0] = left_sibling -> children[left_n];
        }
        left_sibling -> n -= 1;
      } else { /* symmetric with right sibling */
        struct _BTreeNode* right_sibling = NULL;
        var right_n = 0;
        if (i < x -> n) {
          right_sibling = x -> children[i + 1];
          right_n = right_sibling -> n;
        }
        if (right_n >= t) {
          /*
           * Case 3a: x's right sibling has at least t keys.
           * Move a key from x into child, move a key from right_sibling up into
           * x, and move a child pointer from right_sibling into child.
           */
          /* Move a key from x into child. */
          memcpy(
            child -> keys + child -> n * width,
            x -> keys + i * width,
            width
          );
          child -> key_counts[child -> n] = x -> key_counts[i];
          /* Move a key from right_sibling up into x. */
          memcpy(
            x -> keys + i * width,
            right_sibling -> keys + 0 * width,
            width
          );
          x -> key_counts[i] = right_sibling -> key_counts[0];
          /* Move appropriate child pointer from right_sibling into child. */
          if (!child -> is_leaf) {
            child -> children[child -> n + 1] = right_sibling -> children[0];
          }
          /*
           * Move all the right sibling's keys and child pointers to the left by
           * one position.
           * Only move right_n - 1 items, since we remove the r_s.c[0].
           */
          memmove(
            right_sibling -> keys,
            right_sibling -> keys + 1 * width,
            (right_n - 1) * width
          );
          memmove(
            right_sibling -> key_counts,
            right_sibling -> key_counts + 1,
            (right_n - 1) * sizeof(int)
          );
          memmove(
            right_sibling -> children,
            right_sibling -> children + 1,
            (right_n + 1 - 1) * sizeof(struct _BTreeNode*)
          );
          right_sibling -> n -= 1;
          child -> n += 1;
        } else {
          /*
           * Case 3b: The child and each of its immediate siblings have t - 1
           * keys.
           * Merge the child with one sibling, including moving a key from x
           * down into the new merged node as the median key of the merged node.
           */
          if (left_n > 0) {
            /*
             * The child has a left sibling. Merge the child with the left
             * sibling.
             */
            /* Move everything in child right by t positions. */
            memmove(
              child -> keys + t * width,
              child -> keys + 0 * width,
              child -> n * width
            );
            memmove(
              child -> key_counts + t,
              child -> key_counts + 0,
              child -> n * sizeof(int)
            );
            if (!child -> is_leaf) {
              memmove(
                child -> children + t,
                child -> children + 0,
                (child -> n + 1) * sizeof(struct _BTreeNode*)
              );
            }
            /* Take everything from left_sibling. */
            memmove(child -> keys, left_sibling -> keys, left_n * width);
            memmove(
              child -> key_counts,
              left_sibling -> key_counts,
              left_n * sizeof(int)
            );
            if (!child -> is_leaf) {
              memmove(
                child -> children,
                left_sibling -> children,
                (left_n + 1) * sizeof(struct _BTreeNode*)
              );
            }
            /* Move k down from node x into child. */
            memcpy(
              child -> keys + (t - 1) * width,
              x -> keys + (i - 1) + width,
              width
            );/* TODO: BUG? */
            child -> key_counts[t - 1] = x -> key_counts[i - 1];
            child -> n += left_n + 1;
            /*
             * Since node x is losing key i - 1 and child pointer i - 1, move
             * keys i to n - 1 and children i to n left by one position.
             *
             * n = 6, i = 2, length = n - i = 6 - 2 = 4
             * 0 1 2 3 4 5
             * A B C D E F
             *     \-----/
             */
            memmove(
              x -> keys + (i - 1) * width,
              x -> keys + i * width,
              (x -> n - i) * width
            );
            memmove(
              x -> key_counts + i - 1,
              x -> key_counts + i,
              (x -> n - i) * sizeof(int)
            );
            memmove(
              x -> children + i - 1,
              x -> children + i,
              (x -> n + 1 - i) * sizeof(struct _BTreeNode*)
            );
            x -> n -= 1;
            _b_tree_node_deinit(left_sibling);
          } else {
            /*
             * Still in case 3b, but the child has no left sibling. Merge the
             * child with the right sibling.
             */
            /* Take everything from right_sibling (keep a hole for k) */
            memmove(
              child -> keys + (child -> n + 1) * width,
              right_sibling -> keys,
              right_n * width
            );
            memmove(
              child -> key_counts + child -> n + 1,
              right_sibling -> key_counts,
              right_n * sizeof(int)
            );
            if (!child -> is_leaf) {
              memmove(
                child -> children + child -> n + 1,
                right_sibling -> children,
                (right_n + 1) * sizeof(struct _BTreeNode*)
              );
            }
            /* Move a key down from node x into the child. */
            memcpy(
              child -> keys + (t - 1) * width,
              x -> keys + i * width,
              width
            );/* TODO: BUG? */
            child -> key_counts[t - 1] = x -> key_counts[i];
            child -> n += right_n + 1;
            /*
             * Since node x is losing key i and child pointer i, move keys i + 1
             * to n - 1 and children i + 2 to n left by one position in this
             * node.
             *     i        n = 6     length = n - 1 - i = 6 - 1 - 2 = 3
             * 0 1 2 3 4 5
             * A B C D E F
             *       \---/
             */
            memmove(
              x -> keys + i * width,
              x -> keys + (i + 1) * width,
              (x -> n - 1 - i) * width
            );
            memmove(
              x -> key_counts + i,
              x -> key_counts + i + 1,
              (x -> n - 1 - i) * sizeof(int)
            );
            if (!x -> is_leaf) {
              memmove(
                x -> children + i + 1,
                x -> children + i + 2,
                (x -> n + 1 - 1 - i) * sizeof(struct _BTreeNode*)
              );
            }
            x -> n -= 1;
            _b_tree_node_deinit(right_sibling);
          }
        }
      }
    }
    _b_tree_remove_from_subtree(old_child, k, t, width, compare);
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
  /* A Boolean value indicating whether a B-tree allows duplicate elements. */
  bool allow_duplicates;
};

/* MARK: - Creating and Destroying a B-Tree */

/* Creates an empty BTree. */
void b_tree_init(
  struct BTree* tree,
  int t,
  int element_size,
  bool allow_duplicates,
  int (*compare)(const void*, const void*)
) {
  (*tree).root = _b_tree_node_init(t, element_size);
  (*tree).t = t;
  (*tree).element_size = element_size;
  (*tree).allow_duplicates = allow_duplicates;
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
    _b_tree_node_deinit(node);
  }
}

/* Destroys a BTree. */
void b_tree_deinit(struct BTree* tree) {
  _b_tree_deinit((*tree).root);
}

/* MARK: - Adding Elements */

/* Adds a new element in the B-Tree. */
bool b_tree_insert(struct BTree* tree, void* key) {
  if (_b_tree_insert(
    &(*tree).root,
    key,
    (*tree).t,
    (*tree).element_size,
    (*tree).allow_duplicates,
    (*tree).compare
  )) {
    (*tree).is_empty = false;
    (*tree).count += 1;
    return true;
  }
  return false;
}

/* MARK: - Finding Elements */

/*
 * Returns a Boolean value indicating whether the tree contains the given
 * element.
 */
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

static struct _BTreeNode* _b_tree_minimum(struct _BTreeNode* x) {
  while (x -> n > 0 && x -> children[0] != NULL) {
    x = x -> children[0];
  }
  return x;
}

/* Returns the element with the smallest value, if available. */
bool b_tree_min(struct BTree* tree, void* result) {
  if ((*tree).is_empty) {
    return false;
  }
  var x = _b_tree_minimum((*tree).root);
  memcpy(result, x -> keys, (*tree).element_size);
  return true;
}

bool b_tree_predecessor(struct BTree* tree, void* key, void* result) {
  var i = 0;
  var key_node = _b_tree_search(
    (*tree).root,
    key,
    (*tree).element_size,
    &i,
    (*tree).compare
  );
  var pred = _b_tree_predecessor(key_node);
  if (pred == key_node) { /* Same leaf node */
    if (i - 1 < 0) {
      return false;
    }
    memcpy(
      result,
      pred -> keys + (i - 1) * (*tree).element_size,
      (*tree).element_size
    );
  } else {
    memcpy(
      result,
      pred -> keys + (pred -> n - 1) * (*tree).element_size,
      (*tree).element_size
    );
  }
  return true;
}

bool b_tree_successor(struct BTree* tree, void* key, void* result) {
  var i = 0;
  var key_node = _b_tree_search(
    (*tree).root,
    key,
    (*tree).element_size,
    &i,
    (*tree).compare
  );
  var succ = _b_tree_successor(key_node);
  if (succ == key_node) {
    if (i + 1 > succ -> n - 1) {
      return false;
    }
    memcpy(
      result,
      succ -> keys + (i + 1) * (*tree).element_size,
      (*tree).element_size
    );
  } else {
    memcpy(
      result,
      succ -> keys,
      (*tree).element_size
    );
  }
  return true;
}

/* MARK: - Removing Elements */

/* Removes the given element in the collection. */
int b_tree_remove(struct BTree* tree, void* key) {
  if ((*tree).is_empty) {
    return 2;
  }

  var i = 0;
  struct _BTreeNode* dup;
  /* delete dupicate element */
  if ((dup = _b_tree_search(
    (*tree).root,
    key,
    (*tree).element_size,
    &i,
    (*tree).compare)) != NULL
  ) {
    dup -> key_counts[i] -= 1;
    if (dup -> key_counts[i] > 1 - 1) {
      return 0;
    }
  } else {
    /* No such key */
    return 2;
  }

  _b_tree_remove_from_subtree(
    (*tree).root,
    key,
    (*tree).t,
    (*tree).element_size,
    (*tree).compare
  );
  /* Update root when root is an internal node with no keys. */
  if (!(*tree).root -> is_leaf && (*tree).root -> n == 0) {
    (*tree).root = (*tree).root -> children[0];
  }

  (*tree).count -= 1;
  if ((*tree).count == 0) {
    (*tree).is_empty = true;
  }
  return 0;
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
