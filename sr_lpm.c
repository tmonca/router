#include "sr_lpm.h"
#include <string.h>

//methods for Longest Prefix match

/*******************************************************************
 * longest prefix
 *
 * takes an IP address and a router and returns the correct output
 * interface.   Needs to be able to handle 0.0.0.0
 * NB we assume IP is already in correct host order
 ******************************************************************/
char* longest_prefix(struct sr_instance * sr, uint32_t IP){
    assert(sr);
    struct sr_router* subs = (struct sr_router*)sr_get_subsystem(sr);
    
    char * outport = (char*)malloc_or_die(sr_IFACE_NAMELEN);
    uint32_t tmp1;
    uint32_t tmp2;
    uint32_t longest = 0;
    
    struct sr_rt* rt_walk = subs->routing_table;
    if(rt_walk == NULL) { /* in other words no table exists */
        printf("@@@@@@@@@@@@@@@    can't do longest prefix as no routing table\n");
        return NULL;
    }
    
    /*
    
    For each entry in the routing table
    
    (assume they are in descending order? => longest match is first?)
    
    Check for next hop
    if = send to that port
    Take the mask.
    &= it with the IP address.
    Check for significant 1s (check for equals mask?)
    if so send to that port
    Then if no match, assume default route?
    send to default port
    
    */
    
    printf("************    We're looking for ");
    print_ip(IP);
    while (rt_walk != NULL){

       if(IP == rt_walk->gw.s_addr){
          strcpy(outport, rt_walk->interface);
          printf("Direct IP so return interface %s\n", outport);
          break;
       }
      // printf("Trying mask ");
       uint32_t tmp1 = rt_walk->mask.s_addr;
     //  print_ip(tmp1);
       
       tmp2 = IP;
       tmp2 &= tmp1;
       tmp1 = rt_walk->gw.s_addr;
       tmp2 &= tmp1;
       
      // printf("after masking we have ");
      // print_ip(tmp2);
      // print_ip(tmp1);
       
       if(tmp2 > longest){
          printf("longest ");
          print_ip(tmp2);
          longest = tmp2;
          strcpy(outport, rt_walk->interface);
       } 
       
       rt_walk = rt_walk->next;
       
    }
    
    //nothing found so we return NULL ?
    printf("So we return interface %s\n", outport);
    return outport;
    
}

