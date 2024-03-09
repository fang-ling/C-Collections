/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#import <XCTest/XCTest.h>

#import "array.h"
#import "b_tree.h"
#import "binary_heap.h"
#import "deque.h"

#import "binary_search.h"
#import "sort.h"

#define var __auto_type

@interface wkq_tests : XCTestCase

@end

@implementation wkq_tests

- (void)test_array_init {
  struct Array array;
  array_init(&array, sizeof(int));
  
  XCTAssertEqual(array.count, 0);
  XCTAssertEqual(array.element_size, sizeof(int));
  XCTAssertEqual(array.is_empty, true);
  XCTAssertEqual(array.capacity, 0);
  
  XCTAssertEqual(array._storage, NULL);
  
  array_deinit(&array);
}

- (void) test_array_is_empty {
  struct Array array;
  array_init(&array, sizeof(int));
  XCTAssertTrue(array.is_empty);

  var delta = 19358;
  array_append(&array, &delta);
  XCTAssertFalse(array.is_empty);

//  array_remove_lastn(array);
//  expect_true(array -> is_empty);

//     array_insert(array, 0, &delta);
//     expect_false(array -> is_empty);
//
//     array_remove_firstn(array);
//     expect_true(array -> is_empty);
//
//     array_deinit(array);
//
//     array = array_init2(sizeof(Int), delta);
//     expect_false(array -> is_empty);
//
//     array_deinit(array);
//
//     array = array_init3(sizeof(Int), &delta, delta);
//     expect_false(array -> is_empty);
//
  array_deinit(&array);
}

- (void)test_array_count {
  struct Array array;
  array_init(&array, sizeof(int));
  
  XCTAssertEqual(array.count, 0);
  
  var delta = 19358;
  array_append(&array, &delta);
  XCTAssertEqual(array.count, 1);
  //     array_insert(array, 0, &delta);
  //     delta = 2ll;
  //     expect_equal(&delta, &array -> count, int_equal);
  //
  //     free(array_remove_first(array));
  //     delta = 1ll;
  //     expect_equal(&delta, &array -> count, int_equal);
  //
  //     array_remove_lastn(array);
  //     delta = 0ll;
  //     expect_equal(&delta, &array -> count, int_equal);
  //
  array_deinit(&array);
}

- (void)test_array_get {
  struct Array array;
  array_init(&array, sizeof(int));
  
  int input[] = {19358, 12333, 19348, 19306, 19306};
  for (var i = 0; i < 5; i += 1) {
    array_append(&array, &input[i]);
  }
  for (var i = 0; i < 5; i += 1) {
    var delta = 0;
    array_get(&array, i, &delta);
    XCTAssertEqual(input[i], delta);
  }
  
  array_deinit(&array);
}

- (void)test_array_set {
  struct Array array;
  array_init(&array, sizeof(int));
  
  for (var i = 0; i < 5; i += 1) {
    array_append(&array, &i);
  }
  XCTAssertEqual(array.capacity, 8);
  
  int input[] = {19358, 12333, 19348, 19306, 19306};
  
  for (var i = 0; i < 5; i += 1) {
    array_set(&array, i, &input[i]);
  }
  
  var delta = 0;
  for (var i = 0; i < 5; i += 1) {
    array_get(&array, i, &delta);
    XCTAssertEqual(input[i], delta);
  }
  
  array_deinit(&array);
}

- (void)test_array_sort {
  struct Array array;
  array_init(&array, sizeof(int));
  struct Array result;
  array_init(&result, sizeof(int));

  for (var i = 0; i < 19358; i += 1) {
    var delta = arc4random();
    array_append(&array, &delta);
    array_append(&result, &delta);
  }
  array_sort(&array, compare);
  qsort(result._storage, 19358, sizeof(int), compare);
  XCTAssertEqual(
    0, 
    memcmp(result._storage, array._storage, 19358 * sizeof(int))
  );

  array_deinit(&array);
}

bool int_equal(const void* a, const void* b) {
  return *(int*)a == *(int*)b;
}

- (void)test_array_contains {
  struct Array array;
  array_init(&array, sizeof(int));

  var val = 58;
  XCTAssertFalse(array_contains(&array, &val, int_equal));

  int expenses[] = {19358, 12333, 19348, 19306, 19306, 58};
  for (var i = 0; i < 6; i += 1) {
    array_append(&array, &expenses[i]);
  }
  XCTAssertTrue(array_contains(&array, &val, int_equal));
  val = 12333;
  XCTAssertTrue(array_contains(&array, &val, int_equal));

  array_deinit(&array);
}

- (void)test_array_first_index {
  struct Array array;
  array_init(&array, sizeof(int));

  var val = 12333;
  XCTAssertEqual(array_first_index(&array, &val, int_equal), -1);

  int input[] = {19358, 12333, 19348, 19306, 19306, 58};
  for (var i = 0; i < 6; i += 1) {
    array_append(&array, &input[i]);
  }
  XCTAssertEqual(array_first_index(&array, &val, int_equal), 1);

  array_deinit(&array);
}

- (void)test_remove_last {
  int input[] = {12333, 19358, 19348, 12321, 19348, 19333};

  struct Array array;
  array_init(&array, sizeof(int));
  /* expect fatal error */
  XCTAssertEqual(array_remove_last(&array), 6);

  for (var i = 0; i < 6; i += 1) {
     array_append(&array, &input[i]);
  }
  /* remove last (19333) */
  array_remove_last(&array);
  var delta = 0;
  array_get(&array, array.count - 1, &delta);
  XCTAssertEqual(delta, 19348);

  /* remove last (19348) */
  array_remove_last(&array);
  array_get(&array, array.count - 1, &delta);
  XCTAssertEqual(delta, 12321);

  for (var i = 0; i < 6; i += 1) {
     array_append(&array, &input[i]);
  }

  /* remove the rest */
  while (!array.is_empty) {
     array_remove_last(&array);
  }

  XCTAssertEqual(array.count, 0);
  XCTAssertEqual(array.capacity, 1);
  XCTAssertEqual(array.is_empty, true);

  array_deinit(&array);
}

- (void)test_array_equal {
  struct Array lhs;
  struct Array rhs;
  array_init(&lhs, sizeof(int));
  array_init(&rhs, sizeof(int));

  for (var i = 0; i < 100; i += 1) {
    array_append(&lhs, &i);
    array_append(&rhs, &i);
  }
  XCTAssertTrue(array_equal(&lhs, &rhs));

  ((int*)lhs._storage)[58] = 19358;
  XCTAssertFalse(array_equal(&lhs, &rhs));

  array_deinit(&lhs);
  array_deinit(&rhs);
}

- (void)test_array_swap_at {
  struct Array lhs;
  struct Array rhs;
  array_init(&lhs, sizeof(int));
  array_init(&rhs, sizeof(int));

  for (var i = 0; i < 100; i += 1) {
    array_append(&lhs, &i);
    array_append(&rhs, &i);
  }

  ((int*)lhs._storage)[0] = 1;
  ((int*)lhs._storage)[1] = 0;
  array_swap_at(&rhs, 0, 1);

  XCTAssertTrue(array_equal(&lhs, &rhs));

  array_deinit(&lhs);
  array_deinit(&rhs);
}

- (void)test_insert {
  int buf[] = {19358, 19358, 19358, 100, 19358, 19358, 200};
  var delta = 19358;
  struct Array array;
  array_init(&array, sizeof(int));
  for (var i = 0; i < 5; i += 1) {
    array_append(&array, &delta);
  }

  delta = 100ll;
  array_insert(&array, &delta, 3);
  delta = 200ll;
  array_insert(&array, &delta, array.count);
  for (var i = 0; i < array.count; i += 1) {
    XCTAssertEqual(((int*)array._storage)[i], buf[i]);
  }

  array_remove_all(&array);

  array_insert(&array, &delta, 0);
  XCTAssertEqual(((int*)array._storage)[0], delta);

  array_remove_all(&array);

  for (var i = 0; i < 7; i += 1) {
    array_insert(&array, &buf[i], 0);
  }
  for (int i = array.count - 1; i >= 0; i -= 1) {
    XCTAssertEqual(((int*)array._storage)[array.count - 1 - i], buf[i]);
  }

  array_deinit(&array);
}

- (void)test_array_remove_at {
  struct Array array;
  array_init(&array, sizeof(double));

  double buf[] = {1.1, 1.5, 2.9, 1.2, 1.5, 1.3, 1.2};
  for (var i = 0; i < 7; i += 1) {
    array_append(&array, &buf[i]);
  }

  double buf2[] = {1.1, 1.5, 1.2, 1.5, 1.3, 1.2};
  array_remove_at(&array, 2);
  for (var i = 0; i < array.count; i += 1) {
    XCTAssertEqual(((double*)array._storage)[i], buf2[i]);
  }

  double buf3[] = {1.1, 1.5, 1.2, 1.5, 1.2};
  array_remove_at(&array, 4);
  for (var i = 0; i < array.count; i += 1) {
    XCTAssertEqual(((double*)array._storage)[i], buf3[i]);
  }

  double buf4[] = {1.1, 1.5, 1.2, 1.5};
  array_remove_at(&array, 4);
  for (var i = 0; i < array.count; i += 1) {
    XCTAssertEqual(((double*)array._storage)[i], buf4[i]);
  }

  double buf5[] = {1.5, 1.2, 1.5, 1.2};
  array_remove_at(&array, 0);
  for (var i = 0; i < array.count; i += 1) {
    XCTAssertEqual(((double*)array._storage)[i], buf5[i]);
  }

  for (var _ = 0; _ < 4; _ += 1) {
    array_remove_at(&array, 0);
  }
  XCTAssertEqual(array.count, 0);
  XCTAssertTrue(array.is_empty);

  array_deinit(&array);
}

// MARK: - sort

int compare(const void* a, const void* b) {
  if (*(int*)a > *(int*)b) {
    return 1;
  } else if (*(int*)a < *(int*)b) {
    return -1;
  }
  return 0;
}

- (void)test_sort {
  int array[] = {/*19342, 19358, 3, 2, -1, 20, 7*/19358, 19342, 20, 7, 3, 2, -1};
  sort(array, 7, sizeof(int), compare);

  int result[] = {-1, 2, 3, 7, 20, 19342, 19358};
  for (var i = 0; i < 7; i += 1) {
    XCTAssertEqual(array[i], result[i]);
  }
}

// MARK: - BinaryHeap

- (void)test_binary_heap_remove {
  struct BinaryHeap heap;
  binary_heap_init(&heap, sizeof(int), compare);
  
  var x = 19358;
  binary_heap_insert(&heap, &x);
  var ret = 0;
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 19358);
  
  x = 12;
  binary_heap_insert(&heap, &x);
  x = 9;
  binary_heap_insert(&heap, &x);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 12);
  
  x = 13;
  binary_heap_insert(&heap, &x);
  x = 1;
  binary_heap_insert(&heap, &x);
  x = 4;
  binary_heap_insert(&heap, &x);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 13);
  
  int input[] = {1, 3, 4, 2, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  for (var i = 0; i < 20; i += 1) {
    binary_heap_insert(&heap, &input[i]);
  }
  
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 20);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 19);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 18);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 17);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 16);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 15);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 14);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 13);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 12);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 11);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 10);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 9);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  /* One 9 was still in the heap from before */
  XCTAssertEqual(ret, 9);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 8);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 7);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 6);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 5);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 4);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  /* One 4 was still in the heap from before */
  XCTAssertEqual(ret, 4);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 3);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 2);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  XCTAssertEqual(ret, 1);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  /* One 1 was still in the heap from before */
  XCTAssertEqual(ret, 1);
  binary_heap_max(&heap, &ret);
  binary_heap_remove_max(&heap);
  
  XCTAssertEqual(heap.count, 0);
  
  binary_heap_deinit(&heap);
}

- (void)test_binary_heap_is_empty {
  struct BinaryHeap heap;
  binary_heap_init(&heap, sizeof(int), compare);
  
  binary_heap_deinit(&heap);
}

// MARK: - deque

- (void)test_deque_adding_elem {
  struct Deque deque;
  deque_init(&deque, sizeof(int));

  var delta = 19358;
  deque_append(&deque, &delta);
  var result = 0;
  deque_get(&deque, 0, &result);
  XCTAssertEqual(result, delta);

  delta = 12333;
  deque_prepend(&deque, &delta);
  deque_get(&deque, 0, &result);
  XCTAssertEqual(result, delta);

  deque_remove_first(&deque);
  deque_remove_first(&deque);

  /* 0, 1, 2, 3, 4 */
  for (var i = 0; i < 5; i += 1) {
    deque_append(&deque, &i);
  }

  int o[] = {18, 17, 100, 15, 14, 13, 12, 11, 10, 0, 1, 2, 3, 4};
  for (var i = 10; i < 19; i += 1) {
    deque_prepend(&deque, &i);
  }

  delta = 100;
  deque_set(&deque, 2, &delta);

  for (var i = 0; i < 14; i += 1) {
    deque_get(&deque, i, &result);
    XCTAssertEqual(result, o[i]);
  }

  deque_deinit(&deque);
}

- (void)test_deque_is_empty {
  struct Deque deque;
  deque_init(&deque, sizeof(int));

  XCTAssertTrue(deque.is_empty);

  var a = 0;
  deque_append(&deque, &a);
  XCTAssertFalse(deque.is_empty);

  deque_prepend(&deque, &a);
  XCTAssertFalse(deque.is_empty);

  deque_remove_last(&deque);
  XCTAssertFalse(deque.is_empty);

  deque_remove_first(&deque);
  XCTAssertTrue(deque.is_empty);

  deque_deinit(&deque);
}

- (void)test_deque_reblance {
  struct Deque deque;
  deque_init(&deque, sizeof(int));

  for (var i = 0; i < 128; i += 1) {
    deque_append(&deque, &i);
  }
  XCTAssertEqual(deque._head.count, 0);
  XCTAssertEqual(deque._tail.count, 128);
  XCTAssertEqual(deque.count, 128);

  deque_remove_first(&deque); /* Causes reblance */
  /* half_count = 127 / 2 = 63 */
  XCTAssertEqual(deque._head.count, 63);
  XCTAssertEqual(deque._tail.count, 64);
  XCTAssertEqual(deque.count, 127);

  for (var i = 0; i < 64; i += 1) {
    deque_remove_last(&deque);
  }
  XCTAssertEqual(deque.count, 63);
  XCTAssertEqual(deque._tail.count, 0);
  deque_remove_last(&deque); /* Causes reblance */
  /* half_count = 63 / 2 = 31 */
  XCTAssertEqual(deque._head.count, 63 - 31);
  XCTAssertEqual(deque._tail.count, 31 - 1);
  XCTAssertEqual(deque.count, 62);

  for (var i = 0; i < 62; i += 1) {
    deque_remove_first(&deque);
  }
  XCTAssertEqual(deque.count, 0);

  XCTAssertEqual(deque_remove_last(&deque), 6);
  XCTAssertEqual(deque_remove_first(&deque), 6);

  var a = 0;
  deque_append(&deque, &a);
  deque_append(&deque, &a);
  deque_remove_first(&deque);
  XCTAssertEqual(deque.count, 1);
  XCTAssertEqual(deque._head.count, 0);
  XCTAssertEqual(deque._tail.count, 1);

  deque_deinit(&deque);
}

- (void)test_first {
  struct Deque deque;
  deque_init(&deque, sizeof(int));

  /* 0, 1, 2, 3, 4 */
  for (var i = 0; i < 5; i += 1) {
    deque_append(&deque, &i);
  }
  var a = 99;
  deque_get(&deque, 0, &a);
  XCTAssertEqual(a, 0);

  /* 7, 6, 5, 0, 1, 2, 3, 4 */
  for (var i = 5; i < 8; i += 1) {
    deque_prepend(&deque, &i);
  }
  deque_get(&deque, 0, &a);
  XCTAssertEqual(a, 7);

  deque_remove_first(&deque); /* 6, 5, 0, 1, 2, 3, 4 */
  deque_get(&deque, 0, &a);
  XCTAssertEqual(a, 6);

  deque_remove_first(&deque); /* 5, 0, 1, 2, 3, 4 */
  deque_get(&deque, 0, &a);
  XCTAssertEqual(a, 5);
  for (var i = 0; i < deque.count; i += 1) {
    deque_get(&deque, i, &a);
    printf("%d ", a);
  }
  printf("\n");

  deque_remove_first(&deque); /* 0, 1, 2, 3, 4 */
  deque_get(&deque, 0, &a);
  XCTAssertEqual(a, 0);
  XCTAssertEqual(deque._head.count, 0);
  XCTAssertEqual(deque._tail.count, 5);

  for (var i = 0; i < deque.count; i += 1) {
    deque_get(&deque, i, &a);
    printf("%d ", a);
  }
  printf("\n");

  deque_remove_first(&deque); /* 1, 2, 3, 4 */
  deque_get(&deque, 0, &a);
  XCTAssertEqual(a, 1);
  XCTAssertEqual(deque._head.count, 1);
  XCTAssertEqual(deque._tail.count, 3);

  for (var i = 0; i < deque.count; i += 1) {
    deque_get(&deque, i, &a);
    printf("%d ", a);
  }
  printf("\n");

  deque_deinit(&deque);
}

// MARK: - binary_search

- (void)test_binary_search {
  int array[] = {12333, 12361, 19342, 19348, 19358};
  for (var i = 0; i < 5; i += 1) {
    XCTAssertTrue(binary_search(&array[i], array, 5, sizeof(int), compare));
  }
  var key = 58;
  XCTAssertFalse(binary_search(&key, array, 5, sizeof(int), compare));
  key = 0x7fffff;
  XCTAssertFalse(binary_search(&key, array, 5, sizeof(int), compare));
}

- (void)test_lower_bound {
  int array[] = {1, 2, 4, 5, 5, 6};
  /* 1 ≤ 1 at index 0 */
  XCTAssertEqual(0, lower_bound(&array[0], array, 6, sizeof(int), compare));
  /* 2 ≤ 2 at index 1 */
  XCTAssertEqual(1, lower_bound(&array[1], array, 6, sizeof(int), compare));
  var key = 5;
  /* 5 ≤ 5 at index 3 */
  XCTAssertEqual(3, lower_bound(&key, array, 6, sizeof(int), compare));
  key = 3;
  /* 3 ≤ 4 at index 2 */
  XCTAssertEqual(2, lower_bound(&key, array, 6, sizeof(int), compare));
  key = 19358;
  /* 19358 not found */
  XCTAssertEqual(6, lower_bound(&key, array, 6, sizeof(int), compare));
  key = 7;
  /* 7 not found */
  XCTAssertEqual(6, lower_bound(&key, array, 6, sizeof(int), compare));
  key = 0;
  /* 0 ≤ 1 at index 0 */
  XCTAssertEqual(0, lower_bound(&key, array, 6, sizeof(int), compare));
}

// MARK: - b-tree

int char_compare(const void* a, const void* b) {
  if (*(char*)a > *(char*)b) {
    return 1;
  } else if (*(char*)a < *(char*)b) {
    return -1;
  }
  return 0;
}

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

/* This test focus on tree structure */
- (void)test_b_tree {

  struct BTree tree;
  b_tree_init(&tree, 2, sizeof(char), true, char_compare); /* 2-3-4 Tree */

  /* Insert F */
  var c = (char)'F';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(*(char*)tree.root->keys, 'F');
  XCTAssertEqual(tree.root->children[0], NULL);

  /* Insert S */
  c = 'S';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'S');
  XCTAssertEqual(tree.root->children[0], NULL);

  /* Insert Q */
  c = 'Q';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'Q');
  XCTAssertEqual(((char*)tree.root->keys)[2], 'S');
  XCTAssertEqual(tree.root->children[0], NULL);

  /* Insert K (node split) */
  c = 'K';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(tree.root->n, 1);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'Q');
  XCTAssertEqual(tree.root->children[0]->n, 2);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[1], 'K');
  XCTAssertEqual(tree.root->children[1]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'S');

  /* Insert C */
  c = 'C';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(tree.root->n, 1);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'Q');
  XCTAssertEqual(tree.root->children[0]->n, 3);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'C');
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[1], 'F');
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[2], 'K');
  XCTAssertEqual(tree.root->children[1]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'S');

  /* Insert L */
  c = 'L';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(tree.root->n, 2);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'Q');
  XCTAssertEqual(tree.root->children[0]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'C');
  XCTAssertEqual(tree.root->children[1]->n, 2);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'K');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[1], 'L');
  XCTAssertEqual(tree.root->children[2]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[0], 'S');

  /* Insert H */
  c = 'H';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'Q');
  XCTAssertEqual(tree.root->children[0]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'C');
  XCTAssertEqual(tree.root->children[1]->n, 3);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'H');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[1], 'K');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[2], 'L');
  XCTAssertEqual(tree.root->children[2]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[0], 'S');

  /* Insert T */
  c = 'T';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'Q');
  XCTAssertEqual(tree.root->children[0]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'C');
  XCTAssertEqual(tree.root->children[1]->n, 3);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'H');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[1], 'K');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[2], 'L');
  XCTAssertEqual(tree.root->children[2]->n, 2);
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[0], 'S');
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[1], 'T');

  /* Insert V */
  c = 'V';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(tree.root->n, 2);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'Q');
  XCTAssertEqual(tree.root->children[0]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'C');
  XCTAssertEqual(tree.root->children[1]->n, 3);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'H');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[1], 'K');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[2], 'L');
  XCTAssertEqual(tree.root->children[2]->n, 3);
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[0], 'S');
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[1], 'T');
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[2], 'V');

  /* Insert W */
  c = 'W';
  b_tree_insert(&tree, &c);
  XCTAssertEqual(tree.root->n, 3);
  XCTAssertEqual(((char*)tree.root->keys)[0], 'F');
  XCTAssertEqual(((char*)tree.root->keys)[1], 'Q');
  XCTAssertEqual(((char*)tree.root->keys)[2], 'T');
  XCTAssertEqual(tree.root->children[0]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[0]->keys)[0], 'C');
  XCTAssertEqual(tree.root->children[1]->n, 3);
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[0], 'H');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[1], 'K');
  XCTAssertEqual(((char*)tree.root->children[1]->keys)[2], 'L');
  XCTAssertEqual(tree.root->children[2]->n, 1);
  XCTAssertEqual(((char*)tree.root->children[2]->keys)[0], 'S');
  XCTAssertEqual(tree.root->children[3]->n, 2);
  XCTAssertEqual(((char*)tree.root->children[3]->keys)[0], 'V');
  XCTAssertEqual(((char*)tree.root->children[3]->keys)[1], 'W');

  b_tree_deinit(&tree);
}

-(void) test_b_tree2 {
  struct BTree tree;
  b_tree_init(&tree, 2, sizeof(char), true, char_compare);

  char keys[] = "FSQKCLHTVWMRNPABXYDZE";
  var count = strlen(keys);

  for (var _ = 0; _ < 5; _ += 1) {
    for (var i = 0; i < count; i += 1) {
      b_tree_insert(&tree, &keys[i]);
    }
  }
  b_tree_insert(&tree, &keys[count - 1]); /* Insert E */
  b_tree_insert(&tree, &keys[7]); /* Insert T */
  b_tree_insert(&tree, &keys[16]); /* Insert X */
  /*
   *                    KQ
   *        BF           M            TW
   *     A CDE H     L      NP     RS  V  XYZ
   */
  XCTAssertEqual(memcmp(tree.root->keys, "KQ", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->keys, "BF", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->keys, "M", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->keys, "TW", 2 * sizeof(char)), 0);
  XCTAssertEqual(((int*)tree.root->children[2]->key_counts)[0], 6);
  for (var i = 1; i < tree.root->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[0]->keys, "A", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[0]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[1]->keys, "CDE", 3 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[1]->n-1; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[1]->key_counts)[i], 5);
  }
  XCTAssertEqual(((int*)tree.root->children[0]->children[1]->key_counts)[2], 6);

  XCTAssertEqual(memcmp(tree.root->children[0]->children[2]->keys, "H", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[2]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[0]->keys, "L", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[0]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[1]->keys, "NP", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[1]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[0]->keys, "RS", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[0]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[1]->keys, "V", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[1]->key_counts)[i], 5);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[2]->keys, "XYZ", 3 * sizeof(char)), 0);
  for (var i = 1; i < tree.root->children[2]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[2]->key_counts)[i], 5);
  }
  XCTAssertEqual(((int*)tree.root->children[2]->children[2]->key_counts)[0], 6);

  for (var _ = 0; _ < 4; _ += 1) {
    for (var i = 0; i < count; i += 1) {
      b_tree_remove(&tree, &keys[i]);
    }
  }
  b_tree_remove(&tree, &keys[count - 1]); /* Remove E */
  b_tree_remove(&tree, &keys[7]); /* Remove T */
  b_tree_remove(&tree, &keys[16]); /* Remove X */

  /* Remove Z */
  /*
   *                    KQ
   *        BF           M            TW
   *     A CDE H     L      NP     RS  V  XY
   */
  char key = 'Z';
  b_tree_remove(&tree, &key);
  XCTAssertEqual(memcmp(tree.root->keys, "KQ", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->keys, "BF", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->keys, "M", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->keys, "TW", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[0]->keys, "A", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[1]->keys, "CDE", 3 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[2]->keys, "H", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[0]->keys, "L", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[1]->keys, "NP", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[0]->keys, "RS", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[1]->keys, "V", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[2]->keys, "XY", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[2]->key_counts)[i], 1);
  }

  /* Remove K */
  /*
   *                    HQ
   *        BE          M            TW
   *     A CD  F     L     NP     RS  V  XY
   */
  key = 'K';
  b_tree_remove(&tree, &key);
  XCTAssertEqual(memcmp(tree.root->keys, "HQ", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->keys, "BE", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->keys, "M", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->keys, "TW", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[0]->keys, "A", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[1]->keys, "CD", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[2]->keys, "F", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[0]->keys, "L", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[1]->keys, "NP", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[0]->keys, "RS", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[1]->keys, "V", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[2]->keys, "XY", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[2]->key_counts)[i], 1);
  }

  /* Remove A */
  /*
   *                    HQ
   *        CE          M            TW
   *     B  D  F     L     NP     RS  V  XY
   */
  key = 'A';
  b_tree_remove(&tree, &key);
  XCTAssertEqual(memcmp(tree.root->keys, "HQ", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->keys, "CE", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->keys, "M", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->keys, "TW", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[0]->keys, "B", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[1]->keys, "D", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[2]->keys, "F", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[0]->keys, "L", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[1]->keys, "NP", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[0]->keys, "RS", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[1]->keys, "V", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[2]->keys, "XY", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[2]->key_counts)[i], 1);
  }

  /* Remove M */
  /*
   *                    EQ
   *        C           HN            TW
   *     B    D      F  L   P     RS  V  XY
   */
  key = 'M';
  b_tree_remove(&tree, &key);
  XCTAssertEqual(memcmp(tree.root->keys, "EQ", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->keys, "C", tree.root->children[0]->n * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->keys, "HN", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->keys, "TW", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[0]->keys, "B", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[0]->children[1]->keys, "D", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[0]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[0]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[0]->keys, "F", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[1]->keys, "L", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[1]->children[2]->keys, "P", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[1]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[1]->children[2]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[0]->keys, "RS", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[0]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[0]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[1]->keys, "V", 1 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[1]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[1]->key_counts)[i], 1);
  }

  XCTAssertEqual(memcmp(tree.root->children[2]->children[2]->keys, "XY", 2 * sizeof(char)), 0);
  for (var i = 0; i < tree.root->children[2]->children[2]->n; i += 1) {
    XCTAssertEqual(((int*)tree.root->children[2]->children[2]->key_counts)[i], 1);
  }
  /*
   *                    F R
   *        C            N            TW
   *     B    D        HL   P      S  V  XY
   *
   *                    R
   *         FN                  TW
   *     BD  HL   P            S  V  XY
   *
   *                    R
   *         FN                  TW
   *     BD  L   P            S  V  XY
   *
   *                    R
   *         F                  TW
   *      BD   LP            S  V  XY
   *
   *                    R
   *         F                  W
   *      BD   LP            SV  XY
   *
   *               FRV
   *      BD   LP        S  XY
   *
   *               FRV
   *      D   LP        S  XY
   *
   *               RV
   *         LP        S  XY
   *
   *               RV
   *         P        S  XY
   *
   *               SX
   *         R      V     Y
   *
   *               V
   *         S          XY
   *
   *               X
   *             V   Y
   *
   *             VY
   *
   *             V
   *
   *             ø
   */
  char keys2[] = "EQCHNTBDFLPRSXYV";
  for (var i = 0; i < strlen(keys2); i += 1) {
    b_tree_remove(&tree, &keys2[i]);
  }
  key = 'W';
  b_tree_remove(&tree, &key);
  
  b_tree_deinit(&tree);
}

@end

//
// static Bool test_first(void) {
//     var array = array_init(sizeof(Int));
//     expect_null(array_first(array));
//
//     Int input[] = {19358ll, 12333ll, 19348ll, 19306ll, 19306ll};
//     for (var i = 0; i < 5; i += 1) {
//         array_insert(array, 0, &input[i]);
//         expect_equal(&input[i], array_first(array), int_equal);
//     }
//     for (var i = 5 - 1; i >= 0; i -= 1) { /* The order is reversed */
//         expect_equal(&input[i], array_first(array), int_equal);
//         free(array_remove_first(array));
//     }
//
//     array_deinit(array);
//     return true;
// }
//
// static Bool test_last(void) {
//     var array = array_init(sizeof(Int));
//     expect_null(array_last(array));
//
//     Int input[] = {19358ll, 12333ll, 19348ll, 19306ll, 19306ll};
//     for (var i = 5 - 1; i >= 0; i -= 1) {
//         array_append(array, &input[i]);
//         expect_equal(&input[i], array_last(array), int_equal);
//     }
//     for (var i = 0; i < 5; i += 1) {
//         expect_equal(&input[i], array_last(array), int_equal);
//         array_remove_lastn(array);
//     }
//
//     array_deinit(array);
//     return true;
// }
//
// static Bool test_append(void) {
//     var array = array_init(sizeof(struct Task));
//
//     var delta = 0ll;
//     expect_equal(&delta, &array -> count, int_equal);
//
//     struct Task alpha;
//     alpha.name = "Alice";
//     alpha.priority = 12333;
//     array_append(array, &alpha);
//
//     delta = 1ll;
//     expect_equal(&delta, &array -> count, int_equal);
//
//     array_deinit(array);
//     return true;
// }
//
//
// static Bool test_append_big(void) {
//     var c = 128ll;
//     Int input[c];
//     for (var i = 0ll;i < c; i += 1) {
//         input[i] = i;
//     }
//
//     var array = array_init(sizeof(Int));
//     var i = 0ll;
//     while (i < c) {
//         expect_equal(&array -> count, &i, int_equal);
//         array_append(array, &input[i]);
//         i += 1;
//         expect_equal(&array -> count, &i, int_equal);
//     }
//     expect_equal_elements(input, array -> data, c, sizeof(Int), int_equal);
//
//     array_deinit(array);
//     return true;
// }
//
// static Bool test_append2(void) {
//     var c = 128ll;
//     Int input[c * 2];
//     var delta = 12333ll;
//     var alpha = 19358ll;
//     for (var i = 0; i < c; i += 1) {
//         input[i] = delta;
//     }
//     for (var i = 0; i < c; i += 1) {
//         input[i + c] = alpha;
//     }
//
//     var empty1 = array_init(sizeof(Int));
//     var array = array_init3(sizeof(Int), &delta, c);
//     var rhs = array_init3(sizeof(Int), &alpha, c);
//     var empty2 = array_init(sizeof(Int));
//
//     var beta = 0ll;
//     array_append2(empty1, empty2);
//     expect_equal(&empty1 -> count, &beta, int_equal);
//
//     array_append2(empty1, array);
//     expect_equal(&empty1 -> count, &c, int_equal);
//     expect_equal_elements(empty1 -> data, input, c, sizeof(Int), int_equal);
//
//     array_append2(empty1, rhs);
//     c *= 2;
//     expect_equal(&empty1 -> count, &c, int_equal);
//     expect_equal_elements(empty1 -> data, input, c, sizeof(Int), int_equal);
//
//     array_append2(empty1, empty2);
//     expect_equal(&empty1 -> count, &c, int_equal);
//     expect_equal_elements(empty1 -> data, input, c, sizeof(Int), int_equal);
//
//     return true;
// }
//
//
// static Bool test_swap_at(void) {
//     Int input[]    = {12333ll, 19358ll, 19348ll, 12321ll, 19348ll, 19333ll};
//     Int reversed[] = {19333ll, 19348ll, 12321ll, 19348ll, 19358ll, 12333ll};
//
//     var array = array_init(sizeof(Int));
//     for (var i = 0; i < 6; i += 1) {
//         array_append(array, &input[i]);
//     }
//
//     for (var i = 0; i < 3; i += 1) {
//         array_swap_at(array, i, 6 - i - 1);
//     }
//     expect_equal_elements(array -> data, reversed, 6, sizeof(Int), int_equal);
//
//     array_deinit(array);
//     return true;
// }
