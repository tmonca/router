#ifndef SR_HELP_H
#define SR_HELP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "sr_protocol.h"
#include <time.h>
#include "sr_base_internal.h"
#include "sr_router.h"



void mac_copy(uint8_t in[], uint8_t out[]);
void mac_broadcast(uint8_t[]);

uint8_t* array_cpy(void*, unsigned int);
uint8_t* array_cpy_v(void*, unsigned int);
uint8_t* array_join_v(void*, void*, unsigned int, unsigned int);
uint8_t* array_join(void*, void*, unsigned int, unsigned int);

uint8_t* create_ethernet_frame(void*, void*, unsigned int);

uint16_t cksum(uint8_t*, int );

time_t now();

void print_icmp_pkt(struct sr_icmp_hdr*);

void print_ip(uint32_t);

int check_my_interface(struct sr_instance*, uint32_t);

#endif

