# Overview
This is a very simple implementation of IPv4 prefixes management library.

# Compliation
Use make to run the Makefile
```
make
```
or invoke complier
```
gcc -Wall ipv4_prefix.c main.c -o test.out
```

# Implementation
Implementation uses BST to store IPv4 prefixes data.
This allows to insert, remove and search for data in O(log n) on average and O(n) in worst case scenario (if the tree is severly unbalanced).

*Notes*
- In given implementation, limitation of max 100 prefixes is set. This can be easily changed or removed.
- If limitation could be set in final implementation, the BST could be stored in an array to reduce used memory (no left-right pointers).
- Insertion and removal functions are written in non recursive way, to avoid stack overflow.
- Inorder print is written as a recursion, as a simple helper function, not to be used in production code.
- Implementation uses uint32_t, uint8_t (from stdint) to ensure the length of variables on various platforms.

*Possibilities to improve*
- AVL or WAVL implementations would ensure the tree is balanced and therefor O(log n) for search in all cases.
- AVL or WAVL, while technically insertion and removal are O(log n) in complexity, the operations are much more complex, as they introduce rotations with multiple cases and conditions.

## Code style and formatting
All functions available for user start with prefix 'ipv4_prefix_check_ip'.

Formatting with default Visual Studio clang format style (indent width: 4, spaces).


## Return status
Although the task stated that *add* and *remove* methods should be of "int" type, implementation introduces *ipv4_prefix_status* to give more information about result of a given operation:
```
    IPV4_PREFIX_OK,                     /**< Action performed on IPv4 prefix was successful. */
    IPV4_PREFIX_BASE_MASK_MISMATCH,     /**< Base and mask for IPv4 did not match. */
    IPV4_PREFIX_MASK_VALUE_ERROR,       /**< IPv4 mask was invalid (>32). */
    IPV4_PREFIX_LIST_FULL,              /**< IPv4 prefix list is full. */
    IPV4_PREFIX_ALREADY_EXISTS,         /**< IPv4 prefix already exists on the list. */
    IPV4_PREFIX_DOES_NOT_EXIST          /**< IPv4 prefix does not exit on the list. */
```

## Code coverage
Code is covered with tests in main.c file.
To run tests and gcov analysis, run:
```
make
./test.out
make coverage
```

At the moment code coverage is at 100% (by gcov):
```
gcov main.c ipv4_prefix.c
File 'main.c'
Lines executed:100.00% of 58
Creating 'main.c.gcov'

File 'ipv4_prefix.c'
Lines executed:100.00% of 112
Creating 'ipv4_prefix.c.gcov'
```