/*-----------------------------------------------------------------------------
 * file:  sr_integration.c
 * date:  Tue Feb 03 11:29:17 PST 2004
 * Author: Martin Casado <casado@stanford.edu>
 *
 * Description:
 *
 * Methods called by the lowest-level of the network system to talk with
 * the network subsystem.
 *
 * This is the entry point of integration for the network layer.
 *
 *---------------------------------------------------------------------------*/

#include <stdlib.h>

#include <assert.h>

#include "sr_vns.h"
#include "sr_base_internal.h"
#include "sr_router.h"
#include "sr_integration.h"
#include "sr_arp.h"
#include "sr_rt.h"
#include "sr_handler.h"

#ifdef _CPUMODE_
#include "sr_cpu_extension_nf2.h"
#endif

struct sr_instance* get_sr() {
   struct sr_instance* sr;
   
   sr = sr_get_global_instance( NULL );
   assert( sr );
   return sr;
}

/*-----------------------------------------------------------------------------
 * Method: sr_integ_init(..)
 * Scope: global
 *
 *
 * First method called during router initialization.  Called before connecting
 * to VNS, reading in hardware information etc.
 *
 *---------------------------------------------------------------------------*/

void sr_integ_init(struct sr_instance* sr)
{
    printf(" ** sr_integ_init(..) called \n");
    struct sr_router* subsystem = (struct sr_router*)malloc_or_die(sizeof(struct sr_router));
    assert(subsystem);
    sr_set_subsystem(get_sr(), subsystem);
    
    subsystem->sr = sr;   // hopefully this isn't a circular definition
    
    //now create the piecces
    
    subsystem->routing_table = (struct sr_rt*)malloc_or_die(sizeof(struct sr_rt));
    subsystem->arp_cache = (struct arp_cache_list*)malloc_or_die(sizeof(struct arp_cache_list));
    subsystem->waitSend = NULL;
    
} /* -- sr_integ_init -- */

/*-----------------------------------------------------------------------------
 * Method: sr_integ_hw_setup(..)
 * Scope: global
 *
 * Called after all initial hardware information (interfaces) have been
 * received.  Can be used to start subprocesses (such as dynamic-routing
 * protocol) which require interface information during initialization.
 *
 *---------------------------------------------------------------------------*/

void sr_integ_hw_setup(struct sr_instance* sr)
{
    printf(" ** sr_integ_hw(..) called \n");
} /* -- sr_integ_hw_setup -- */

/*---------------------------------------------------------------------
 * Method: sr_integ_input(struct sr_instance*,
 *                        uint8_t* packet,
 *                        char* interface)
 * Scope:  Global
 *
 * This method is called each time the router receives a packet on the
 * interface.  The packet buffer, the packet length and the receiving
 * interface are passed in as parameters. The packet is complete with
 * ethernet headers.
 *
 * Note: Both the packet buffer and the character's memory are handled
 * by sr_vns_comm.c that means do NOT delete either.  Make a copy of the
 * packet instead if you intend to keep it around beyond the scope of
 * the method call.
 *
 *---------------------------------------------------------------------*/

void sr_integ_input(struct sr_instance* sr,
        const uint8_t * packet/* borrowed */,
        unsigned int len,
        const char* interface/* borrowed */)
{
    /* -- INTEGRATION PACKET ENTRY POINT!-- */

    printf(" ** sr_integ_input(..) called \n");
                      
   assert(sr);
   assert(packet);
   assert(interface);
   struct sr_router* subsystem = (struct sr_router*)sr_get_subsystem(sr);
   
   printf("*** -> Received packet of length %d \n",len);
   int check;
   struct sr_eth_pkt* Frame;
   Frame = (struct sr_eth_pkt*) malloc_or_die(len);    
   Frame = read_ethernet_frame(packet, len);
   
   printf("\n passed the processed frame back to %s\n", __func__);  
   
   uint16_t tmpType;
   tmpType = ntohs(Frame->header->ether_type);
   
   /* Now we have to check the type and handle accordingly */
   if(tmpType == ETHERTYPE_ARP){
      struct sr_arphdr* arp;
      arp = extract_arp_header(Frame->payload);
      printf("******************     ARP PACKET    **************\n");
      if( process_arp(sr, interface, arp)){
         printf("*******   ******    we processed the ARP  *******   *******\n");
      }
   }
   else if (tmpType == ETHERTYPE_IP){
      struct sr_ip_pkt* Pkt = read_ip_pkt(Frame->payload, (len-14));
      
      printf("***@***@***@***@***     IP PACKET    @***@***@***@***\n");
      check = handle_ip_pkt(sr, Pkt, interface, (len-14));
      
      switch (check) {
         case 2:
            printf("******************   This isn't my packet so I will now call forwarding code\n");
            break;
         case 1:
            printf("******************   This packet belongs to me so I will now process it\n");
            break;
         case -1:
            printf("@@@@@@@@@@@@@@@@   This packet has expired so I send back an ICMP\n");
            break;
         case -2:
            printf("@@@@@@@@@@@@@@@@   This packet failed checksum check...\n");
            break;
         default:
            printf("@@@******@@@@********   something else happened\n");
            break;
      }
      
   } 
   else{
      printf("\n@@@@@@@@@@@@@@@@@@@@@@@  UNRECOGNISED PACKET TYPE %hu\n", tmpType);
   }
   return;
   

} /* -- sr_integ_input -- */

/*-----------------------------------------------------------------------------
 * Method: sr_integ_add_interface(..)
 * Scope: global
 *
 * Called for each interface read in during hardware initialization.
 * struct sr_vns_if is defined in sr_base_internal.h
 *
 *---------------------------------------------------------------------------*/

void sr_integ_add_interface(struct sr_instance* sr,
                            struct sr_vns_if* vns_if/* borrowed */)
{
    printf(" ** sr_integ_add_interface(..) called \n");
    
    struct sr_vns_if* intf = (struct sr_vns_if*)malloc_or_die(sizeof(struct sr_vns_if));
    memcpy(intf, vns_if, sizeof(struct sr_vns_if));
    struct sr_router* subsystem = (struct sr_router*)sr_get_subsystem(sr);
    if(subsystem->if_list == NULL){
       printf("***** Blank list so add an entry %s\n", intf->name);
       subsystem->if_list = intf;
       subsystem->if_list->next = NULL;
       return;
    }
    intf->next = subsystem->if_list;
    subsystem->if_list = intf;
    printf("*****  List already exists. Add an entry %s (became %s)\n", intf->name, subsystem->if_list->name);

    return;
    
} /* -- sr_integ_add_interface -- */


/*-----------------------------------------------------------------------------
 * Method: sr_integ_low_level_output(..)
 * Scope: global
 *
 * Send a packet to VNS to be injected into the topology
 *
 *---------------------------------------------------------------------------*/

int sr_integ_low_level_output(struct sr_instance* sr /* borrowed */,
                             uint8_t* buf /* borrowed */ ,
                             unsigned int len,
                             const char* iface /* borrowed */)
{
#ifdef _CPUMODE_
    return sr_cpu_output(sr, buf /*lent*/, len, iface);
#else
    return sr_vns_send_packet(sr, buf /*lent*/, len, iface);
#endif /* _CPUMODE_ */
} /* -- sr_vns_integ_output -- */

/*-----------------------------------------------------------------------------
 * Method: sr_integ_destroy(..)
 * Scope: global
 *
 * For memory deallocation pruposes on shutdown.
 *
 *---------------------------------------------------------------------------*/

void sr_integ_destroy(struct sr_instance* sr)
{
    printf(" ** sr_integ_destroy(..) called \n");
} /* -- sr_integ_destroy -- */

/*-----------------------------------------------------------------------------
 * Method: sr_integ_findsrcip(..)
 * Scope: global
 *
 * Called by the transport layer for outgoing packets generated by the
 * router.  Expects source address in network byte order.
 *
 *---------------------------------------------------------------------------*/

uint32_t sr_integ_findsrcip(uint32_t dest /* nbo */)
{
    fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    fprintf(stderr, "!!! Tranport layer called ip_findsrcip(..) this must be !!!\n");
    fprintf(stderr, "!!! defined to return the correct source address        !!!\n");
    fprintf(stderr, "!!! given a destination                                 !!!\n ");
    fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    assert(0);

    /* --
     * e.g.
     *
     * struct sr_instance* sr = sr_get_global_instance();
     * struct my_router* mr = (struct my_router*)
     *                              sr_get_subsystem(sr);
     * return my_findsrcip(mr, dest);
     * -- */

    return 0;
} /* -- ip_findsrcip -- */

/*-----------------------------------------------------------------------------
 * Method: sr_integ_ip_output(..)
 * Scope: global
 *
 * Called by the transport layer for outgoing packets that need IP
 * encapsulation.
 *
 *---------------------------------------------------------------------------*/

uint32_t sr_integ_ip_output(uint8_t* payload /* given */,
                            uint8_t  proto,
                            uint32_t src, /* nbo */
                            uint32_t dest, /* nbo */
                            int len)
{
    fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    fprintf(stderr, "!!! Tranport layer called sr_integ_ip_output(..)        !!!\n");
    fprintf(stderr, "!!! this must be defined to handle the network          !!!\n ");
    fprintf(stderr, "!!! level functionality of transport packets            !!!\n ");
    fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    assert(0);

    /* --
     * e.g.
     *
     * struct sr_instance* sr = sr_get_global_instance();
     * struct my_router* mr = (struct my_router*)
     *                              sr_get_subsystem(sr);
     * return my_ip_output(mr, payload, proto, src, dest, len);
     * -- */

    return 0;
} /* -- ip_integ_route -- */

/*-----------------------------------------------------------------------------
 * Method: sr_integ_close(..)
 * Scope: global
 *
 *  Called when the router is closing connection to VNS.
 *
 *---------------------------------------------------------------------------*/

void sr_integ_close(struct sr_instance* sr)
{
    printf(" ** sr_integ_close(..) called \n");
}  /* -- sr_integ_close -- */
