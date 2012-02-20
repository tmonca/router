#ifndef SR_HELP_H
#define SR_HELP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "sr_protocol.h"
#include <time.h>



void mac_copy(uint8_t in[], uint8_t out[]);

uint8_t* array_cpy(void*, unsigned int);

uint8_t* create_ethernet_frame(void*, void*, unsigned int);

uint16_t cksum(uint8_t*, int );

time_t now();

#endif

