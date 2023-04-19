#include <stdio.h>
#include "utils.h"
#include <string.h>

void print_ipv4_info(const char *ipv4string)
{
    printf("%16s", ipv4string);
    printf(" => ");
    printf("%10u", ipv4_strtouint32(ipv4string));
    printf(" (");
    print_ipv4binary(ipv4_strtouint32(ipv4string));
    printf(")\n");
}

uint32_t ipv4_strtouint32(const char *ipv4string)
{
    uint32_t res = 0;
    uint8_t seg = 0;
    for (uint8_t i = 0; i < strlen(ipv4string); i++)
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

void print_ipv4binary(uint32_t ipv4)
{
    for (int8_t i = 31; i >= 0; i--)
    {
        printf("%d", (ipv4 >> i) & 0x1);
        if (((i) % 8) == 0 && i != 0)
            printf(" ");
    }
}

void print_ipv4u32(uint32_t ipv4)
{
    uint8_t seg1, seg2, seg3, seg4;
    seg1 = ipv4 & 0xFF;
    seg2 = (ipv4 >> 8) & 0xFF;
    seg3 = (ipv4 >> 16) & 0xFF;
    seg4 = (ipv4 >> 24) & 0xFF;
    printf("%d.%d.%d.%d", seg4, seg3, seg2, seg1);
}
