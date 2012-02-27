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
#include <pthread.h>

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
    struct arp_cach_list* arp_cache_static;
    struct send_list* waitSend;
    
    //add a mutex
    pthread_mutex_t* lock;
    
};

/* -- sr_main.c -- */
int sr_verify_routing_table(struct sr_instance* sr);


/* -- sr_if.c -- */
void sr_add_interface(struct sr_instance* , const char* );
void sr_set_ether_ip(struct sr_instance* , uint32_t );
void sr_set_ether_addr(struct sr_instance* , const unsigned char* );
void sr_print_if_list(struct sr_instance* );


#if 0


pthread_create(&garbage, NULL, garbage_loop, (void*) router->arp_cache);
pthread_detach(garbage);

}

void garbage_loop(arp_cache_t* cache){
   
   /*garbage collect  */
   while (1) {
      struct timespec delay, remains;
      delay.tv_sec = ARP_GC_INTERVAL_S;
      delay.tv_nsec = 1; 
      int done = 1; 
      /*pthread_mutex_lock (router->arp_cache->Lock);*/
      
      /* so what do we need to do to garbage collect?*/
      
      done = nanosleep(&delay, &remains);
      if(done == 0) {
         printf("@ * GARBAGE COLLECTION * @\n");
         done = 1;  
      }
      else
         printf("******************\n");
   }
}
#endif
#endif /* SR_ROUTER_H */
