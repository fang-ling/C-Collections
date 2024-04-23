/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2024 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef string_h
#define string_h

//struct String {
//  /* A buffer of a string’s contents as a collection of UTF-8 code units. */
//  char utf8[STRING_MAX_LENGTH];
//  int utf8_count;
//  /* The number of characters in a string. */
//  int count;
//  /* A Boolean value indicating whether a string has no characters. */
//  bool is_empty;
//};
//
///* Creates an empty string. */
//void string_init(struct String* str);
//
///*
// * Creates a string from the null-terminated character sequence (C-string)
// * pointed by s.
// */
//void string_init_c_string(struct String* str, const char* s);
//
///* Destroys a string. */
////void string_deinit(struct String* str);
//
///* Appends the given string to this string. */
////void string_append(struct String* str, struct String* other);
//
///*
// * Appends the null-terminated character sequence (C-string) pointed by other
// * to this string.
// */
////void string_append_c_string(struct String* str, const char* other);
//
///*
// * Returns an array containing substrings from the string that have been divided
// * by the given separator.
// * It's caller's responsibility to free the strings stored in the result.
// */
////void string_components_c_string(
////  struct String* str,
////  const char* separator,
////  struct Array* result
////);
//
///* Accesses a contiguous substring of the string’s elements. */
//void string_substring(
//  struct String* str,
//  int start,
//  int end,
//  struct String* substring
//);

#endif /* string_h */
