# Overview
This is a very simple implementation of IPv4 prefixes management library.

# Compliation
Use make to run the Makefile
```
make
```

# Implementation
Implementation uses Trie data structure/
This allows to insert, remove and search for data in O(1). This does not depend on the number of prefixes added, only on maximal mask (i.e. 32).
Worst case scenario, if added a prefix with /32 (e.g. 192.168.1.1/32) and checking 192.168.1.1 it will take 32 steps to find result.

*Notes*
- Insertion and removal functions are written in non recursive way, to avoid stack overflow.
- Implementation uses uint32_t, uint8_t (from stdint) to ensure the length of variables on various platforms.

*Possibilities to improve*
- Folding in Trie might be implemented, to reduce number of steps when adding, removing and searching, but this would cause more memory usage. The trie nodes would have to have base and mask fields.

## Code style and formatting
All functions available for user start with prefix 'ipv4_prefix_check_ip'.

Formatting with default Visual Studio clang format style (indent width: 4, spaces).

## Return status
Although the task stated that *add* and *remove* methods should be of "int" type, implementation introduces *ipv4_prefix_status* to give more information about result of a given operation:
```
    IPV4_PREFIX_OK,                 /**< Action performed on IPv4 prefix was successful. */
    IPV4_PREFIX_BASE_MASK_MISMATCH, /**< Base and mask for IPv4 did not match. */
    IPV4_PREFIX_MASK_VALUE_ERROR,   /**< IPv4 mask was invalid (>32). */
    IPV4_PREFIX_ALREADY_EXISTS,     /**< IPv4 prefix already exists on the list. */
    IPV4_PREFIX_DOES_NOT_EXIST,     /**< IPv4 prefix does not exit on the list. */
    IPV4_PREFIX_INSUFFICIENT_MEM    /**< Not enough memory to allocate IPv4 prefix. */
```

## Util functions
Set of util functions allowing to use IPv4 prefixes and IPv4 for testing in string form (e.g. "192.168.10.0" instead of 3232238080).

# Testing
Main file consists of number of assertions, that do basic tests for addition, removal and check.

## Simple tests
Few simple tests provided:
- Removal when empty
- Mask incorrect (>32) and mask and base mismatch
- Adding multiple same base with various mask and checking IPv4
- Adding already existing IPv4 prefix
- Base and mask mismatch
- Basic removal

## Complex test
More complex test of removal shows such trie:

![Trie for complex removal test](/docs/img/testing_trie.png "Trie for complex removal test")

Tests on given trie:
- Testing removal of IPv4 prefix that was an intermediate node
- Testing removal of IPv4 prefix that was a leaf node
- Testing removal of IPv4 prefix that was a leaf node, but parent was intermediate

More details in main.c file.

## Code coverage
Code is covered with tests in main.c file.
To run tests and gcov analysis, run:
```
make
./test.out
make coverage
```

At the moment code coverage is at 97% (by gcov), due to not checking running out of memory issue. (i.e. malloc too many nodes)
```
gcov main.c ipv4_prefix.c utils.c
File 'main.c'
Lines executed:100.00% of 85
Creating 'main.c.gcov'

File 'ipv4_prefix.c'
Lines executed:97.24% of 145
Creating 'ipv4_prefix.c.gcov'

File 'utils.c'
Lines executed:90.91% of 33
Creating 'utils.c.gcov'
```

## Static code analysis
Cppcheck does not mark any warnings (except for stdlibrary headers) or issues:

```
cppcheck --enable=all main.c ipv4_prefix.c
Checking ipv4_prefix.c ...
1/2 files checked 53% done
Checking main.c ...
2/2 files checked 100% done
nofile:0:0: information: Cppcheck cannot find all the include files (use --check-config for details) [missingIncludeSystem]
```