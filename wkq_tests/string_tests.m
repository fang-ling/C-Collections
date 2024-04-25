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

#import "string.h"

#define var __auto_type

@interface StringTests : XCTestCase

@end

@implementation StringTests

- (void) test_init {
  var c_str = "This is a test string, こんにちは, 🐶。";
  var string = string_init(c_str);
  
  XCTAssertEqual(string->count, 32);
  XCTAssertFalse(string->is_empty);
  
  char buf[128];
  string_c_string(string, buf);
  
  XCTAssertEqual(0, strcmp(buf, c_str));
  
  string_deinit(string);
}

- (void) test_components {
  var string = string_init("#zyy#jasdjq2n3oasd#zyy##zyy#adn972929#zyy#");
  var separator = string_init("#zyy#");
  
  var result = array_init(sizeof(struct String*));
  
  string_components(string, separator, result);
  
  char buf[128];
  struct String* s;
  
  XCTAssertEqual(result->count, 5);
  
  array_get(result, 0, &s);
  string_c_string(s, buf);
  XCTAssertEqual(0, strcmp(buf, ""));
  free(s);
  
  array_get(result, 1, &s);
  string_c_string(s, buf);
  XCTAssertEqual(0, strcmp(buf, "jasdjq2n3oasd"));
  free(s);
  
  array_get(result, 2, &s);
  string_c_string(s, buf);
  XCTAssertEqual(0, strcmp(buf, ""));
  free(s);
  
  array_get(result, 3, &s);
  string_c_string(s, buf);
  XCTAssertEqual(0, strcmp(buf, "adn972929"));
  free(s);
  
  array_get(result, 4, &s);
  string_c_string(s, buf);
  XCTAssertEqual(0, strcmp(buf, ""));
  free(s);
  
  array_deinit(result);
  string_deinit(string);
}

- (void) test_compare_ascii {
  var s1 = string_init("1");
  var s2 = string_init("10");
  var s3 = string_init("2");
  
  XCTAssertTrue(string_compare_ascii(s1, s2) < 0);
  XCTAssertTrue(string_compare_ascii(s2, s3) < 0);
  XCTAssertTrue(string_compare_ascii(s3, s1) > 0);
  
  string_deinit(s1);
  string_deinit(s2);
  string_deinit(s3);
  
  s1 = string_init("cat");
  s2 = string_init("dog");
  
  XCTAssertTrue(string_compare_ascii(s1, s2) < 0);
  
  string_deinit(s1);
  string_deinit(s2);
}

@end
