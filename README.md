## C Collections

**C Collections** is an open-source package of data structure implementations and algorithms for ANSI C. It's primarily designed for competitive programming, but we strive to make it generic and ensure memory safety. We also aim to minimize the use of third-party dependencies.

Tested under:

- UVa Online Judge: 5.3.0 - GNU C Compiler with options: `-lm -lcrypt -O2 -pipe -ansi -DONLINE_JUDGE`
- Local Xcode Workplace: 15.0.0 - Apple clang with customized option: `-ansi`

Objective-C codes are only for Xcode unit tests (`xctest`).

## Contents

- `Array` v1.2 An ordered, random-access collection.
- `sort()` v1.0 Randomized quicksort with insertion sort on small arrays and optimized for sorted and reverse-sorted arrays.
