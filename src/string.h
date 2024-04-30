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

#include "types.h"

#define WKQ_INT64_MAX       0x7fffffffffffffffLL      /* max signed long long */
#define WKQ_INT64_MIN       (-0x7fffffffffffffffLL-1) /* min signed long long */

#include <stdlib.h>
#include <ctype.h>

#include "array.h"

struct String {
  /*
   * A buffer for storing UTF-8 string as an array of uint32 numbers.
   * In UTF-8 encoding, characters are represented by variable-length code 
   * units. The code units for a single character are "concatenated" together to
   * form a single uint32 number. This allows easy implementation of functions
   * for manipulating multi-language strings.
   */
  UInt32* _utf8;
  
  /* The length of each unicode character. */
  Int32* _utf8_length;
  
  Int64 _utf8_capacity;
  
  /* The number of (unicode) characters in a string. */
  Int64 count;
  
  /* A Boolean value indicating whether a string has no characters. */
  Bool is_empty;
};

/*----------------------------------------------------------------------------*/
/*
 * Creates a string from the null-terminated character sequence (C-string)
 * pointed by s.
 */
struct String* string_init(const char* s);

/* Destroys a string. */
void string_deinit(struct String* string);

/* Returns a representation of the string as a C string using utf-8 encoding. */
void string_c_string(struct String* string, char* result);

struct String* string_substring(
  struct String* string,
  Int64 start,
  Int64 end
);

void string_components(
  struct String* string,
  struct String* separator,
  struct Array* result
);

/*
 * Returns an integer greater than, equal to, or less than 0, according as the
 * string lhs is greater than, equal to, or less than the string rhs.
 */
Int32 string_compare_ascii(const void* lhs, const void* rhs);

Int32 string_to_int64(struct String* string, Int32 base, Int64* result);

Bool string_contains(struct String* string, struct String* another);

Int64 string_last_index_of(struct String* string, struct String* value);

Int64 string_first_index_of(struct String* string, struct String* value);
/*----------------------------------------------------------------------------*/

#endif /* string_h */
