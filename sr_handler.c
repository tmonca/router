
#include "sr_handler.h"

/*--------------------------------------------------------------------- 
 * Method: sr_eth_pkt* read_ethernet_frame( uint8_t* frame, unsigned int len)
 *
 * This will take the raw bytes from the line and split them into 
 * header and payload. It returns the pointer to the new struct
 *---------------------------------------------------------------------*/

struct sr_eth_pkt* read_ethernet_frame( uint8_t* frame, unsigned int len){
   struct sr_eth_pkt* processed = (struct sr_eth_pkt*) malloc_or_die(sizeof(struct sr_eth_pkt));
   uint8_t* tmp = (uint8_t*) malloc_or_die(len - ETH_HDR_LEN);
   tmp = (frame+14);
   processed->header = (struct sr_ethernet_hdr*) frame;
   processed->payload = tmp;
   printf("*****************   READ IN THE ETEHRNET FRAME\n");
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
   
   struct sr_ip_pkt* pkt = (struct sr_ip_pkt*) malloc_or_die(len);
   uint8_t* tmp = (uint8_t*) malloc_or_die(len - sizeof(struct ip));
   tmp = raw + 40; 
   
   pkt->header = (struct ip*)raw;
   pkt->payload = tmp;
   
   printf("************************   READ IN AN IP PACKET\n");
   return pkt;
}

/*--------------------------------------------------------------------- 
 * Method: handle_ip_pkt(sr_instance* sr, sr_ip_pkt* Pkt, char* interface, unsigned int len)
 *
 * A method for dealing with IP packets and assigning to correct place
 *---------------------------------------------------------------------*/

int handle_ip_pkt(struct sr_instance* sr, struct sr_ip_pkt* Pkt, char* interface, unsigned int len){
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
    
   struct sr_router* subsystem = (struct sr_router*)sr_get_subsystem(sr);

    
   struct sr_vns_if* myIntf = sr_get_interface(sr, interface); 
   uint32_t myIP = myIntf->ip;
   uint32_t originIP = Pkt->header->ip_src.s_addr;
   
   if(cksum((uint8_t*)Pkt->header, 20)){
      printf("@@@@@@@@@@@@@@@@@@@@@@    CHECKSUM WAS INVALID SO WE WILL DROP THE PACKET\n");
      return -1;
   }
   
   if(Pkt->header->ip_ttl < 2){
      printf("@@@@@@@@@@@@@@@@@@@@@@    TTL has expired so we will send back an ICMP\n");
      return -1;
   }
   
   if((uint32_t)Pkt->header->ip_dst.s_addr == myIP){
      
      printf("This is my packet so I will process it\n");
      if(Pkt->header->ip_p == IPPROTO_ICMP){
         printf("\nThis is an ICMP packet\n");
         //really we should call an ICMP processing function here. But I need to write it...
         //so for now be very dumb and assume echo request
         uint8_t* ICMP = (uint8_t*) malloc_or_die(64);
         uint8_t* offset = Pkt->payload + 4;
         ICMP = array_cpy(offset, 64);
         int tmp;
         tmp = create_ICMP_pkt(sr, interface, originIP, 8, 0, ICMP);
      }
      
      return 1;
   }
   else{
      printf("**********  Not my packet, so I will forward it\n");
      return 2;
   }
   return 0;
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

int create_ICMP_pkt(struct sr_instance* sr, char* interface, uint32_t dstIP, uint8_t type, uint8_t code, uint8_t* data){
   struct sr_icmp_pkt* packet;
   int len, rtn;
   switch (type){
         
      case 3:
      case 11:
         len = 8 + 20 + 8; //8byte ICMP hdr, 20byte IP hdr, 8byte IP data
         break; //for now to keep it easy to follow code
      case 0:
         len = 4 + sizeof(data); // 8byte ICMP hdr, data as in echo rqst allowing for ID and sequence in data 
         break;
      default:
         len = 8; //this might not be helpful...
         
   }
   packet = (struct sr_icmp_pkt*) malloc_or_die(len);
   
   // now they all get some std entries
   
   packet->type = type;
   packet->code = code;
   packet->checksum = 0x0; // needs to be 0'ed before calculation
                           //and then we fill the other entries as appropriate
   switch(type){
      case 3:
      case 11:
         packet->field1 = 0x0;
         packet->field2 = 0x0;
         packet->data = array_cpy(data, 28);
         break;
      case 0:
         packet = packet + 4;
         packet = array_cpy(data, (len-4));  //will this work? Will this copy all 3 fields?
         break;
      default:
         packet->field1 = 0x0;
         packet->field2 = 0x0;
         break;
   }
   
   //finally we can add a checksum
   uint16_t check = cksum((uint8_t*)packet, len);
   packet->checksum = check; //But did we need to finddle with endianess?
                             //now we send the packet
   rtn = make_and_send(sr, interface, dstIP, (uint8_t*)packet, len, IPPROTO_ICMP);
   
   return rtn;
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
   
   struct sr_vns_if* myIntf = sr_get_interface(sr, interface);
   
   struct ip* ipHdr = (struct ip*) malloc_or_die(20);
   
   int rtn;
   
   struct in_addr src;
   struct in_addr dst;
   src.s_addr = myIntf->ip;
   dst.s_addr = dstIP;
   
   //fill in ip hdr
   ipHdr->ip_tos = 0x0;
   ipHdr->ip_len = len + 20;
   ipHdr->ip_id = 1638;
   ipHdr->ip_off = 0x0;
   ipHdr->ip_ttl = 128;
   ipHdr->ip_p = proto;   // probably always IPPROTO_ICMP for the router
   ipHdr->ip_sum = 0x0;
   ipHdr->ip_src = src;
   ipHdr->ip_dst = dst;
   
   uint16_t check = cksum((uint8_t*)ipHdr, 20);
   ipHdr->ip_sum = check;
   
   //Then we make an ethernet payload
   
   uint8_t* payload = (uint8_t*) malloc_or_die(len + 20);
   payload = array_cpy(ipHdr, 20);
   payload = payload + 20;
   payload = array_cpy(payld, len);
   rtn = arp_lookup(sr, interface, payload, dstIP, (len + 20));
      
   return rtn;
   
}

struct send_list* send_list_new(){
   struct send_list* list = (struct send_list*) malloc_or_die(sizeof(struct send_list));
   list->next = NULL;
   return list;
}

void send_list_add(struct send_list** list, uint32_t dstIP, uint8_t* payload, unsigned int len, char* iface, uint16_t type, time_t now){
   assert(*list);
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

