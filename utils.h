#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdint.h>

uint32_t ipv4_strtouint32(const char *ipv4string);
void print_ipv4binary(uint32_t ipv4);
void print_ipv4u32(uint32_t ipv4);
void print_ipv4_info(const char *ipv4string);

#endif /* _UTILS_H_ */
