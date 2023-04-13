#include <stdio.h>
#include <assert.h>
#include "ipv4_prefix.h"


int main(void) {
    /* Tests when no prefixes exist */
    assert(ipv4_prefix_remove(2560, 1) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_check(32858) == -1);
    assert(ipv4_prefix_remove(2574, 4) == IPV4_PREFIX_BASE_MASK_MISMATCH);

    /* Adding prefixes */
    assert(ipv4_prefix_add(4026531840, 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(4026531840, 24) == IPV4_PREFIX_ALREADY_EXISTS);
    assert(ipv4_prefix_add(4026531840, 26) == IPV4_PREFIX_OK);


    assert(ipv4_prefix_add(3690987520, 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(2560, 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(2560, 8) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(2560, 6) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(589824, 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(2560, 24) == IPV4_PREFIX_BASE_MASK_MISMATCH);

    assert(ipv4_prefix_check(2580) == 8);
    assert(ipv4_prefix_check(37374) == -1);


    /* Removing prefixes */
    assert(ipv4_prefix_remove(2560, 1) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(2560, 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(3690987520, 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(3690987520, 24) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(4026531840, 26) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(2560, 6) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(589824, 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(4026531840, 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(2560, 8) == IPV4_PREFIX_OK);

    /* Create a bigger tree to test all special cases */
    assert(ipv4_prefix_add(7, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(3, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(15, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(13, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(12, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(10, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(11, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(20, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(25, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(19, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(5, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(1, 0) == IPV4_PREFIX_OK);
    // Use print function just to cover 100% with tests
    ipv4_prefix_print_inorder();

    assert(ipv4_prefix_remove(15, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(20, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(19, 0) == IPV4_PREFIX_OK);

    /* Clean up */
    assert(ipv4_prefix_remove(1, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(5, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(3, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(7, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(10, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(11, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(12, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(13, 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(25, 0) == IPV4_PREFIX_OK);

    /* Test incorrect mask (>32) */
    assert(ipv4_prefix_add(250, 35) == IPV4_PREFIX_MASK_VALUE_ERROR);

    /* Test limit of 100 prefixes */
    for (int i = 0; i < 100; i++) {
        assert(ipv4_prefix_add(i, 0) == IPV4_PREFIX_OK);
    }
    assert(ipv4_prefix_add(101, 0) == IPV4_PREFIX_LIST_FULL);

    /* Clean up */
    for (int i = 0; i < 100; i++) {
        assert(ipv4_prefix_remove(i, 0) == IPV4_PREFIX_OK);
    }
}