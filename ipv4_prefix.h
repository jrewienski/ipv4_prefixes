#ifndef _IPV4_PREFIX_H_
#define _IPV4_PREFIX_H_
#include <stdbool.h>
#include <stdint.h>

#define MAX_MASK_VALUE 32
#define IPV4_NOT_IN_PREFIXES_RANGE -1

typedef enum
{
    IPV4_PREFIX_OK,                 /**< Action performed on IPv4 prefix was successful. */
    IPV4_PREFIX_BASE_MASK_MISMATCH, /**< Base and mask for IPv4 did not match. */
    IPV4_PREFIX_MASK_VALUE_ERROR,   /**< IPv4 mask was invalid (>32). */
    IPV4_PREFIX_ALREADY_EXISTS,     /**< IPv4 prefix already exists on the list. */
    IPV4_PREFIX_DOES_NOT_EXIST,     /**< IPv4 prefix does not exit on the list. */
    IPV4_PREFIX_INSUFFICIENT_MEM    /**< Not enough memory to allocate IPv4 prefix. */
} ipv4_prefix_status;

typedef struct ipv4_prefix_trie_node
{
    struct ipv4_prefix_trie_node *zero;
    struct ipv4_prefix_trie_node *one;
    bool word_end;
} ipv4_prefix_trie_node;

/* Adds a prefix to list.
 *
 * @param[in]   base        IPv4 Prefix base
 * @param[in]   mask        IPv$ Prefix mask
 *
 * @return      IPV4_PREFIX_OK                      If IPv4 prefix was added successfully.
 * @return      IPV4_PREFIX_BASE_MASK_MISMATCH      If IPv4 Base and Mask did not match.
 * @return      IPV4_PREFIX_MASK_VALUE_ERROR        If Mask was bigger than 32.
 * @return      IPV4_PREFIX_ALREADY_EXISTS          If prefix already exists in the list.
 */
ipv4_prefix_status ipv4_prefix_add(uint32_t base, uint8_t mask);

/* Removes a prefix from the list.
 *
 * @param[in]   base        IPv4 Prefix base
 * @param[in]   mask        IPv$ Prefix mask
 *
 * @return      IPV4_PREFIX_OK                      If IPv4 prefix was removed successfully.
 * @return      IPV4_PREFIX_BASE_MASK_MISMATCH      If IPv4 Base and Mask did not match.
 * @return      IPV4_PREFIX_MASK_VALUE_ERROR        If Mask was bigger than 32.
 * @return      IPV4_PREFIX_DOES_NOT_EXIST          If prefix was not on the list exists.
 */
ipv4_prefix_status ipv4_prefix_remove(uint32_t base, uint8_t mask);

/* Checks if IPv4 address belongs to any of prefixes on the list.
 *
 * @param[in]   ipv4        IPv4 address
 *
 * @return      mask    Mask of lowest prefix that covers given IPv4 address.
 * @return      -1      If IPv4 Base and Mask did not match.
 */
int8_t ipv4_prefix_check(uint32_t ipv4);

#endif /* _IPV4_PREFIX_H_ */
