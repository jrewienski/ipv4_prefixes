#include <stdio.h>
#include "ipv4_prefix.h"

static ipv4_prefix prefixes[MAX_PREFIXES];
static uint8_t prefixes_cnt = 0;

void ipv4_prefix_print_list(void) {
    printf("## Prefix list \n");
    for (uint8_t i = 0; i < prefixes_cnt; i++) {
        printf("%3d, %u/%d\n", i, prefixes[i].base, prefixes[i].mask);
    }
}

ipv4_prefix_status ipv4_validate_prefix(uint32_t base, uint8_t mask) {
    if (mask > MAX_MASK_VALUE) {
        return IPV4_PREFIX_MASK_VALUE_ERROR;
    }

    for (uint8_t i = 0; i < mask; i++) {
        if ((base >> i) & 0x1) return IPV4_PREFIX_BASE_MASK_MISMATCH;
    }

    return IPV4_PREFIX_OK;
}

ipv4_prefix_status ipv4_add(uint32_t base, uint8_t mask) {
    ipv4_prefix_status err_code;

    err_code = ipv4_validate_prefix(base, mask);
    if (err_code != IPV4_PREFIX_OK) {
        return err_code;
    }

    if (prefixes_cnt == MAX_PREFIXES) {
        return IPV4_PREFIX_LIST_FULL;
    }

    uint8_t i;
    for (i = 0; i < prefixes_cnt; i++) {
        if (prefixes[i].base == base && prefixes[i].mask == mask) return IPV4_PREFIX_ALREADY_EXISTS;
        if (prefixes[i].base > base || (prefixes[i].base == base && prefixes[i].mask < mask)) break; 
    }

    uint8_t j;
    for (j = prefixes_cnt; j > i; j--) {
        prefixes[j] = prefixes[j-1];
    }

    prefixes[j] = prefixes[i];   
    prefixes[i].base = base;
    prefixes[i].mask = mask;

    prefixes_cnt++;
    return IPV4_PREFIX_OK;
}

ipv4_prefix_status ipv4_remove(uint32_t base, uint8_t mask) {
    ipv4_prefix_status err_code;

    err_code = ipv4_validate_prefix(base, mask);
    if (err_code != IPV4_PREFIX_OK) {
        return err_code;
    }

    uint8_t i;
    for (i = 0; i < prefixes_cnt; i++) {
        if (prefixes[i].base == base && prefixes[i].mask == mask) break;
    }
    if (i == prefixes_cnt) return IPV4_PREFIX_DOES_NOT_EXIST;

    for (; i < prefixes_cnt; i++) {
        prefixes[i] = prefixes[i+1];
    }
    prefixes_cnt--;

    return IPV4_PREFIX_OK;
}


int8_t ipv4_check(uint32_t ipv4) {
    for (uint8_t i = 0; i < prefixes_cnt; i++) {
        if ((ipv4 >> prefixes[i].mask) == (prefixes[i].base >> prefixes[i].mask)) return (int8_t) prefixes[i].mask;
    }

    return -1;
}

