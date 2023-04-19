#include "ipv4_prefix.h"
#include <stdio.h>
#include <stdlib.h>

ipv4_prefix_trie_node *trie_root = NULL;

ipv4_prefix_status ipv4_prefix_validate(uint32_t base, uint8_t mask)
{
    if (mask > MAX_MASK_VALUE)
    {
        return IPV4_PREFIX_MASK_VALUE_ERROR;
    }

    for (int8_t i = MAX_MASK_VALUE - 1 - mask; i >= 0; i--)
    {
        if ((base >> i) & 0x1)
            return IPV4_PREFIX_BASE_MASK_MISMATCH;
    }

    return IPV4_PREFIX_OK;
}

ipv4_prefix_status ipv4_prefix_add(uint32_t base, uint8_t mask)
{
    ipv4_prefix_status err_code;

    err_code = ipv4_prefix_validate(base, mask);
    if (err_code != IPV4_PREFIX_OK)
    {
        return err_code;
    }

    if (trie_root == NULL) {
        initR();
    }

    return insert(base, mask);
}

ipv4_prefix_status ipv4_prefix_remove(uint32_t base, uint8_t mask)
{
    ipv4_prefix_status err_code;

    err_code = ipv4_prefix_validate(base, mask);
    if (err_code != IPV4_PREFIX_OK)
    {
        return err_code;
    }

    if (trie_root == NULL) {
        return IPV4_PREFIX_DOES_NOT_EXIST;
    }

    uint8_t current_bit;
    ipv4_prefix_trie_node* parent = trie_root;
    ipv4_prefix_trie_node* current = NULL;
    ipv4_prefix_trie_node* chain[mask+1]; // +1 for root node

    for (uint8_t i = 0; i <= mask; i++) {
        chain[i] = NULL;
    }
    chain[0] = parent;

    // Traverse from root
    for (uint8_t i = 1; i <= mask; i++) {
        chain[i] = parent;
        current_bit = (base >> (32 - i)) & 0x1;
        if (current_bit) {
            if (parent->one) {
                current = parent->one;
            } else {
                return IPV4_PREFIX_DOES_NOT_EXIST;
            }
        } else {
            if (parent->zero) {
                current = parent->zero;
            } else {
                return IPV4_PREFIX_DOES_NOT_EXIST;
            } 
        }

        parent = current;
    }

    // If traversed to given prefix, but it's only an intermediate node.
    if (current->word_end == false) {
        return IPV4_PREFIX_DOES_NOT_EXIST;
    }

    // Prefix node had children - meaning subprefixes are used, do not delete.
    if (current->zero || current->one) {
        current->word_end = false;
        return IPV4_PREFIX_OK;
    }

    // Traverse back
    for (int8_t i = mask; i >= 1; i--){
        current = chain[i];
        parent = chain[i-1];
        if (parent->zero == current) {
            parent->zero = NULL;
            free(current);
        } else {
            parent->one = NULL;
            free(current);
        }

        // Reached a point where node was had more then one child or is a word-end.
        // Cannot delete any more nodes.
        if (parent->zero != NULL || parent->one != NULL || parent->word_end == true) {
            break;
        }
    }

    return IPV4_PREFIX_OK;
}

int8_t ipv4_prefix_check(uint32_t ipv4)
{
    uint8_t current_bit;
    int8_t highest_mask = -1;
    uint8_t depth = 0;
    ipv4_prefix_trie_node* parent = trie_root;
    ipv4_prefix_trie_node* current = NULL;

    if (parent == NULL) return -1;

    for (int i = 31; i >= 0; i--){
        current_bit = (ipv4 >> i) & 0x1;
        if (current_bit) {
            current = parent->one;
        } else {
            current = parent->zero;
        }
        depth++;


        if (current == NULL) {
            break;
        } else {
            if (current->word_end) highest_mask = depth;
        }
        parent = current;
    }

    return highest_mask;
}

void initR() {
    trie_root = malloc(sizeof(ipv4_prefix_trie_node));
    trie_root->one = NULL;
    trie_root->zero = NULL;
    trie_root->word_end = false;
}


ipv4_prefix_status insert(uint32_t base, uint8_t mask) {
    uint8_t i = 0;
    uint8_t current_bit;
    ipv4_prefix_trie_node* parent = trie_root;
    ipv4_prefix_trie_node* current = NULL;

    // Traverse from root
    while (i < mask) {
        current_bit = (base >> (31 - i)) & 0x1;
        if (current_bit) {
            if (parent->one) {
                current = parent->one;
            } else {
                current = malloc(sizeof(ipv4_prefix_trie_node));
                current->word_end = false;
                parent->one = current;
            }
        } else {
            if (parent->zero) {
                current = parent->zero;
            } else {
                // printf("here");
                current = malloc(sizeof(ipv4_prefix_trie_node));
                current->word_end = false;
                parent->zero = current;
            } 
        }

        parent = current;
        i++;
    }

    if (current->word_end == false) {
        current->word_end = true;
        return IPV4_PREFIX_OK;
    } else {
        return IPV4_PREFIX_ALREADY_EXISTS;
    }
}
