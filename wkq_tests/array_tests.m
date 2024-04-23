/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2024 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#import <XCTest/XCTest.h>

#import "array.h"

#define var __auto_type

@interface ArrayTests : XCTestCase

@end

@implementation ArrayTests

- (void) test_init {
  var array = array_init(sizeof(int));
  
  XCTAssertEqual(array->count, 0);
  XCTAssertEqual(array->_width, sizeof(int));
  XCTAssertEqual(array->is_empty, true);
  XCTAssertEqual(array->_capacity, 0);
  
  XCTAssertEqual(array->_storage, NULL);
  
  array_deinit(array);
}

- (void) test_is_empty {
  var array = array_init(sizeof(int));
  XCTAssertTrue(array->is_empty);

  var delta = 19358;
  array_append(array, &delta);
  XCTAssertFalse(array->is_empty);

  array_remove_last(array);
  XCTAssertTrue(array -> is_empty);

  /* Add insert & remove_at test cases here */
  
  array_deinit(array);
}

- (void) test_count {
  var array = array_init(sizeof(int));

  XCTAssertEqual(array->count, 0);

  var delta = 19358;
  array_append(array, &delta);
  XCTAssertEqual(array->count, 1);
  
  /* Add insert & remove_at test cases here */
  
  array_remove_last(array);
  XCTAssertEqual(array->count, 0);

  array_deinit(array);
}

- (void) test_remove_all {
  var array = array_init(sizeof(int));
  
  array_remove_all(array);
  
  for (var i = 0; i < 100; i += 1) {
    array_append(array, &i);
  }
  
  XCTAssertEqual(array->count, 100);
  
  array_remove_all(array);
  
  XCTAssertEqual(array->count, 0);
  
  array_deinit(array);
}

- (void) test_sort {
  var array = array_init(sizeof(int));
  var result = array_init(sizeof(int));

  for (var i = 0; i < 19358; i += 1) {
    var delta = arc4random();
    array_append(array, &delta);
    array_append(result, &delta);
  }
  array_sort(array, compare);
  qsort(result->_storage, 19358, sizeof(int), compare);
  XCTAssertEqual(
    0,
    memcmp(result->_storage, array->_storage, 19358 * sizeof(int))
  );

  array_deinit(array);
  array_deinit(result);
}

static int compare(const void* a, const void* b) {
  if (*(int*)a > *(int*)b) {
    return 1;
  } else if (*(int*)a < *(int*)b) {
    return -1;
  }
  return 0;
}

@end
