#include <stdio.h>
#include "ipv4_prefix.h"


int main(void) {
    ipv4_prefix_print_list();
    printf("%d\n", ipv4_add(4026531840, 24));
    printf("%d\n", ipv4_add(3690987520, 24));
    printf("%d\n", ipv4_add(2560, 4));
    printf("%d\n", ipv4_add(2560, 4));
    ipv4_prefix_print_list();
    printf("%d\n", ipv4_add(2560, 2));
    printf("%d\n", ipv4_add(2560, 6));
    printf("%d\n", ipv4_add(3690987520, 24));
    printf("%d\n", ipv4_add(3690987520, 12));
    ipv4_prefix_print_list();
    printf("%d\n", ipv4_remove(2560, 1));
    printf("%d\n", ipv4_remove(3690987520, 8));
    printf("%d\n", ipv4_remove(2560, 4));
    ipv4_prefix_print_list();
    printf("%d\n", ipv4_check(3690987610));
    printf("%d\n", ipv4_check(32858));

}