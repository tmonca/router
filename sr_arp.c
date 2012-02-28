
#include "sr_arp.h"
#include "sr_protocol.h"
#include <string.h>
#include "sr_if.h"
#include "sr_router.h"
#include "sr_handler.h"


/*
 adds a new entry at start of list (just after head)
 sets correct creation time
 then returns the pointer to the new entry
 */

void add_cache_entry(struct sr_instance* sr, uint32_t IP, uint8_t MAC[6], time_t created){
   assert(sr);
   struct sr_router* subs = (struct sr_router*)sr_get_subsystem(sr);
   assert(subs);
   struct arp_cache_list* new_entry = (struct arp_cache_list*) malloc_or_die(sizeof(struct arp_cache_list));
   new_entry->IP = IP;
   mac_copy(MAC, new_entry->MAC);
   new_entry->created = created;
   
   if(subs->arp_cache == NULL){ /* there was no pre-extant cache*/
      new_entry->next = NULL;
      subs->arp_cache = new_entry;
      return;
   }
   //headRef = &subs->arp_cache;
   new_entry->next = subs->arp_cache;
   subs->arp_cache = new_entry;
  
}

/*
 adds a new entry at start of list (just after head)
 sets correct creation time
 then returns the pointer to the new entry
 */

void add_static_entry(struct sr_instance* sr, uint32_t IP, uint8_t MAC[6], time_t created){
   assert(sr);
   struct sr_router* subs = (struct sr_router*)sr_get_subsystem(sr);
   assert(subs);
   struct arp_cache_list* new_entry = (struct arp_cache_list*) malloc_or_die(sizeof(struct arp_cache_list));
   new_entry->IP = IP;
   mac_copy(MAC, new_entry->MAC);
   new_entry->created = created;
   
   if(subs->arp_cache_static == NULL){ /* there was no pre-extant cache*/
      new_entry->next = NULL;
      subs->arp_cache_static = new_entry;
      return;
   }
   //headRef = &subs->arp_cache;
   new_entry->next = subs->arp_cache_static;
   subs->arp_cache_static = new_entry;
   
}


/*
 Looks through the cache list
 updates the first match
 returns 1 if it found a match
 or 0 if nothing has been found (or list doesn't exist yet)
 */

int find_and_update(struct sr_instance* sr, uint32_t IP, uint8_t MAC[6], time_t now){
   assert(sr);
   struct sr_router* subs = (struct sr_router*)sr_get_subsystem(sr);
   assert(subs);
   struct arp_cache_list* head = (struct arp_cache_list*)malloc_or_die(sizeof(struct arp_cache_list));
   head = subs->arp_cache;
  
  
  //  CHANGING TO sr_instance from arp_cache_list
  
  
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
     add_cache_entry(sr, IP, MAC, now);
    return 0;
  }
  
}

uint8_t* find_mac_in_cache(struct sr_instance* sr, uint32_t IP){
   assert(sr);
   struct sr_router* subs = (struct sr_router*)sr_get_subsystem(sr);
   assert(subs);
   struct arp_cache_list* head = (struct arp_cache_list*)malloc_or_die(sizeof(struct arp_cache_list));
   head = subs->arp_cache;
   unsigned char* MAC = (unsigned char*) malloc_or_die(6* sizeof(unsigned char));
  
   while (head != NULL){
    printf(" ~~~~~~~~~~~~~~~~~~   IP = 0x%x, looking for 0x%x\n", head->IP, IP);
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

int get_cache_length(struct sr_instance* sr, unsigned int type){
  assert(sr);
  struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
  assert(sub);
  int length = 0;
  if(type == 1){  /* dynamic cache */
     struct arp_cache_list* current = sub->arp_cache;
     while(current != NULL){
        printf("length %d ", length);
        length++;
        current = current->next;
     }
  }
  else { /* assume it must be static*/
     struct arp_cache_list* current = sub->arp_cache_static;
     while(current != NULL){
        length++;
        current = current->next;
     }
  }
  return length;
}

/* remove all entries with this IP address
 returns the number of entries removed
 */

int remove_cache_entry(struct sr_instance* sr, uint32_t IP){
  assert(sr);
  struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
  assert(sub);
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

/***********************************************
*  2 identical functions to delete all entries 
*  in eitehr static or dynamic cache. Both 
*  return the number of entries deleted. These
*  will be hooked into the two arp_cache_*_purge
*  commands in cli.
************************************************/

int delete_arp_cache(struct sr_instance*){
   assert(sr);
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
   assert(sub);
   int count = 0;
   
   
   return 0;
}
int delete_static_cache(struct sr_instance*){
   assert(sr);
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
   assert(sub);
   int count = 0;
   
   
   return 0;
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

   if(check_my_interface(sr, arp->ar_tip)){
      //this is our packet
      //look for an existing entry (and update if needed)
      time_t now = time(NULL);
      if(find_and_update(sr, arp->ar_sip, arp->ar_sha, now)){
         printf("*****************  %s   Found an entry and updated it \n", __func__);
      }
      
      if(arp->ar_op == ARP_REQUEST){
         
         //create a reply - how? 
         if(send_arp_reply(sr, intf, arp)){
            return 2;
         }
         else{
            return -2;
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
   //struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
   int rtn;

   //uint8_t src[6];
   mac_copy(intf->addr, ethHdr->ether_shost);
#if 0   
   printf("********* at %s    Payload is 0x ", __func__);
   
   for (i = 0; i < len; i++){
      printf("%x", payload[i]);
   }
   printf("\n");
#endif   
   uint8_t* dst;
   dst = find_mac_in_cache(sr, dstIP);
   if(dst != NULL){
      mac_copy(dst, ethHdr->ether_dhost);
      mac_copy(intf->addr, ethHdr->ether_shost);
      ethHdr->ether_type = htons(ETHERTYPE_IP);
      
      printf("*************      %s    We got  ARP    ******************\n", __func__);
      
      uint8_t* newPkt = (uint8_t*) malloc_or_die(len+14);
      newPkt = create_ethernet_frame(ethHdr, payload, len);
      //pad the frame somehow?
      //now we send it using sr_integ_low_level_output

      if((sr_integ_low_level_output(sr, newPkt, len+14, interface)) == 0){
         printf("**********         %s   successfully sent a packet\n", __func__);
         rtn = 1;
      }
      else {
         printf("\n @@@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@     %s  sending packet failed?\n", __func__);
         rtn =  0;
      }

      
      return 1;

   }
   //else we need to send off an arp request
   printf("*************      %s    send ARP    ******************\n", __func__);
   rtn = send_arp_request(sr, interface, payload, dstIP, len);
   if(rtn == 1){
      
      return 2;
   }
   else{
      return 0;
   }
   return 0;
}

//need a send_arp_request fuction that also queues the raw ether packet ready to send

int send_arp_request(struct sr_instance * sr, char* iface, uint8_t* payload, uint32_t dstIP, unsigned int len){
   // create an ARP request bound for 0xffffff/dstIP, source as in interface
   // add a send_list entry for payload, dstIP, now
   printf("************   %s    so sending ARP request\n", __func__);
   
   time_t now = time(NULL);
   int rtn = 0;
   struct send_list* new = (struct send_list*)malloc_or_die(sizeof(struct send_list));
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
   struct sr_arphdr* tmparp = (struct sr_arphdr*) malloc_or_die(28*sizeof(uint8_t));
   struct sr_vns_if* intf = sr_get_interface(sr, iface);
   int arp_pkt_len;
   arp_pkt_len = ETH_HDR_LEN + sizeof(struct sr_arphdr);
   
   //make the ARP header for an ARP request
   
   tmparp->ar_hrd = htons(ARPHDR_ETHER);
   tmparp->ar_pro = htons(ETHERTYPE_IP);
   tmparp->ar_op = htons(ARP_REQUEST);  
   tmparp->ar_hln = 6;
   tmparp->ar_pln = 4;
   mac_broadcast(tmparp->ar_tha);
   tmparp->ar_tip = dstIP;
   mac_copy(intf->addr, tmparp->ar_sha);
   tmparp->ar_sip = intf->ip;
   
   struct sr_ethernet_hdr* tmpHdr = (struct sr_ethernet_hdr*) malloc_or_die(14* sizeof(uint8_t));
   mac_copy(tmparp->ar_tha, tmpHdr->ether_dhost);
   mac_copy(tmparp->ar_sha, tmpHdr->ether_shost);
   tmpHdr->ether_type = htons(ETHERTYPE_ARP);

   uint8_t* newPkt = (uint8_t*) malloc_or_die(arp_pkt_len);
   newPkt = create_ethernet_frame(tmpHdr, tmparp, 28);
   //pad the frame somehow?
   //now we send it using sr_integ_low_level_output
   printf("£££££££££££££££££££££££££££££££££\n");
   if((sr_integ_low_level_output(sr, newPkt, (unsigned int)arp_pkt_len, iface)) == 0){
      printf("**********         %s   successfully sent an ARP request\n", __func__);
      rtn = 1;
   }
   else {
      printf("\n @@@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@     %s  sending ARP request failed?\n", __func__);
      rtn =  0;
   }
   
   //before leaving make an entry in the send list   
   send_list_add(sub->waitSend, dstIP, payload, len, iface, ETHERTYPE_ARP, now);
   
   return rtn;
}

//which in turn means we need a structure to store the pending requests

#if 1
void arp_cache_cleanup(struct sr_instance *sr){
   
   struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);   /*garbage collect  */
   while (1) {
      assert(sr);
      struct sr_router* sub = (struct sr_router*)sr_get_subsystem(sr);
      assert(sub);
      struct timespec delay, remains;
      delay.tv_sec = ARP_GC_INTERVAL_S;
      delay.tv_nsec = 0; 
      int done = 1; 
      int count;
      /* so what do we need to do to garbage collect?*/
      printf("PEEKABOO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      done = nanosleep(&delay, &remains);
      if(done == 0) {
         count = get_cache_length(sr, 1);
         pthread_mutex_lock(sub->lock);
         printf("@ * GARBAGE COLLECTION * @\n");
         time_t now = time(NULL);
         time_t then;
         struct arp_cache_list* step;
         struct arp_cache_list* prev;
         struct arp_cache_list* tmp;
         step = sub->arp_cache;
         prev = sub->arp_cache;
         while(step != NULL){
            if (difftime(now, sub->arp_cache->created) > ARP_TIMEOUT_INTERVAL){
               printf("*****  $$$$  ***** %s:  We had %d ARP entries and current one timed out\n",__func__, count);
               if(count >1){
                  prev->next = step->next;  //remove the link to current
                  count--;
                  }
                  else{
                     printf("@@@@      ********     KILL THE CACHE\n");
                     count = 0;
                     sub->arp_cache = NULL;
                  }
            }
            else {
               printf("Nothing to delete in ARP cache\n");
               prev = step;
            }
            step = step->next;
         }
         
         pthread_mutex_unlock(sub->lock);
         done = 1;  
      }
      else
         printf("******************\n");
   }
}

#endif

