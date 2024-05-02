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
  
  var empty = string_init("");
  XCTAssertEqual(empty->count, 0);
  XCTAssertTrue(empty->is_empty);
  
  buf[0] = '\0';
  var empty2 = string_init(buf);
  XCTAssertEqual(empty2->count, 0);
  XCTAssertTrue(empty2->is_empty);
  
  var empty3 = string_substring(string, 15, 15);
  XCTAssertEqual(empty3->count, 0);
  XCTAssertTrue(empty3->is_empty);
  
  string_deinit(string);
  string_deinit(empty);
  string_deinit(empty2);
  string_deinit(empty3);
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
  
  XCTAssertTrue(string_compare_ascii(&s1, &s2) < 0);
  XCTAssertTrue(string_compare_ascii(&s2, &s3) < 0);
  XCTAssertTrue(string_compare_ascii(&s3, &s1) > 0);
  
  string_deinit(s1);
  string_deinit(s2);
  string_deinit(s3);
  
  s1 = string_init("cat");
  s2 = string_init("dog");
  
  XCTAssertTrue(string_compare_ascii(&s1, &s2) < 0);
  
  string_deinit(s1);
  string_deinit(s2);
}

- (void) test_to_int64 {
  var s1 = string_init("0x3f3f3f3f3f3f3f3fLL");
  var s2 = string_init("1001");
  var s3 = string_init("-238124712831290LL");
  var outofrange = string_init("9999999999999999999999999999999999999999");
  var invalid = string_init("TRACY");
  
  var r = 0LL;
  XCTAssertEqual(string_to_int64(s1, 16, &r), 0);
  XCTAssertEqual(r, 0x3f3f3f3f3f3f3f3fLL);
  
  XCTAssertEqual(string_to_int64(s2, 2, &r), 0);
  XCTAssertEqual(r, 9);
  
  XCTAssertEqual(string_to_int64(s3, 10, &r), 0);
  XCTAssertEqual(r, -238124712831290LL);
  
  XCTAssertEqual(string_to_int64(outofrange, 10, &r), 1);
  
  XCTAssertEqual(string_to_int64(invalid, 10, &r), -1);
  
  string_deinit(s1);
  string_deinit(s2);
  string_deinit(s3);
  string_deinit(outofrange);
  string_deinit(invalid);
}

- (void) test_contains {
  var string = string_init("1 + 1 = 2");
  var another = string_init("=");
  var another2 = string_init("==");
  
  XCTAssertTrue(string_contains(string, another));
  XCTAssertFalse(string_contains(string, another2));
  
  string_deinit(string);
  string_deinit(another);
}

- (void) test_first_index_of {
  var s1 = string_init("Hello, World");
  var m11 = string_init("o");
  var m12 = string_init("z");
  var s2 = string_init("Tracy  is ...");
  var m21 = string_init("is");
  var m22 = string_init("Diana");
  var empty = string_init("");
  
  /* Substring found */
  XCTAssertEqual(string_first_index_of(s1, m11), 4);
  XCTAssertEqual(string_first_index_of(s2, m21), 7);
  
  /* Substring not found */
  XCTAssertEqual(string_first_index_of(s1, m12), -1);
  XCTAssertEqual(string_first_index_of(s2, m22), -1);
  
  /* Empty string */
  XCTAssertEqual(string_first_index_of(empty, empty), -1);
  XCTAssertEqual(string_first_index_of(empty, m11), -1);
  XCTAssertEqual(string_first_index_of(s1, empty), -1);
  
  /* Substring is the entire string */
  XCTAssertEqual(string_first_index_of(s1, s1), 0);
  
  string_deinit(s1);
  string_deinit(m11);
  string_deinit(m12);
  string_deinit(s2);
  string_deinit(m21);
  string_deinit(m22);
  string_deinit(empty);
}

- (void) test_last_index_of {
  var s1 = string_init("Hello, World");
  var m11 = string_init("o");
  var m12 = string_init("z");
  var s2 = string_init("Tracy  is ...");
  var m21 = string_init("is");
  var m22 = string_init("Diana");
  var empty = string_init("");
  var s3 = string_init("foo bar foo baz");
  var m3 = string_init("foo");
  
  /* Substring found */
  XCTAssertEqual(string_last_index_of(s1, m11), 8);
  XCTAssertEqual(string_last_index_of(s2, m21), 7);
  
  /* Substring not found */
  XCTAssertEqual(string_last_index_of(s1, m12), -1);
  XCTAssertEqual(string_last_index_of(s2, m22), -1);
  
  /* Empty string */
  XCTAssertEqual(string_last_index_of(empty, empty), -1);
  XCTAssertEqual(string_last_index_of(empty, m11), -1);
  XCTAssertEqual(string_last_index_of(s1, empty), -1);
  
  /* Substring is the entire string */
  XCTAssertEqual(string_last_index_of(s1, s1), 0);
  
  /* Multiple occurrences of the substring */
  XCTAssertEqual(string_last_index_of(s3, m3), 8);
  
  string_deinit(s1);
  string_deinit(m11);
  string_deinit(m12);
  string_deinit(s2);
  string_deinit(m21);
  string_deinit(m22);
  string_deinit(empty);
  string_deinit(s3);
  string_deinit(m3);
}

@end
