#include <stdio.h>
#include <assert.h>
#include "ipv4_prefix.h"
#include "utils.h"

int main(void)
{
    printf("Table of used IPv4 for prefix tests\n");
    print_ipv4_info("192.0.0.0");
    print_ipv4_info("192.168.0.0");
    print_ipv4_info("192.168.10.0");
    print_ipv4_info("192.168.16.0");
    print_ipv4_info("0.0.0.0");
    print_ipv4_info("255.255.255.255");

    /* Removal when empty */
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 4) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.1"), 32) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtou32("255.255.255.255"), 32) == IPV4_PREFIX_DOES_NOT_EXIST);

    /* Mask incorrect (>32) and mask and base mismatch */
    assert(ipv4_prefix_add(ipv4_strtou32("192.168.16.0"), 33) == IPV4_PREFIX_MASK_VALUE_ERROR);
    assert(ipv4_prefix_remove(ipv4_strtou32("255.255.255.255"), 31) == IPV4_PREFIX_BASE_MASK_MISMATCH);

    /* Adding multiple same base with various mask and checking IPv4 */
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == -1);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 4);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 24);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 2) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 24);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 24);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.1"), 32) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 32);

    /* Adding already existing IPv4 prefix */
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.1"), 32) == IPV4_PREFIX_ALREADY_EXISTS);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 16) == IPV4_PREFIX_ALREADY_EXISTS);


    /* Base and mask mismatch */
    assert(ipv4_prefix_add(ipv4_strtou32("192.168.16.0"), 16) == IPV4_PREFIX_BASE_MASK_MISMATCH);
    assert(ipv4_prefix_add(ipv4_strtou32("192.168.16.0"), 4) == IPV4_PREFIX_BASE_MASK_MISMATCH);
    assert(ipv4_prefix_add(ipv4_strtou32("192.168.16.0"), 24) == IPV4_PREFIX_OK);

    /* Removal */
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 32);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.1"), 32) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.1"), 32) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 24);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 24);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 24) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.0.0.1")) == 4);
    assert(ipv4_prefix_remove(ipv4_strtou32("208.0.0.0"), 24) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 6) == IPV4_PREFIX_DOES_NOT_EXIST);


    return 0;
}
