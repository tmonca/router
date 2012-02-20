
#ifndef SR_HANDLER_H
#define SR_HANDLER_H

#include <netinet/in.h>
#include <sys/time.h>
#include <stdio.h>

#include "sr_protocol.h"
#include "sr_base_internal.h"
#include "cli/helper.h"
#include "sr_router.h"
#include "sr_arp.h"
#include "sr_help.h"

struct send_list
{
  uint8_t* payload;
  uint32_t IP;
  char* interface;
  unsigned int len;
  time_t added;
  send_list* next; 
};


/*  Toby's amazing functions  */

struct sr_eth_pkt* read_ethernet_frame( uint8_t*, unsigned int);
struct sr_arphdr* extract_arp_header(uint8_t*);
struct sr_ip_pkt* read_ip_pkt(uint8_t*, unsigned int );
int handle_ip_pkt(struct sr_instance* , struct sr_ip_pkt* , char* , unsigned int );

int make_and_send(struct sr_instance*, char*, uint32_t, uint8_t*, unsigned int, uint8_t);
int create_ICMP_pkt(struct sr_instance*, char*, uint32_t, uint8_t, uint8_t, uint8_t*);

#endif /* SR_HANDLER_H */
