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
  int array[] = {1, 3, 5, 7, 9, 11, 13};
  var key = 5;
  XCTAssertEqual(5, array[binary_search(&key, array, 7, sizeof(int), compare)]);
  /* subarray: 7, 9, 11, 13 */
  XCTAssertEqual(binary_search(&key, array + 3, 4, sizeof(int), compare), -1);
  key = 7;
  XCTAssertEqual(binary_search(&key, array + 3, 4, sizeof(int), compare), 0);
  key = 9;
  XCTAssertEqual(binary_search(&key, array + 3, 4, sizeof(int), compare), 1);
  key = 11;
  XCTAssertEqual(binary_search(&key, array + 3, 4, sizeof(int), compare), 2);
  key = 13;
  XCTAssertEqual(binary_search(&key, array + 3, 4, sizeof(int), compare), 3);
  key = 15;
  XCTAssertEqual(binary_search(&key, array + 3, 4, sizeof(int), compare), -1);
}

// MARK: - b-tree

- (void)test_b_tree {
  struct BTree tree;
  b_tree_init(&tree, 1024, sizeof(int), compare);

  for (var i = 0; i < 65536; i += 1) {
    b_tree_insert(&tree, &i);
  }

//  printf(tree.root);

//  for (var i = 0; i < 65536; i += 1) {
//    XCTAssertTrue(b_tree_contains(&tree, &i));
//  }

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
// static Bool test_insert(void) {
//     Int buf[] = {19358ll, 19358ll, 19358ll, 100ll, 19358ll, 19358ll, 200ll};
//     var delta = 19358ll;
//     var array = array_init3(sizeof(Int), &delta, 5);
//
//     delta = 100ll;
//     array_insert(array, 3, &delta);
//     delta = 200ll;
//     array_insert(array, array -> count, &delta);
//     expect_equal_elements(array -> data, buf, 7, sizeof(Int), int_equal);
//
//     array_deinit(array);
//
//     array = array_init(sizeof(Int));
//     array_insert(array, 0, &delta);
//     expect_equal(array_get(array, 0), &delta, int_equal);
//
//     array_deinit(array);
//
//     array = array_init(sizeof(Int));
//     for (var i = 0; i < 7; i += 1) {
//         array_insert(array, 0, &buf[i]);
//     }
//     for (int i = 6; i >= 0; i -= 1) {
//         expect_equal(&buf[i], array_get(array, 6 - i), int_equal);
//     }
//
//     array_deinit(array);
//     return true;
// }
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
