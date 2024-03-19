## C Collections

**C Collections** is an open-source package of data structure implementations and algorithms for ANSI C. It's primarily designed for competitive programming, but we strive to make it generic and ensure memory safety. We also aim to minimize the use of third-party dependencies.

Tested under:

- UVa Online Judge: 5.3.0 - GNU C Compiler with options: `-lm -lcrypt -O2 -pipe -ansi -DONLINE_JUDGE`
- Local Xcode Workplace: 15.0.0 - Apple clang with customized option: `-ansi`

Objective-C codes are only for Xcode unit tests (`xctest`).

## Contents

- `Array` [`v1.6`] An ordered, random-access collection.
- `BinaryHeap` [`v1.0`] A complete binary tree which satisfies the heap ordering property. It provides constant time lookup of the largest (by default) element, at the expense of logarithmic insertion and extraction.
- `BTree` [`v1.0-beta`] An efficient in-memory B-tree implementation, suitable for use as a bag, a set, or a dictionary.
- `Deque` [`v1.1`] A double-ended queue backed by a ring buffer. Deques are random-access collections that allows fast insertion and deletion at both its beginning and its end.
- `RedBlackTree` [`v1.0`] A self-balancing binary search tree, serving as an alternative to B-trees, suitable for use as a bag, a set, or a dictionary.

- `binary_search()` [`v1.1`] An efficient algorithm used to quickly locate a specific target value within a sorted collection.
- `sort()` [`v1.0`] Randomized quicksort with insertion sort on small arrays and optimized for sorted and reverse-sorted arrays.

## Usage

```c
#define var __auto_type

int compare(const void* a, const void* b) {
  var _a = *(int*)a;
  var _b = *(int*)b;
  
  if (_a < _b) {
    return -1;
  } else if (_a > _b) {
    return 1;
  }
  return 0;
}

int main(int argc, const char * argv[]) {
  struct RedBlackTree tree;
  red_black_tree_init(&tree, sizeof(int), true, compare);
  
  int keys[] = {1, 2, 3, 19358, 5, 6, -12321};
  int count = 7;
  while (count--) {
    red_black_tree_insert(&tree, &keys[count]);
  }
  
  var result = 0;
  red_black_tree_select(&tree, 1, &result);
  printf("%d\n", result); /* Prints -12321 */
  
  var key = 4;
  red_black_tree_predecessor(&tree, &key, &result);
  printf("%d\n", result); /* Prints 3 */
  
  key = 7;
  red_black_tree_successor(&tree, &key, &result);
  printf("%d\n", result); /* Prints 19358 */
  
  key = 1;
  printf("%d\n", red_black_tree_rank(&tree, &key)); /* Prints 2 */
  
  red_black_tree_max(&tree, &result);
  red_black_tree_remove(&tree, &result); /* Removes the largest key 19358 */
  red_black_tree_max(&tree, &result);
  printf("%d\n", result); /* Prints 6 */
  
  if (!tree.is_empty) {
    printf("%d\n", tree.count); /* Prints 6 */
  }
  
  red_black_tree_deinit(&tree);
  
  return 0;
}
```
