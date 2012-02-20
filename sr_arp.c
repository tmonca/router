
#include "sr_arp.h"
#include "sr_protocol.h"
#include <string.h>
#include "sr_if.h"
#include "sr_router.h"

/*
   creates a new arp_cache_list and returns a pointer
*/

 struct arp_cache_list* create_cache_new(){
   struct arp_cache_list* new = (struct arp_cache_list*) malloc_or_die(sizeof(struct arp_cache_list));
   new->next = NULL;
   return new;
}

/*
 adds a new entry at start of list (just after head)
 sets correct creation time
 then returns the pointer to the new entry
 */

void add_cache_entry(struct arp_cache_list** headRef, uint32_t IP, uint8_t MAC[6], time_t created){
  struct arp_cache_list* new_entry = (struct arp_cache_list*) malloc_or_die(sizeof(struct arp_cache_list));
  //arp_cache_list* Tmp = (arp_cache_list*) malloc_or_die(sizeof(arp_cache_list)); 
  new_entry->IP = IP;
  mac_copy(MAC, new_entry->MAC);
  new_entry->created = created;
  new_entry->next = *headRef;
  *headRef= new_entry;
  
}

/*
 Looks through the cache list
 updates the first match
 returns 1 if it found a match
 or 0 if nothing has been found (or list doesn't exist yet)
 */

int find_and_update(struct arp_cache_list* head, uint32_t IP, uint8_t MAC[6], time_t now){
  
  while (head != NULL){
    if (head->IP == IP){
      break;
    }
    head = head->next;
  }
  if(head != NULL){
    mac_copy(MAC, head->MAC);
    head->created = now;
    printf("***********      %s  updated an entry in the cache\n", __func__);
    return 1;
  }
  else{
     printf("***********  I didn't already have it in cache, so I will add it\n");
     add_cache_entry(&head, IP, MAC, now);
    return 0;
  }
  
}

uint8_t* find_mac_in_cache(struct arp_cache_list* head, uint32_t IP){
  unsigned char* MAC = (unsigned char*) malloc_or_die(6* sizeof(unsigned char));
  
  while (head != NULL){
    if(head->IP == IP){
      break;
    }
    head = head->next;
  }
  if(head != NULL){
    printf("******************    %s  Found a MAC address\n", __func__);
    
    mac_copy(head->MAC, MAC);
    /* Hopefully this isn't going to go wrong if the entry is subsequently deleted? */
    
    return MAC;
  }
  return NULL;
}

/*
 calculate the length of the arp cache (ignoring head)
 rturns the count
 */

int get_cache_length(struct arp_cache_list* head){
  int length;
  struct arp_cache_list* current = head->next;
  while(current != NULL){
    length++;
  }
  return length;
  
}

/* remove all entries with this IP address
 returns the number of entries removed
 */

int remove_cache_entry(struct arp_cache_list* head, uint32_t IP){
  int count = 0;
  struct arp_cache_list* current = head;
  while (current != NULL){
    if(current->next->IP == IP){
      //remove it
      current->next = current->next->next;
      count++;
    }
  }
  return count;
}


// CODE BELOW HERE MAY BE WRONG

//generic arp handling function

// ARP packets can only be doing 2 things:

// requesting a response (e.g. a request) - these can only be sent for correct interface
//     (but they can also be used to update the cache entry for that sender)

// replying to a request - this means a request had been sent. But for safety we only
//    do anything if this is the correct interface. In which case we have to find
//    any packets we had queued waiting for this response

int process_arp(struct sr_instance* sr, char* intf, struct sr_arphdr* arp){
   //1) is this our packet? Or perhaps we should check that before we pass it?
   //2) Then we use it to update our ARP cache
   //3) Then check if it is a request and reply to it

   assert(sr);
   assert(intf);
   assert(arp);
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);

   struct sr_vns_if* interface = sr_get_interface(sr, intf);
   //check this is a well-formed ARP packet

   if(arp->ar_hrd != ARPHDR_ETHER){
      printf("@@@@@@@@@@@@@@@@@@@@@@  %s  not an ethernet packet\n", __func__);
      return -1;
   }
   if(arp->ar_pro != ETHERTYPE_IP){
      printf("@@@@@@@@@@@@@@@@@@@@@@ %s  I expected IP in this ARP packet\n", __func__);
      return -1;
   }
   if(arp->ar_hln != 6){
      printf("@@@@@@@@@@@@@@@@@@@@@@ %s  wrong ethernet address length\n", __func__);
      return -1;
   }
   if(arp->ar_pln != 4){
      printf("@@@@@@@@@@@@@@@@@@@@@@ %s  wrong IP address length\n", __func__);
      return -1;
   }

   if(interface->ip == arp->ar_tip) {
      //this is our packet
      //look for an existing entry (and update if needed)
      time_t now = time(NULL);
      if(find_and_update(sub->arp_cache, arp->ar_sip, arp->ar_sha, now)){
         printf("*****************  %s   Found an entry and updated it \n", __func__);
      }
      
      if(arp->ar_op == ARP_REQUEST){
         
         //create a reply - how? 
         if(send_arp_reply(sr, intf, arp)){
            return 2;
         }
         else{
            return -1;
         }
      }
      else if(arp->ar_op == ARP_REPLY){
         //do what?
      }
      else{
         printf("@@@@@@@@@@@@@@@@@@@@@@@@@    %s   this was a malformed ARP packet (or I have gone wrong)\n", __func__);
         return -1;
      }
   }
   //what do we do if this wasn't our packet?
   else{
      return -1;
   }
   return -1;

}

/*
   create an arp reply, then send it out 
   return 1 if it succeeds
   return 0 if it fails
*/

int send_arp_reply(struct sr_instance* sr, char* intf, struct sr_arphdr* arp){
   assert(arp);
   assert(sr);
   int arp_pkt_len;
   arp_pkt_len = ETH_HDR_LEN + sizeof(struct sr_arphdr);
   
   struct sr_vns_if * interface;
   interface = sr_get_interface(sr, intf);
   
   struct sr_arphdr* tmparp = (struct sr_arphdr*) malloc_or_die(28*sizeof(uint8_t));
   
   //use 'arp' to create the reply
   tmparp->ar_hrd = htons(arp->ar_hrd);
   tmparp->ar_pro = htons(arp->ar_pro);
   tmparp->ar_op = htons(ARP_REPLY);  
   tmparp->ar_hln = arp->ar_hln;
   tmparp->ar_pln = arp->ar_pln;
   mac_copy(arp->ar_sha, tmparp->ar_tha);
   tmparp->ar_tip = arp->ar_sip;
   mac_copy(interface->addr, tmparp->ar_sha);
   tmparp->ar_sip = interface->ip;
   
   struct sr_ethernet_hdr* tmpHdr = (struct sr_ethernet_hdr*) malloc_or_die(14* sizeof(uint8_t));
   mac_copy(tmparp->ar_tha, tmpHdr->ether_dhost);
   mac_copy(tmparp->ar_sha, tmpHdr->ether_shost);
   tmpHdr->ether_type = htons(ETHERTYPE_ARP);

   uint8_t* newPkt = (uint8_t*) malloc_or_die(arp_pkt_len);
   // How do we join the pieces?
   newPkt = create_ethernet_frame(tmpHdr, tmparp, 28);
   //pad the frame somehow?
   //now we send it using sr_send_packet
   if((sr_integ_low_level_output(sr, newPkt, (unsigned int)arp_pkt_len, intf)) == 0){
      printf("**********         %s   successfully sent an ARP reply\n", __func__);
      return 1;
   }
   else {
      printf("\n @@@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@     %s  sending ARP reply failed?\n", __func__);
      return 0;
   }
   return 1;

}

//need  arp_lookup(struct sr_instance sr, char* interface, uint8_t* payload, uint32_t dstIP, unsigned int len)
//This will take the dstIP, look it up in the ARP cache. If it's there, get the MAC
//If it isn't there it will have to send an arp request. Use send_arp_request as below
//
//If you have the MAC, you can create the ethernet frame, then send

//retun will be 1 for success, 2 for ARP, 0 for fail

int arp_lookup(struct sr_instance * sr, char* interface, uint8_t* payload, uint32_t dstIP, unsigned int len){
   struct sr_ethernet_hdr* ethHdr = (struct sr_ethernet_hdr*) malloc_or_die(sizeof(struct sr_ethernet_hdr));
   struct sr_vns_if* intf = sr_get_interface(sr, interface);
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);

   uint8_t src[6];
   mac_copy(intf->addr, ethHdr->ether_shost);
   
   uint8_t* dst;
   dst = find_mac_in_cache(sub->arp_cache, dstIP);
   if(dst == NULL){
      //we need to send off an arp request
      return 2;
   }
   else{
      dst = array_cpy(&ethHdr->ether_dhost, 6);
      return 1;
   }
   return 0;
}

//need a send_arp_request fuction that also queues the raw ether packet ready to send

int send_arp_request(struct sr_instance * sr, char* interface, uint8_t* payload, uint32_t dstIP, unsigned int len){
   // create an ARP request bound for 0xffffff/dstIP, source as in interface
   // add a send_list entry for payload, dstIP, now
   time_t now = time(NULL);
   struct send_list* new = (struct send_list*)malloc_or_die(sizeof(struct send_list));
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
   
   
}


//which in turn means we need a structure to store the pending requests



