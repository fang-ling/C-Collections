/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef binary_search_h
#define binary_search_h

/*
 * Returns the first position in which the new element cloud be inserted without
 * changing the ordering, or nel if no such element is found.
 */
int lower_bound(
  const void* key,
  const void* base,
  int nel,
  int width,
  int (*compare)(const void*, const void*)
);

#endif /* binary_search_h */
