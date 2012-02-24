
#include "sr_handler.h"
#include "sr_if.h"


/*--------------------------------------------------------------------- 
 * Method: sr_eth_pkt* read_ethernet_frame( uint8_t* frame, unsigned int len)
 *
 * This will take the raw bytes from the line and split them into 
 * header and payload. It returns the pointer to the new struct
 *---------------------------------------------------------------------*/

struct sr_eth_pkt* read_ethernet_frame( uint8_t* frame, unsigned int len){
   struct sr_eth_pkt* processed = (struct sr_eth_pkt*) malloc_or_die(len);
   uint8_t* tmp = (uint8_t*) malloc_or_die(len - ETH_HDR_LEN);
   tmp = (frame+14);
   processed->header = (struct sr_ethernet_hdr*) frame;
   processed->payload = tmp;
   printf("*****************   READ IN THE ETHERNET FRAME\n");
   return processed;
}

/*--------------------------------------------------------------------- 
 * Method: extract_arp_header(uint8_t* raw)
 *
 * This will take the raw arp payload and cast it to an sr_arphdr
 * It also sorts out endian issues
 *---------------------------------------------------------------------*/


struct sr_arphdr* extract_arp_header(uint8_t* raw){
   struct sr_arphdr* arp = (struct sr_arphdr*) malloc_or_die(sizeof(struct sr_arphdr));
   arp = (struct sr_arp_hdr*)raw;
   arp->ar_hrd = ntohs(arp->ar_hrd);
   arp->ar_pro = ntohs(arp->ar_pro);
   arp->ar_op = ntohs(arp->ar_op);
   return arp;
}
/*--------------------------------------------------------------------- 
 * Method: extract_arp_header(uint8_t* raw, unsigned int len)
 *
 * This will take the raw arp payload and convert it to an sr_ip_pkt
 * len is the length of the raw data in bytes.
 *---------------------------------------------------------------------*/

struct sr_ip_pkt* read_ip_pkt(uint8_t* raw, unsigned int len){
   printf("receiving %d bytes\n", len);
   assert(raw);
   struct sr_ip_pkt* pkt = (struct sr_ip_pkt*) malloc_or_die(len);
   assert(pkt);
   uint8_t* tmp = (uint8_t*) malloc_or_die(len - sizeof(struct ip));
   assert(tmp);  //tmp will be the payload
   pkt->header = (struct ip*) raw;
   tmp = raw + sizeof(struct ip); 
   pkt->payload = tmp;

   
   printf("************************   READ IN AN IP PACKET\n");
   return pkt;
}

/*--------------------------------------------------------------------- 
 * Method: handle_ip_pkt(sr_instance* sr, sr_ip_pkt* Pkt, char* interface, unsigned int len)
 *
 * A method for dealing with IP packets and assigning to correct place
 *---------------------------------------------------------------------*/

int handle_ip_pkt(struct sr_instance* sr, uint8_t* Pkt, char* interface, unsigned int len, uint8_t srcMAC[]){
   
   //actually I have to pass in a uint8_t* packet not an sr_ip_pkt...
   
   /*
    Extract the IP header
    Header actions:
    1) check the IP checksum
    -> discard if wrong
    2) check if TTL expired
    -> send ICMP type 11 (0)
    3) check if it is our message
    -> if yes jump to "OURS:"
    Not Ours:
    1) Find the next hop in rtable
    2) Decrement TTL, recalculate Checksum
    3) Assemble new packet
    4) Send on correct interface
    
    OURS:
    1) Check type of packet
    -> ICMP
    -> UDP/TCP
    
    ICMP:
    1) Check type
    2) respond as appropriate
    
    UDP/TCP
    1) Pass up the stack
    
    */
    assert(Pkt);
    assert(sr);
    assert(interface);

   //struct sr_router* subsystem = (struct sr_router*)sr_get_subsystem(sr);
   struct sr_vns_if* myIntf = sr_get_interface(sr, interface); 
   uint32_t myIP = myIntf->ip;

   struct ip* pktHdr = (struct ip*)Pkt;
   uint8_t* payload = (uint8_t*)malloc_or_die(len - 20);
   uint8_t* ptr = Pkt + 20;
   memcpy(payload, Pkt, len-20);
   
   uint32_t originIP = pktHdr->ip_src.s_addr;
   
   //first thing is to update ARP - no point wasiting useful information is there!
   time_t now = time(NULL);
   int checkArp; 
   checkArp = find_and_update(sr, originIP, srcMAC, now);
   
   if(cksum((uint8_t*)pktHdr, 20)){
      printf("@@@@@@@@@@@@@@@@@@@@@@    CHECKSUM WAS INVALID SO WE WILL DROP THE PACKET\n");
      return -2;
   }
   
   if(pktHdr->ip_len != htons(20)){
      //means there were IP options so unerachable (host or network?)
      uint8_t* data = (uint8_t*)malloc_or_die(28);
      memcpy(data, Pkt, 28);
      int tmp;
      tmp = create_ICMP_pkt(sr, interface, originIP, 3, 1, 0, 0, data, 28);
      return -3;
   }
   
   if(pktHdr->ip_ttl < 2){
      printf("@@@@@@@@@@@@@@@@@@@@@@    TTL has expired so we will send back an ICMP\n");
      //get the data (IP hdr + 8 bytes of packet)
      uint8_t* data = (uint8_t*)malloc_or_die(28);
      memcpy(data, Pkt, 28);
      int tmp;
      tmp = create_ICMP_pkt(sr, interface, originIP, 11, 0, 0, 0, data, 28);
      return -1;
   }
   
   if((uint32_t)pktHdr->ip_dst.s_addr == myIP){
      
      printf("This is my packet so I will process it\n");
      
      switch(pktHdr->ip_p) {
         case IPPROTO_ICMP:
            printf("\nThis is an IP ICMP packet of length %d\n", len);
            int datalen = len - sizeof(struct ip);
            printf("After removing the IP header we are left with %d bytes\n", datalen);
            uint8_t* ICMP = (uint8_t*) malloc_or_die(datalen);
            //ICMP = array_cpy(Pkt->payload, datalen);
            memcpy(ICMP, payload, datalen);
            int tmp;
            tmp = process_ICMP_pkt(sr, interface, originIP, ICMP, datalen); 
            return tmp;

         case IPPROTO_TCP:
            printf("TCP packet sent up the stack\n");
            sr_transport_input(Pkt);
            return 1;
         default:
            return 0;
      }
   }
   else{
      printf("**********  Not my packet, so I will forward it\n");
      return 2;
   }
   return 0;
}

/*--------------------------------------------------------------------- 
 * Method: process ICMP pkt(struct sr_instance sr*, char* interface, uint32_t srcIP, uint8_t* data, unsigned int len)
 * 
 * Proecsses an incoming ICMP packet. Passes it on to create_ICMP_pkt
 *
 * returns 1 on success, 2 on waiting for ARP, 0 on fail, -1 on interface error, -2 on checksum failure
 *---------------------------------------------------------------------*/
int process_ICMP_pkt(struct sr_instance* sr, char* interface, uint32_t srcIP, uint8_t* packet, unsigned int len){
   assert(sr);
   assert(interface);
   assert(packet);
   int rtn;
   //first thing is to do a checksum check
   if(cksum(packet, len)){
      printf("@@@@@@@@@@@@@@@@@@@@@@    CHECKSUM WAS INVALID SO WE WILL DROP THE PACKET\n");
      return -2;
   }
#if 0   
   printf("********* at %s    ICMP Packet is 0x ", __func__);
   int i ;
   for(i = 0; i < len; i++){
      printf("%x", packet[i]);
   }
   printf("\n");
#endif  
   //then cast the packet into an ICMP proto-packet
   
   struct sr_icmp_hdr* tmpH = (struct sr_icmp_hdr*) malloc_or_die(sizeof(struct sr_icmp_hdr));
   tmpH = (struct sr_icmp_hdr*)packet;
   uint16_t field1 = tmpH->field1;
   uint16_t field2 = tmpH->field2;
   
   //then identify the type. Assume we mainly have to deal with echo request (what to do with errors coming back?)
   uint8_t type = tmpH->type;
   switch(type){
      case 8:
         //call create ICMP with type echo response
         printf("len = %d\n", len);
         uint8_t* data = (uint8_t*)malloc_or_die(len - 8);
         memcpy(data, (packet+8), len-8);
         rtn = create_ICMP_pkt(sr, interface, srcIP, 0, 0, field1, field2, data, len - 8);
         break;
      default:
         printf("****** DIFFERENT TYPE OF ICMP PACKET (%d). Add code to %s to process it\n", type, __func__);
         rtn = 0;
         break;
   }
   return rtn;
}


/*--------------------------------------------------------------------- 
 * Method: create ICMP pkt(struct sr_instance sr*, char* interface, uint32_t dstIP, uint8_t type, uint8_t code, uint8_t* data)
 * 
 * Creates a new ICMP packet
 * dstIP will be used to look up ARP response. I will need functionality to cope with
 * waiting for ARP response.
 * some (most) types of ICMP will have NULL data, errors will be passed the raw IP packet
 *
 * returns 1 on success, 2 on waiting for ARP, 0 on fail, -1 on interface error
 *---------------------------------------------------------------------*/

int create_ICMP_pkt(struct sr_instance* sr, char* interface, uint32_t dstIP, uint8_t type, uint8_t code, uint16_t field1, uint16_t field2, uint8_t* data, unsigned int datalen){

   assert(data);
   assert(sr);
   assert(interface);
#if 0   
   printf("********* at %s    Payload is 0x ", __func__);
   int i ;
   for(i = 0; i < datalen; i++){
      printf("%x", data[i]);
   }
   printf("\n");
#endif   
   int len, rtn;
   switch (type){
         
      case 3:
      case 11:
         len = 8 + sizeof(struct ip) + 8; //8byte ICMP hdr, 20byte IP hdr, 8byte IP data
         break; //for now to keep it easy to follow code
      case 0:
         len = sizeof(struct sr_icmp_hdr) + datalen; // 8byte ICMP hdr, data as in echo rqst allowing for ID and sequence in data 
         break;
      default:
         len = 8; //this might not be helpful...
         
   }
   //printf("********   We're assigning %d bytes of memory\n", len);
   struct sr_icmp_hdr* packet = (struct sr_icmp_hdr*) malloc_or_die(sizeof(struct sr_icmp_hdr));
   
   // now they all get some std entries
   
   packet->type = type;
   packet->code = code;
   packet->checksum = 0x0; // needs to be 0'ed before calculation
   packet->field1 = field1;
   packet->field2 = field2;
   
   //and now we fill the other entries as appropriate

  // printf("********* at %s    After conversion to packet \n ", __func__);
  // print_icmp_pkt(packet);

   //finally we can add a checksum
   //uint16_t check = cksum((uint8_t*)packet, len);   // The problem is I can't do this cast apparently
     
   //printf("\n\n********* at %s    transferring  0x \n", __func__);
   //print_icmp_pkt(packet);  
   
   uint8_t* Pkt = array_join(packet, data, sizeof(struct sr_icmp_hdr), datalen);
   
   //memcpy(Pkt, packet, len); 
   uint16_t check = cksum(Pkt, len);
  // printf("\n\n &&&&&&&&&&   the checksum is calculated as %hu\n", check);
   
   ((struct sr_icmp_hdr*)Pkt)->checksum = check; 

   // Pkt = icmp_to_raw(packet, len); 
   
   //so we still have the issue of how to insert the checksum
   
   
   rtn = make_and_send(sr, interface, dstIP, Pkt, len, IPPROTO_ICMP);
   
   if(rtn == 2){
         printf("*************    ICMP pkt, ARP request sent\n");
         return 2;
   }
   else if (rtn == 1){
      printf("**************    ICMP packet, we had an ARP entry\n");
      return 2;
   }
   else{
      return 0;
   }
   return 0;
}

/*--------------------------------------------------------------------- 
 * Method: make_and_send(struct sr_instance sr, char * interface, uint32_t dstIP, uint8_t* payld)
 *
 * payld is what yhe IP packet will carry. len is actual length. We can assemble IP pkt
 * We will use ARP to look up the dst mac
 * Then we assemble ethernet frame
 *
 * returns 1 for sent, 2 for wait-for-ARP, 0 for fail
 *---------------------------------------------------------------------*/

int make_and_send(struct sr_instance* sr, char * interface, uint32_t dstIP, uint8_t* payld, unsigned int len, uint8_t proto){
   assert(payld);
   assert(sr);
   assert(interface);
#if 0   
   printf("********* at %s    Payload is 0x ", __func__);
   int i ;
   for(i = 0; i < len; i++){
      printf("%hhx ", payld[i]);
   }
   printf("\n");
#endif   
   struct sr_vns_if* myIntf = sr_get_interface(sr, interface);
   
   struct ip* ipHdr = (struct ip*) malloc_or_die(20);
   
   int rtn;
   
   struct in_addr src;
   struct in_addr dst;
   src.s_addr = myIntf->ip;
   dst.s_addr = dstIP;
   
   //fill in ip hdr

   //How do we set version and hl?
   ipHdr->ip_hl = 5;
   ipHdr->ip_v = 4;
   
   ipHdr->ip_tos = 0x0;
   ipHdr->ip_len = htons(len + 20);
   ipHdr->ip_id = htons(1638);
   ipHdr->ip_off = 0x0;
   ipHdr->ip_ttl = 64;
   ipHdr->ip_p = proto;   // probably always IPPROTO_ICMP for the router
   ipHdr->ip_sum = 0x0;
   ipHdr->ip_src = src;
   ipHdr->ip_dst = dst;
   
   uint16_t check = cksum((uint8_t*)ipHdr, 20);
   ipHdr->ip_sum = check;
   
   //Then we make an ethernet payload
   
   //   Tracking back from sr_arp.c:298
   //  we don't have the valid ICMP here even so go back further
   
   uint8_t* packet = (uint8_t*) malloc_or_die(len + 20);
   packet = array_join(ipHdr, payld, 20, len);
   
#if 0   
   printf("\n");
     
   for(i = 0; i < len + 20; i++){
      printf("%hhx ", packet[i]);
   }
   printf("\n*************      %s     ******************\n", __func__);
   
#endif
   rtn = arp_lookup(sr, interface, packet, dstIP, (len + 20));
      
   return rtn;
   
}

struct send_list* send_list_new(){
   struct send_list* list = (struct send_list*) malloc_or_die(sizeof(struct send_list));
   list->next = NULL;
   return list;
}

void send_list_add(struct send_list** list, uint32_t dstIP, uint8_t* payload, unsigned int len, char* iface, uint16_t type, time_t now){
   assert(list);
   assert(payload);
   assert(iface);
   
   struct send_list* new_list = (struct send_list*) malloc_or_die(sizeof(struct send_list));
   
   new_list->IP = dstIP;
   new_list->iface = iface;
   new_list->payload = payload;
   new_list->len = len;
   new_list->added = now;
   new_list->type = type;
   new_list->next = *list;
   *list= new_list;
   
   printf("**********    %s   added a packet to the queue waiting for ARP\n", __func__);
   
}

int send_list_send(struct send_list* list, struct sr_instance* sr, uint32_t* dstIP, uint8_t* dstMAC){
   assert(sr);
   int success;
   int count = 0;
   
   while(list != NULL){
      if(list->IP == dstIP){
         //break here -> list has all the bits I need
         // NO - I have to keep searching in case tehre are multiple entries.
         //solution spawn another func then return here
         success = send_packet(sr, list->iface, dstMAC, list->payload, list->len, list->type);
         count++;
      }
      list = list->next;
   }
   if(count != 0){
      //this is where I send the packet
      printf("***************     %s  I sent at least %d packets, and success is %d\n", __func__, count, success);
      return 1;
   }
   return 0;
}

int send_packet(struct sr_instance* sr, char* iface, uint8_t* dstMAC, uint8_t* payload, unsigned int len, uint16_t type){
      assert(sr);
      assert(payload);
      assert(iface);
      uint8_t* packet = (uint8_t*)malloc_or_die(len + 14);
      struct sr_ethernet_hdr* tmpHdr = (struct sr_ethernet_hdr*) malloc_or_die(14* sizeof(uint8_t));
      struct sr_vns_if* interface = sr_get_interface(sr, iface);
      
      mac_copy(dstMAC, tmpHdr->ether_dhost);
      mac_copy(interface->addr, tmpHdr->ether_shost);
      tmpHdr->ether_type = htons(type);
      memcpy(packet, tmpHdr, 14);
      packet = packet + 14;
      memcpy(packet, payload, len);
      packet = packet - 14;
      if((sr_integ_low_level_output(sr, packet, (unsigned int)(len + 14), iface)) == 0){
         printf("**********         %s   successfully sent a packet reply\n", __func__);
         return 1;
      }
      else {
         printf("\n @@@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@     %s  sending packet failed?\n", __func__);
         return 0;
      }
      return -1;
}

