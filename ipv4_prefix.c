#include "ipv4_prefix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

ipv4_prefix_trie_node *trie_root = NULL;

static ipv4_prefix_status root_init(void);
static ipv4_prefix_status node_insert(uint32_t base, uint8_t mask);
static ipv4_prefix_status node_remove(uint32_t base, uint8_t mask);

/* Traverses through trie and insert node for IPv4 prefix.
 */
static ipv4_prefix_status root_init(void)
{
    trie_root = (ipv4_prefix_trie_node *)malloc(sizeof(ipv4_prefix_trie_node));
    if (!trie_root)
        return IPV4_PREFIX_INSUFFICIENT_MEM;

    trie_root->one = NULL;
    trie_root->zero = NULL;
    trie_root->word_end = false;

    return IPV4_PREFIX_OK;
}

/* Traverses through trie and insert node for IPv4 prefix.
 *
 * @param[in]   base        IPv4 Prefix base
 * @param[in]   mask        IPv$ Prefix mask
 *
 * @return      IPV4_PREFIX_OK                      If IPv4 prefix was added successfully.
 * @return      IPV4_PREFIX_ALREADY_EXISTS          If prefix already exists in the list.
 */
static ipv4_prefix_status node_insert(uint32_t base, uint8_t mask)
{
    ipv4_prefix_trie_node *parent = trie_root;
    ipv4_prefix_trie_node *current = trie_root;

    // if (mask == 0) {
    //     if (trie_root->word_end == false) {
    //         trie_root->word_end = true;
    //         return IPV4_PREFIX_OK;
    //     } else {
    //         return IPV4_PREFIX_ALREADY_EXISTS;
    //     }
    // }

    // Traverse from root, to create all missing nodes for given prefix.
    for (uint8_t i = 0; i < mask; i++)
    {
        // Get prefix base bits from MSB to choose left or right subtree.
        if ((base >> (MAX_MASK_VALUE - 1 - i)) & 0x1)
        {
            // If the subtree node exists then continue, else create node and continue.
            if (parent->one)
            {
                current = parent->one;
            }
            else
            {
                current = (ipv4_prefix_trie_node *)malloc(sizeof(ipv4_prefix_trie_node));
                if (!current)
                    return IPV4_PREFIX_INSUFFICIENT_MEM;
                current->word_end = false;
                current->one = NULL;
                current->zero = NULL;
                parent->one = current;
            }
        }
        else
        {
            if (parent->zero)
            {
                current = parent->zero;
            }
            else
            {
                current = (ipv4_prefix_trie_node *)malloc(sizeof(ipv4_prefix_trie_node));
                if (!current)
                    return IPV4_PREFIX_INSUFFICIENT_MEM;
                current->word_end = false;
                current->one = NULL;
                current->zero = NULL;
                parent->zero = current;
            }
        }

        parent = current;
    }

    // Finally, when landed on the last masked bit of prefix, set the word_end flag to true.
    // If it was true already, that means the prefix already existed.
    if (current && current->word_end == false)
    {
        current->word_end = true;
        return IPV4_PREFIX_OK;
    }
    else
    {
        return IPV4_PREFIX_ALREADY_EXISTS;
    }
}

/* Traverses the trie and removes IPv4 prefix node.
 *
 * @param[in]   base        IPv4 Prefix base
 * @param[in]   mask        IPv$ Prefix mask
 *
 * @return      IPV4_PREFIX_OK                      If IPv4 prefix was removed successfully.
 * @return      IPV4_PREFIX_DOES_NOT_EXIST          If prefix was not on the list exists.
 */
ipv4_prefix_status node_remove(uint32_t base, uint8_t mask)
{
    ipv4_prefix_trie_node *parent = trie_root;
    ipv4_prefix_trie_node *current = trie_root;
    ipv4_prefix_trie_node *chain[mask + 1]; // Storing the whole branch, +1 for the root node.

    // Cover edge-case of mask == 0, where trie-root has to be freed / updated.
    if (mask == 0) {
        if (trie_root->word_end) {
            if ((trie_root->zero == NULL) && (trie_root->one == NULL)) {
                free(trie_root);
                trie_root = NULL;
            } else {
                trie_root->word_end = false;
            }
            return IPV4_PREFIX_OK;
        }
    }

    for (uint8_t i = 0; i <= mask; i++)
    {
        chain[i] = NULL;
    }
    chain[0] = trie_root;

    // Traverse from root, add nodes to chain[]
    // Return if there is no node (e.g. for 11001 we have found branch 110, but no further 0).
    for (uint8_t i = 1; i <= mask; i++)
    {
        if ((base >> (MAX_MASK_VALUE - i)) & 0x1)
        {
            if (parent->one)
            {
                current = parent->one;
            }
            else
            {
                return IPV4_PREFIX_DOES_NOT_EXIST;
            }
        }
        else
        {
            if (parent->zero)
            {
                current = parent->zero;
            }
            else
            {
                return IPV4_PREFIX_DOES_NOT_EXIST;
            }
        }

        parent = current;
        chain[i] = parent;
    }

    // If traversed to given prefix, but it's only an intermediate node, not end of word.
    if (current->word_end == false)
    {
        return IPV4_PREFIX_DOES_NOT_EXIST;
    }

    // Prefix node had children - meaning subprefixes are used, do not delete but change the flag
    // so it's not end of word anymore.
    if (current->zero || current->one)
    {
        current->word_end = false;
        return IPV4_PREFIX_OK;
    }

    // Traverse back to remove all chain nodes from leaf to root.
    // Break if a given node has another child (another subtree).
    for (int8_t i = mask; i > 0; i--)
    {
        current = chain[i];
        parent = chain[i - 1];
        if (parent->zero == current)
        {
            parent->zero = NULL;
            free(current);
        }
        else
        {
            parent->one = NULL;
            free(current);
        }

        // Reached a point where node had more then one child or is a word-end.
        // Cannot delete any more nodes.
        if (parent->zero != NULL || parent->one != NULL || parent->word_end == true)
        {
            break;
        }
    }

    return IPV4_PREFIX_OK;
}

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

    if (!trie_root)
    {
        err_code = root_init();
        if (err_code != IPV4_PREFIX_OK)
        {
            return err_code;
        }
    }

    return node_insert(base, mask);
}

ipv4_prefix_status ipv4_prefix_remove(uint32_t base, uint8_t mask)
{
    ipv4_prefix_status err_code;

    err_code = ipv4_prefix_validate(base, mask);
    if (err_code != IPV4_PREFIX_OK)
    {
        return err_code;
    }

    if (!trie_root)
    {
        return IPV4_PREFIX_DOES_NOT_EXIST;
    }

    return node_remove(base, mask);
}

int8_t ipv4_prefix_check(uint32_t ipv4)
{
    int8_t highest_mask = -1;
    uint8_t depth = 0;
    ipv4_prefix_trie_node *parent = trie_root;
    ipv4_prefix_trie_node *current = NULL;

    // If there is no root, the IPv4 prefix list is empty, return -1.
    if (!trie_root) {
        return IPV4_NOT_IN_PREFIXES_RANGE;
    } else if (trie_root->word_end) {
        highest_mask = 0;
    }

    // Traverse the trie from root
    for (int i = MAX_MASK_VALUE - 1; i >= 0; i--)
    {
        if ((ipv4 >> i) & 0x1)
        {
            current = parent->one;
        }
        else
        {
            current = parent->zero;
        }
        depth++;

        // There are no more nodes, break and return the highest found mask
        if (!current)
        {
            break;
        }
        else
        {
            // If node is a word end, it means IPv4 is in range of prefixes
            // Need to continue the smallest range (highest mask = depth in trie)
            if (current->word_end == true) {
                highest_mask = depth;
            }
        }
        parent = current;
    }

    return highest_mask;
}
