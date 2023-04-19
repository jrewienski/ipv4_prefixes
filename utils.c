#include "utils.h"
#include <stdio.h>

void print_ipv4_info(const char *ipv4string)
{
    printf("%16s", ipv4string);
    printf(" => ");
    printf("%10u", ipv4_strtou32(ipv4string));
    printf(" (");
    print_ipv4_binary(ipv4_strtou32(ipv4string));
    printf(")\n");
}

uint32_t ipv4_strtou32(const char *ipv4string)
{
    uint32_t res = 0;
    uint8_t seg = 0;
    for (uint8_t i = 0; ipv4string[i] != 0; i++)
    {
        if (ipv4string[i] != '.')
        {
            seg *= 10;
            seg += ipv4string[i] - '0';
        }
        else
        {
            res <<= 8;
            res += seg;
            seg = 0;
        }
    }
    res <<= 8;
    res += seg;
    return res;
}

void print_ipv4_binary(uint32_t ipv4)
{
    for (int8_t i = 31; i >= 0; i--)
    {
        printf("%d", (ipv4 >> i) & 0x1);
        if (((i) % 8) == 0 && i != 0)
            printf(" ");
    }
}

void print_ipv4_u32(uint32_t ipv4)
{
    printf("%d.%d.%d.%d", ipv4 & 0xFF, (ipv4 >> 8) & 0xFF, (ipv4 >> 16) & 0xFF, (ipv4 >> 24) & 0xFF);
}
