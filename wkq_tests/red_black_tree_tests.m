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

#import "red_black_tree.h"
#import "string.h"

#define var __auto_type

@interface RedBlackTreeTests : XCTestCase

@end

@implementation RedBlackTreeTests

- (void) test_string {
  var tree = red_black_tree_init(
    sizeof(struct String*),
    false,
    string_compare_ascii
  );
  
  struct String* input[5];
  input[0] = string_init("1");
  input[1] = string_init("3");
  input[2] = string_init("5");
  input[3] = string_init("7");
  input[4] = string_init("9");
  
  /* unique */
  for (var i = 0; i < 5; i += 1) {
    red_black_tree_insert(tree, &input[i]);
  }
  
  XCTAssertEqual(tree->count, 5);
  for (var i = 0; i < 5; i += 1) {
    struct String* string;
    red_black_tree_select(tree, i, &string);
    XCTAssertEqual(string_compare_ascii(input[i], string), 0);
  }
  
  /* duplicates */
  var five = string_init("5");
  var three = string_init("3");
  var seven = string_init("7");
  red_black_tree_insert(tree, &five);
  red_black_tree_insert(tree, &three);
  red_black_tree_insert(tree, &seven);
  string_deinit(five);
  string_deinit(three);
  string_deinit(seven);
  
  XCTAssertEqual(tree->count, 5);
  for (var i = 0; i < tree->count; i += 1) {
    struct String* string;
    red_black_tree_select(tree, i, &string);
    XCTAssertEqual(string_compare_ascii(input[i], string), 0);
  }
  
  for (var i = 0; i < 5; i += 1) {
    string_deinit(input[i]);
  }
  red_black_tree_deinit(tree);
}

- (void) test_int {
  var tree = red_black_tree_init(sizeof(Int32), false, compare);
  
  Int32 input[5];
  input[0] = 1;
  input[1] = 3;
  input[2] = 5;
  input[3] = 7;
  input[4] = 9;
  
  /* unique */
  for (var i = 0; i < 5; i += 1) {
    red_black_tree_insert(tree, &input[i]);
  }
  
  XCTAssertEqual(tree->count, 5);
  for (var i = 0; i < 5; i += 1) {
    var j = 0;
    red_black_tree_select(tree, i, &j);
    XCTAssertEqual(input[i], j);
  }
  
  /* duplicates */
  red_black_tree_insert(tree, &input[2]);
  red_black_tree_insert(tree, &input[3]);
  red_black_tree_insert(tree, &input[1]);
  XCTAssertEqual(tree->count, 5);
  for (var i = 0; i < tree->count; i += 1) {
    var j = 0;
    red_black_tree_select(tree, i, &j);
    XCTAssertEqual(input[i], j);
  }
  
  red_black_tree_deinit(tree);
}

static Int32 compare(const void* a, const void* b) {
  if (*(int*)a > *(int*)b) {
    return 1;
  } else if (*(int*)a < *(int*)b) {
    return -1;
  }
  return 0;
}

@end
