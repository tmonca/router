
#include "sr_help.h"
#include "cli/helper.h"


void mac_copy(uint8_t in[], uint8_t out[]){
 // printf("MAC in is ");
  int i;
  for(i = 0; i < 6; i++){
    //printf("0x%hhx:", in[i]);
    out[i] = in[i];
  }
  //printf(" and MAC out is ");
  for(i = 0; i < 6; i++){
   // printf("0x%hhx:", out[i]);
  }
 // printf("\n");
}

void mac_broadcast(uint8_t dst[]){
  int i;
  for(i = 0; i < 6; i++){
     dst[i] = 0xff;
  }
}

//replace all calls to the following with memcpy (Allowing for void* rtn type)

uint8_t* array_cpy(void* in, unsigned int len){
   uint8_t* output = (uint8_t*) malloc_or_die(len);
   uint8_t* input = (uint8_t*) in;
   int i;
   for(i = 0; i < len; i++){
      output[i] = input[i];
   }
   return output;
}

uint8_t* array_cpy_v(void* in, unsigned int len){
   uint8_t* output = (uint8_t*) malloc_or_die(len);
   uint8_t* input = (uint8_t*) in;
   int i;
   for(i = 0; i < len; i++){
      printf("%x->", input[i]);
      output[i] = input[i];
      printf("%x ", output[i]);
   }
   printf("\n");
   return output;
}

uint8_t* array_join_v(void* in1, void* in2, unsigned int len1, unsigned int len2){
   uint8_t* output = (uint8_t*) malloc_or_die(len1 + len2);
   uint8_t* input1 = (uint8_t*) in1;
   uint8_t* input2 = (uint8_t*) in2;
   
   int i;
   printf("\n");
   for(i = 0; i < len1 ; i++){
      printf("%x->", input1[i]);
      output[i] = input1[i];
      printf("%x ", output[i]);
   }
   printf("\n");
   for(i = 0; i < len2 ; i++){
      printf("%x->", input2[i]);
      output[i+len1] = input2[i];
      printf("%x ", output[i+len1]);
   }
   printf("\n");
   return output;
}

uint8_t* array_join(void* in1, void* in2, unsigned int len1, unsigned int len2){
   uint8_t* output = (uint8_t*) malloc_or_die(len1 + len2);
   uint8_t* input1 = (uint8_t*) in1;
   uint8_t* input2 = (uint8_t*) in2;
   
   int i;
     for(i = 0; i < len1 ; i++){
      output[i] = input1[i];
   }
   for(i = 0; i < len2 ; i++){
      output[i+len1] = input2[i];
   }
   return output;
}

/*
  Assigns a chunk of memory then copies the hdr, then the payload
 
  NB len is the length of the payload
*/

uint8_t* create_ethernet_frame(void* hdr, void* payld, unsigned int len){
  assert(hdr);
  assert(payld);
  uint8_t* header = (uint8_t*) hdr;
  uint8_t* payload = (uint8_t*) payld;
  
  uint8_t* frame = (uint8_t*) malloc_or_die(len + 14);
  
  int i;
  for(i = 0; i < 14; i++){
    frame[i] = header[i];
  }
  for(i = 14; i < (len + 14); i++){
    frame[i] = payload[i-14];
  }
  return frame;
}

/*
 **************************************************************************
 Function: cksum (calculates IP cksum)
 ***************************************************************************
 */

//original had struct ip* ip. Try chanigng to the more generic uint8_t* data

uint16_t cksum(uint8_t* data, int len){
   uint32_t sum = 0;  /* assume 32 bit long, 16 bit short */
   uint16_t* ptr;
   ptr = (uint16_t*)data;
   while(len > 1){
      sum += *ptr;
      ptr++;
      if(sum & 0x80000000)   /* if high order bit set, fold */
         sum = (sum & 0xFFFF) + (sum >> 16);
      len -= 2;
   }
  
   if(len)       /* take care of left over byte */
      sum += (uint16_t) *ptr;
  
   while(sum>>16)
      sum = (sum & 0xFFFF) + (sum >> 16);
  
   return ~sum;
}

/* loosely based on one from TCP/IP Illustrated Vol. 2(1995) by Gary R. Wright and W. Richard
 Stevens. Page 236. Also looked at the lwIP checksum code */
 
time_t now(){
   time_t now = time(NULL);
   return now;
}

void print_icmp_pkt(struct sr_icmp_hdr* packet){
   
   printf("ICMP packet type = %hhx, code = %hhx, check = %hx\n field1 is: %hx field2 is: %hx", packet->type, packet->code, packet->checksum, packet->field1, packet->field2);
   
   printf("\n\n");
}

void print_ip(uint32_t IP){
   uint8_t tmp;
   uint32_t masked;
   printf("IP address ");
   
   /* byte 1*/
   masked = IP;
   masked &= 0xff000000;
   masked = masked>>24;
   tmp=(uint8_t)masked;
   printf("%hhu.", tmp);
   
   /* byte 2*/
   masked = IP;
   masked &= 0x00ff0000;
   masked = masked>>16;
   tmp=(uint8_t)masked;
   printf("%hhu.", tmp);

   /* byte 3*/
   masked = IP;
   masked &= 0x0000ff00;
   masked = masked>>8;
   tmp=(uint8_t)masked;
   printf("%hhu.", tmp);
   
   /* byte 4*/
   masked = IP;
   masked &= 0x000000ff;
   tmp=(uint8_t)masked;
   printf("%hhu.", tmp);
   
   printf(" \n");
   return;
   
}

/* ***************************************

Checks to see if IP is one of my interfaces. 
returns 1 if yes, 0 if no

**************************************** */

int check_my_interface(struct sr_instance* sr, uint32_t IP){
   assert(sr);
   struct sr_router* subs = (struct sr_router*)sr_get_subsystem(sr);
   struct sr_vns_if* intf_walker = subs->if_list;
   while(intf_walker != NULL){
      if(intf_walker->ip == IP){
         //this is our intf)
         return 1;
      }
      intf_walker = intf_walker->next;
   }
   return 0;
}


