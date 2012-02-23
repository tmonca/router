
#ifndef SR_ARP_H
#define SR_ARP_H


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "sr_router.h"
#include "sr_protocol.h"
#include "sr_help.h"
#include "sr_if.h"


//struct for arp entry linked list

struct arp_cache_list{

   struct arp_cache_list* previous;
   struct arp_cache_list* next;
   time_t created;
   uint8_t MAC[6];
   uint32_t IP;

};

struct arp_cache_list* create_cache_new();
void add_cache_entry(struct sr_instance*, uint32_t, uint8_t MAC[6], time_t);
uint8_t* find_mac_in_cache(struct sr_instance*, uint32_t);
int find_and_update(struct sr_instance*, uint32_t, uint8_t MAC[6], time_t);

int get_cache_length(struct arp_cache_list*);
int remove_cache_entry(struct arp_cache_list*, uint32_t);

int process_arp(struct sr_instance*, char*, struct sr_arphdr*);

int send_arp_reply(struct sr_instance*, char*, struct sr_arphdr*);

int arp_lookup(struct sr_instance*, char*, uint8_t*, uint32_t, unsigned int );

int send_arp_request(struct sr_instance * , char* , uint8_t* , uint32_t , unsigned int);


#endif   //end of sr_arp.h
