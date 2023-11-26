/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling.
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#import <XCTest/XCTest.h>

#import "array.h"

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

@end
