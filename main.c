#include <stdio.h>
#include "ipv4_prefix.h"


int main(void) { 
    // printf("%d\n", ipv4_add(5, 0));
    //         ipv4_prefix_print_inorder();

    // printf("%d\n", ipv4_remove(5, 0));
    //     ipv4_prefix_print_inorder();


    // printf("%d\n", ipv4_add(5, 0));

    // printf("%d\n", ipv4_add(3, 0));
    // printf("%d\n", ipv4_add(1, 0));
    // printf("%d\n", ipv4_add(2, 0));
    // printf("%d\n", ipv4_add(7, 0));
    // printf("%d\n", ipv4_add(10, 0));
    // printf("%d\n", ipv4_add(12, 0));
    // ipv4_prefix_print_inorder();
    // printf("%d\n", ipv4_remove(12, 0));
    // ipv4_prefix_print_inorder();


    // return 0;
    ipv4_prefix_print_inorder();
    printf("%d\n", ipv4_add(4026531840, 24));
    printf("%d\n", ipv4_add(3690987520, 24));
    printf("%d\n", ipv4_add(2560, 4));
    printf("%d\n", ipv4_add(2560, 4));
    ipv4_prefix_print_inorder();
    printf("%d\n", ipv4_add(2560, 2));
    printf("%d\n", ipv4_add(2560, 6));
    printf("%d\n", ipv4_add(3690987520, 12));
    printf("%d\n", ipv4_add(3690987520, 24));
    printf("%d\n", ipv4_add(3690987520, 12));

    ipv4_prefix_print_inorder();
    printf("%d\n", ipv4_remove(2560, 1));
    printf("%d\n", ipv4_remove(3690987520, 8));
    printf("%d\n", ipv4_remove(2560, 4));
    ipv4_prefix_print_inorder();

    // ipv4_prefix_print_inorder();
    // printf("%d\n", ipv4_check(3690987610));
    // printf("%d\n", ipv4_check(32858));
    for (int i = 0; i < 100; i++) {
        printf("%d\n", ipv4_add(i, 0));
    }
        ipv4_prefix_print_inorder();

    printf("%d\n", ipv4_remove(10, 0));
    printf("%d\n", ipv4_remove(12, 0));
    printf("%d\n", ipv4_add(100, 0));
    ipv4_prefix_print_inorder();
}