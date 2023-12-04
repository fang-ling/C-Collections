/*
 * This source file is part of the C Collections open source project
 *
 * Copyright (c) 2023 Fang Ling
 * Licensed under Apache License v2.0
 *
 * See https://github.com/fang-ling/C-Collections/blob/main/LICENSE for license
 * information
 */

#ifndef sort_h
#define sort_h

void sort(
  void* base,
  int nel,
  int width,
  int (*compare)(const void*, const void*)
);

#endif /* sort_h */
