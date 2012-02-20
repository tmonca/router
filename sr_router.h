/*-----------------------------------------------------------------------------
 * File: sr_router.h
 * Date: ?
 * Authors: Guido Apenzeller, Martin Casado, Virkam V.
 * Contact: casado@stanford.edu
 *
 *---------------------------------------------------------------------------*/

#ifndef SR_ROUTER_H
#define SR_ROUTER_H

#include <netinet/in.h>
#include <sys/time.h>
#include <stdio.h>

#include "sr_protocol.h"
#include "sr_base_internal.h"
#include "cli/helper.h"



#define INIT_TTL 255
#define PACKET_DUMP_SIZE 1024

/* forward declare */


/* ----------------------------------------------------------------------------
 * struct sr_router
 *
 * Locally held state for a single virtual router's instantiation.
 *
 * -------------------------------------------------------------------------- */

struct sr_router
{
    struct sr_instance* sr;
    struct sr_vns_if* if_list; /* list of interfaces */
    struct sr_rt* routing_table; /* routing table */
    struct arp_cache_list* arp_cache;
    struct send_list* waitSend;
};

/* -- sr_main.c -- */
int sr_verify_routing_table(struct sr_instance* sr);


/* -- sr_if.c -- */
void sr_add_interface(struct sr_instance* , const char* );
void sr_set_ether_ip(struct sr_instance* , uint32_t );
void sr_set_ether_addr(struct sr_instance* , const unsigned char* );
void sr_print_if_list(struct sr_instance* );


#endif /* SR_ROUTER_H */
