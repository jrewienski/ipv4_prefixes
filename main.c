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

    /* Removing and adding 0.0.0.0/0 -> this affects only root of trie. */
    assert(ipv4_prefix_remove(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_DOES_NOT_EXIST);
    assert(ipv4_prefix_check(ipv4_strtou32("192.1.0.1")) == -1);
    assert(ipv4_prefix_add(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_ALREADY_EXISTS);
    assert(ipv4_prefix_check(ipv4_strtou32("192.1.0.1")) == 0);
    assert(ipv4_prefix_remove(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_DOES_NOT_EXIST);

    assert(ipv4_prefix_add(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(ipv4_strtou32("192.168.0.0"), 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(ipv4_strtou32("0.0.0.0"), 0) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.168.0.1")) == 16);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.168.0.0"), 16) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("192.168.0.1")) == -1);

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

    /* Basic removal */
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

    /* Clean-up */
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 2) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_remove(ipv4_strtou32("192.168.16.0"), 24) == IPV4_PREFIX_OK);

    /* More complex trie */
    // printf("\n%u\n",ipv4_strtou32("128.0.0.0"));
    assert(ipv4_prefix_add(ipv4_strtou32("16.0.0.0"), 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(ipv4_strtou32("128.0.0.0"), 4) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 2) == IPV4_PREFIX_OK);  // Add Node A
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 4) == IPV4_PREFIX_OK);  // Add Node B
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 6) == IPV4_PREFIX_OK);  // Add Node C
    assert(ipv4_prefix_add(ipv4_strtou32("208.0.0.0"), 5) == IPV4_PREFIX_OK);  // Add Node D
    assert(ipv4_prefix_add(ipv4_strtou32("216.0.0.0"), 5) == IPV4_PREFIX_OK);  // Add Node E
    assert(ipv4_prefix_add(ipv4_strtou32("248.0.0.0"), 5) == IPV4_PREFIX_OK);
    assert(ipv4_prefix_check(ipv4_strtou32("200.0.0.1")) == 4); // Check Node B
    assert(ipv4_prefix_check(ipv4_strtou32("192.1.0.1")) == 6); // Check Node C
    assert(ipv4_prefix_check(ipv4_strtou32("208.0.0.1")) == 5); // Check Node D
    assert(ipv4_prefix_check(ipv4_strtou32("216.0.0.1")) == 5); // Check Node E

    // Testing removal of IPv4 prefix that was an intermediate node
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 4) == IPV4_PREFIX_OK);  // Remove B
    assert(ipv4_prefix_check(ipv4_strtou32("200.0.0.1")) == 2); // Now this should point to Node A
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 2) == IPV4_PREFIX_OK);  // Remove node A
    assert(ipv4_prefix_check(ipv4_strtou32("200.0.0.1")) == -1); // Now there is no prefix for 200.0.0.1

    // Testing removal of IPv4 prefix that was a leaf node
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 2) == IPV4_PREFIX_OK);  // Add Node A again
    assert(ipv4_prefix_remove(ipv4_strtou32("192.0.0.0"), 6) == IPV4_PREFIX_OK);  // Remove Node C
    assert(ipv4_prefix_check(ipv4_strtou32("192.1.0.1")) == 2); // Now this point to Node A
    assert(ipv4_prefix_add(ipv4_strtou32("192.0.0.0"), 4) == IPV4_PREFIX_OK);  // Add Node B
    assert(ipv4_prefix_check(ipv4_strtou32("192.1.0.1")) == 4); // Now this point to Node B

    // Testing removal of IPv4 prefix that was a leaf node, but parent was intermediate
    assert(ipv4_prefix_check(ipv4_strtou32("208.0.0.1")) == 5); // Check Node D
    assert(ipv4_prefix_check(ipv4_strtou32("216.0.0.1")) == 5); // Check Node E
    assert(ipv4_prefix_remove(ipv4_strtou32("208.0.0.0"), 5) == IPV4_PREFIX_OK);  // Remove Node D
    assert(ipv4_prefix_check(ipv4_strtou32("208.0.0.1")) == 2); // Now this should point to Node A
    assert(ipv4_prefix_check(ipv4_strtou32("216.0.0.1")) == 5); // Check Node E (no change)
    assert(ipv4_prefix_remove(ipv4_strtou32("216.0.0.0"), 5) == IPV4_PREFIX_OK);  // Remove Node E
    assert(ipv4_prefix_check(ipv4_strtou32("216.0.0.1")) == 2); // Now this should point to Node A

    return 0;
}
