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
// static Bool test_remove(void) {
//     Int input[] = {12333ll, 19358ll, 19348ll, 12321ll, 19348ll, 19333ll};
//
//     var array = array_init(sizeof(Int));
//     /* expect fatal error */
//     //array_remove_lastn(array);
//     //free(array_remove_last(array));
//     //array_remove_firstn(array);
//     //free(array_remove_first(array));
//
//     for (var i = 0; i < 6; i += 1) {
//         array_append(array, &input[i]);
//     }
//     /* remove 19348 */
//     var alpha = 19348ll;
//     var delta = array_remove(array, 2); /* void* */
//     expect_equal(&alpha, delta, int_equal);
//     free(delta);
//     alpha = 5;
//     expect_equal(&alpha, &array -> count, int_equal);
//
//     /* remove first (12333) */
//     alpha = 12333ll;
//     delta = array_remove_first(array);
//     expect_equal(&alpha, delta, int_equal);
//     free(delta);
//     alpha = 4;
//     expect_equal(&alpha, &array -> count, int_equal);
//
//     /* remove last (19333) */
//     alpha = 19333ll;
//     delta = array_remove_last(array);
//     expect_equal(&alpha, delta, int_equal);
//     free(delta);
//     alpha = 3;
//     expect_equal(&alpha, &array -> count, int_equal);
//
//     /* remove first but not return */
//     array_remove_firstn(array);
//     alpha = 2;
//     expect_equal(&alpha, &array -> count, int_equal);
//
//     /* remove the rest */
//     while (!array -> is_empty) {
//         array_remove_lastn(array);
//         alpha -= 1;
//         expect_equal(&alpha, &array -> count, int_equal);
//     }
//
//     array_deinit(array);
//     return true;
// }
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
