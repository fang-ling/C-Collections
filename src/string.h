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

struct String {
  struct Array utf8;
  int count;
  bool is_empty;
};

/* Creates an empty string. */
void string_init(struct String* str);

/*
 * Creates a string from the null-terminated character sequence (C-string)
 * pointed by s.
 */
void string_init_c_string(struct String* str, const char* s);

/* Destroys a string. */
void string_deinit(struct String* str);

/* Appends the given string to this string. */
void string_append(struct String* str, struct String* other);

/*
 * Appends the null-terminated character sequence (C-string) pointed by other
 * to this string.
 */
void string_append_c_string(struct String* str, const char* other);

/*
 * Returns an array containing substrings from the string that have been divided
 * by the given separator.
 * It's caller's responsibility to free the strings stored in the result.
 */
void string_components_c_string(
  struct String* str,
  const char* separator,
  struct Array* result
);

#endif /* string_h */
