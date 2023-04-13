#include "ipv4_prefix.h"

#include <stdio.h>
#include <stdlib.h>

static uint8_t prefixes_cnt = 0;
ipv4_prefix_node *root = NULL;

void traverse_inorder(ipv4_prefix_node *current)
{
    if (current != NULL)
    {
        traverse_inorder(current->left);
        printf("%u/%d -> ", current->base, current->mask);
        traverse_inorder(current->right);
    }
}

void ipv4_prefix_print_inorder(void)
{
    traverse_inorder(root);
    printf("end\n");
}

ipv4_prefix_status ipv4_prefix_validate(uint32_t base, uint8_t mask)
{
    if (mask > MAX_MASK_VALUE)
    {
        return IPV4_PREFIX_MASK_VALUE_ERROR;
    }

    for (uint8_t i = 0; i < mask; i++)
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

    if (prefixes_cnt >= MAX_PREFIXES)
    {
        return IPV4_PREFIX_LIST_FULL;
    }

    // Non recursive Binary Search Tree insertion
    ipv4_prefix_node *current = root;
    ipv4_prefix_node *parent = NULL;
    ipv4_prefix_node *tmp = malloc(sizeof(ipv4_prefix_node));
    tmp->base = base;
    tmp->mask = mask;
    tmp->left = NULL;
    tmp->right = NULL;

    // Searching the tree until we hit an empty leaf (NULL)
    while (current != NULL)
    {
        parent = current;
        if (base < current->base ||
            (base == current->base && mask < current->mask))
        {
            current = current->left;
        }
        else if (base > current->base ||
                 (base == current->base && mask > current->mask))
        {
            current = current->right;
        }
        else
        {
            return IPV4_PREFIX_ALREADY_EXISTS;
        }
    }

    // When we found the leaf node, update its parent
    if (parent == NULL)
    {
        root = tmp;
    }
    else if (base < parent->base ||
             (base == parent->base && mask < parent->mask))
    {
        parent->left = tmp;
    }
    else
    {
        parent->right = tmp;
    }

    prefixes_cnt++;
    return IPV4_PREFIX_OK;
}

ipv4_prefix_status ipv4_prefix_remove(uint32_t base, uint8_t mask)
{
    ipv4_prefix_status err_code;

    err_code = ipv4_prefix_validate(base, mask);
    if (err_code != IPV4_PREFIX_OK)
    {
        return err_code;
    }

    ipv4_prefix_node *current = root;
    ipv4_prefix_node *parent = NULL;
    ipv4_prefix_node *successor = NULL;
    ipv4_prefix_node *successor_parent = NULL;

    // Searching the tree until we hit an empty leaf
    while (current != NULL && (base != current->base || mask != current->mask))
    {
        parent = current;
        if (base < current->base ||
            (base == current->base && mask < current->mask))
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // If search finished at empty leaf, no node to delete
    if (current == NULL)
    {
        return IPV4_PREFIX_DOES_NOT_EXIST;
    }

    // Case if node to delete is a leaf or has only one child
    if (current->left == NULL || current->right == NULL)
    {
        // First, check which node is there (if any) and set it as successor node
        if (current->left == NULL)
        {
            successor = current->right;
        }
        else
        {
            successor = current->left;
        }

        // If there's no parent, then it means root node is being removed, need
        // to update root
        if (parent == NULL)
        {
            root = successor;
        }
        else
        {
            // else, update parent of removed node to point to its successor
            if (parent->left == current)
            {
                parent->left = successor;
            }
            else
            {
                parent->right = successor;
            }
        }

        free(current);
    }
    else
    { // If removed node has both children, we choose smallest node from
      // right
        // subtree.
        successor = current->right;
        while (successor->left != NULL)
        {
            successor_parent = successor;
            successor = successor->left;
        }

        // There's no more 'left' to go, so update the successors parent with
        // successors 'right' subtree.
        if (successor_parent != NULL)
        {
            successor_parent->left = successor->right;
        }
        else
        { // The right subtree did not have 'left'
            current->right = successor->right;
        }

        // Copy data from successor to current node and free succesory memory.
        current->base = successor->base;
        current->mask = successor->mask;
        free(successor);
    }

    prefixes_cnt--;
    return IPV4_PREFIX_OK;
}

int8_t ipv4_prefix_check(uint32_t ipv4)
{
    ipv4_prefix_node *current = root;

    // Search through BST, if we run into an empty leaf (NULL) it means the IPv4
    // is not in range of stored prefixes.
    while (current != NULL)
    {
        if ((ipv4 >> current->mask) < (current->base >> current->mask))
        {
            current = current->left;
        }
        else if ((ipv4 >> current->mask) > (current->base >> current->mask))
        {
            current = current->right;
        }
        else
        {
            return (int8_t)current->mask;
        }
    }

    return -1;
}
