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

#import "deque.h"

#define var __auto_type

@interface DequeTests : XCTestCase

@end

@implementation DequeTests

- (void) test_init {
  var deque = deque_init(sizeof(int));
  
  XCTAssertEqual(deque->count, 0);
  XCTAssertEqual(deque->_width, sizeof(int));
  XCTAssertEqual(deque->is_empty, true);
  
  deque_deinit(deque);
}

- (void) test_is_empty {
  var deque = deque_init(sizeof(int));
  XCTAssertTrue(deque->is_empty);

  var delta = 19358;
  deque_append(deque, &delta);
  XCTAssertFalse(deque->is_empty);

  deque_remove_first(deque);
  XCTAssertTrue(deque -> is_empty);

  /* Add insert & remove_at test cases here */
  
  deque_deinit(deque);
}

- (void) test_count {
  var deque = deque_init(sizeof(int));

  XCTAssertEqual(deque->count, 0);

  var delta = 19358;
  deque_append(deque, &delta);
  XCTAssertEqual(deque->count, 1);
  
  /* Add insert & remove_at test cases here */
  
  deque_remove_first(deque);
  XCTAssertEqual(deque->count, 0);

  deque_deinit(deque);
}

- (void) test_remove_all {
  var deque = deque_init(sizeof(int));
  
  deque_remove_all(deque);
  
  for (var i = 0; i < 100; i += 1) {
    deque_append(deque, &i);
  }
  
  XCTAssertEqual(deque->count, 100);
  
  deque_remove_all(deque);
  
  XCTAssertEqual(deque->count, 0);
  
  deque_deinit(deque);
}

@end
