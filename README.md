## C Collections

**C Collections** is an open-source package of data structure implementations and algorithms for ANSI C. It's primarily designed for competitive programming, but we strive to make it generic and ensure memory safety. We also aim to minimize the use of third-party dependencies.

Tested under:

- UVa Online Judge: 5.3.0 - GNU C Compiler with options: `-lm -lcrypt -O2 -pipe -ansi -DONLINE_JUDGE`
- Local Xcode Workplace: 15.0.0 - Apple clang with customized option: `-ansi`

Objective-C codes are only for Xcode unit tests (`xctest`).

## Contents

- `Array` [`v1.6`] An ordered, random-access collection.
- `BTree` [`v1.0`] An efficient in-memory B-tree implementation, suitable for use as a bag, a set, or a dictionary.
- `Deque` [`v1.1`] A double-ended queue backed by a ring buffer. Deques are random-access collections that allows fast insertion and deletion at both its beginning and its end.

- `binary_search()` [`v1.1`] An efficient algorithm used to quickly locate a specific target value within a sorted collection.
- `sort()` [`v1.0`] Randomized quicksort with insertion sort on small arrays and optimized for sorted and reverse-sorted arrays.
