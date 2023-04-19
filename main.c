#include <stdio.h>
#include <assert.h>
#include "ipv4_prefix.h"
#include "utils.h"


int main(void) {
    printf("Table of used IPv4 for prefix tests\n");
    print_ipv4_info("192.0.0.0");
    print_ipv4_info("192.168.0.0");
    print_ipv4_info("192.168.10.0");
    print_ipv4_info("192.168.16.0");
    print_ipv4_info("0.0.0.0");
    print_ipv4_info("255.255.255.255");

    /* Removal when empty */
    assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.0"), 4) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.1"), 32) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtouint32("0.0.0.0"), 0) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtouint32("255.255.255.255"), 32) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtouint32("255.255.255.255"), 31) == IPV4_PREFIX_BASE_MASK_MISMATCH);


    /* Adding multiple same base with various mask and checking IPv4 */
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == -1);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.0.0.0"), 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 4);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.0.0.0"), 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 24);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.0.0.0"), 2) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 24);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.0.0.0"), 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 24);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.0.0.1"), 32) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 32);

    /* Base and mask mismatch */
    assert(ipv4_prefix_add(ipv4_strtouint32("192.168.16.0"), 16) == IPV4_PREFIX_BASE_MASK_MISMATCH);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.168.16.0"), 4) == IPV4_PREFIX_BASE_MASK_MISMATCH);
    assert(ipv4_prefix_add(ipv4_strtouint32("192.168.16.0"), 24) == IPV4_PREFIX_OK);

    /* Removal */
    // assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.0"), 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 32);
    assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.1"), 32) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.1"), 32) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 24);
    assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.0"), 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 24);
    assert(ipv4_prefix_remove(ipv4_strtouint32("192.0.0.0"), 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtouint32("192.0.0.1")) == 4);




    return 0;

    // bin. 1100 0000. 1010 1000 . 0000 1010 . 1100 1000 = 3232238280
    // 192.168.10.200 / 24   
    // assert(ipv4_prefix_add(3232238280, 30) == IPV4_PREFIX_OK);
    // assert(ipv4_prefix_add(3232238080, 30) == IPV4_PREFIX_OK);
    // assert(ipv4_prefix_add(3232238080, 16) == IPV4_PREFIX_BASE_MASK_MISMATCH);
    // assert(ipv4_prefix_add(3232238080, 26) == IPV4_PREFIX_OK);


    // assert(ipv4_prefix_add(3231711232, 12) == IPV4_PREFIX_OK);  // 192.160.00.0 / 12
    // assert(ipv4_prefix_add(3231711232, 24) == IPV4_PREFIX_OK);  // 192.160.00.0 / 24

    // ipv4_prefix_print_inorder();
    // print2D();
    printf("Res for 192.160.0.1 -> %d\n", ipv4_prefix_check(3231711233));  /// 192.160.0.1  -> should return 24 (!)

    printf("Res for 192.160.0.1 -> %d\n", ipv4_prefix_check(3231711233));  /// 192.160.0.1  -> should return 24 (!)

    // 3232238088  = 192.168.10.8
    printf("Res for 192.168.10.8 -> %d\n", ipv4_prefix_check(3232238088));
    printf("Res for 192.168.10.1 -> %d\n", ipv4_prefix_check(3232238081));
    // assert(ipv4_prefix_check(3232238088) == 24);



    return 0;
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
    // Use print function just to cover 100% of code with test
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
