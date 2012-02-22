/*
 *  Copyright (c) 1998, 1999, 2000 Mike D. Schiffman <mike@infonexus.com>
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * sr_protocol.h
 *
 */

#ifndef SR_PROTOCOL_H
#define SR_PROTOCOL_H

#ifdef _LINUX_
#include <stdint.h>
#endif /* _LINUX_ */

#include <sys/types.h>
#include <arpa/inet.h>
 
#ifndef IP_MAXPACKET
#define IP_MAXPACKET 65535
#endif

/* FIXME
 * ohh how lame .. how very, very lame... how can I ever go out in public
 * again?! /mc 
 */
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1
#endif
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 2
#endif
#ifdef _CYGWIN_
#ifndef __BYTE_ORDER
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif
#endif
#ifdef _LINUX_
#ifndef __BYTE_ORDER
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif
#endif
#ifdef _SOLARIS_
#ifndef __BYTE_ORDER
#define __BYTE_ORDER __BIG_ENDIAN
#endif
#endif
#ifdef _DARWIN_
#ifndef __BYTE_ORDER
#define __BYTE_ORDER __BIG_ENDIAN
#endif
#endif

/*
 * Structure of an internet header, naked of options.
 */
struct ip
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ip_hl:4;		/* header length */
    unsigned int ip_v:4;		/* version */
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int ip_v:4;		/* version */
    unsigned int ip_hl:4;		/* header length */
#else
#error "Byte ordering not specified " 
#endif 
    uint8_t ip_tos;			/* type of service */
    uint16_t ip_len;			/* total length */
    uint16_t ip_id;			/* identification */
    uint16_t ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
    uint8_t ip_ttl;			/* time to live */
    uint8_t ip_p;			/* protocol */
    uint16_t ip_sum;			/* checksum */
    struct in_addr ip_src, ip_dst;	/* source and dest address */
  } __attribute__ ((packed)) ;

/* 
 *  Ethernet packet header prototype.  Too many O/S's define this differently.
 *  Easy enough to solve that and define it here.
 */

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif

#ifndef IP_ADDR_LEN
#define IP_ADDR LEN 4
#endif

struct sr_ethernet_hdr
{
    uint8_t  ether_dhost[ETHER_ADDR_LEN];    /* destination ethernet address */
    uint8_t  ether_shost[ETHER_ADDR_LEN];    /* source ethernet address */
    uint16_t ether_type;                     /* packet type ID */
} __attribute__ ((packed)) ;

#ifndef ARPHDR_ETHER
#define ARPHDR_ETHER    1
#endif

#ifndef IPPROTO_ICMP
#define IPPROTO_ICMP            0x0001  /* ICMP protocol */
#endif

#ifndef ETHERTYPE_IP
#define ETHERTYPE_IP            0x0800  /* IP protocol */
#endif

#ifndef ETHERTYPE_ARP
#define ETHERTYPE_ARP           0x0806  /* Addr. resolution protocol */
#endif

#define ARP_REQUEST 1
#define ARP_REPLY   2

#define ETH_HDR_LEN 14

struct sr_arphdr 
{
    unsigned short  ar_hrd;             /* format of hardware address   */
    unsigned short  ar_pro;             /* format of protocol address   */
    unsigned char   ar_hln;             /* length of hardware address   */
    unsigned char   ar_pln;             /* length of protocol address   */
    unsigned short  ar_op;              /* ARP opcode (command)         */
    uint8_t   ar_sha[ETHER_ADDR_LEN];   /* sender hardware address      */
    uint32_t        ar_sip;             /* sender IP address            */
    uint8_t   ar_tha[ETHER_ADDR_LEN];   /* target hardware address      */
    uint32_t        ar_tip;             /* target IP address            */
} __attribute__ ((packed)) ;

struct sr_eth_pkt {
  struct sr_ethernet_hdr* header; /* keep a copy of the original header */
  uint8_t* payload;  /* */
};

struct sr_ip_pkt
{
  struct ip* header;
  uint8_t* payload;
};

struct sr_icmp_hdr
{
   uint8_t type;
   uint8_t code;
   uint16_t checksum;
   //uint8_t code;
   //uint8_t type;
   /* these are all assuming that it comes in "backwards" */
   
};

struct sr_icmp_pkt
{
   struct sr_icmp_hdr header;
   uint8_t* data;
};


#endif /* -- SR_PROTOCOL_H -- */
