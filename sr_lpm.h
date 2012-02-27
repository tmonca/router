#ifndef SR_LPM_H
#define SR_LPM_H

//this will be the header for longest prefix matching

/*For each entry in the routing table, IPv4 performs a bit-wise logical AND operation between the destination IPv4 address and the Network Mask field. The result is compared with the Destination field of the entry for a match.

As described in Chapter 4, "Subnetting," the result of the bit-wise logical AND operation is:

For each bit in the subnet mask that is set to 1, copy the corresponding bit from the destination IPv4 address to the result.

For each bit in the subnet mask that is set to 0, set the corresponding bit in the result to 0.

IPv4 compiles the list of matching routes and selects the route that has the longest match (that is, the route with the highest number of bits set to 1 in the subnet mask). The longest matching route is the most specific route to the destination IPv4 address. If the router finds multiple routes with the longest matches (for example, multiple routes to the same address prefix), the router uses the lowest metric to select the best route. If the metrics are the same, IPv4 chooses the interface that is first in the binding order.
 
 Still want to know what I do with the gateway? When is it actually used?
 
 We will build on the struct in sr_rt
 
 struct sr_rt
 {
 struct in_addr dest;
 struct in_addr gw;
 struct in_addr mask;
 char   interface[SR_NAMELEN];
 struct sr_rt* next;
 };

 
 Add a bool for type (static = 0 or dynamic = 1)
 
 
we will need the following CLI hooks:
 Adds a route to the appropriate routing table.
void rtable_route_add( struct sr_instance* sr,
                      uint32_t dest, uint32_t gw, uint32_t mask,
                      void* intf,
                      int is_static_route ) {
    fprintf( stderr, "not yet implemented: rtable_route_add\n" );
}

Removes the specified route from the routing table, if present. 
int rtable_route_remove( struct sr_instance* sr,
                        uint32_t dest, uint32_t mask,
                        int is_static ) {
    fprintf( stderr, "not yet implemented: rtable_route_remove\n" );
    return 0 ;
}

Remove all routes from the router. 
void rtable_purge_all( struct sr_instance* sr ) {
    fprintf( stderr, "not yet implemented: rtable_purge_all\n" );
}
 Remove all routes of a specific type from the router.
void rtable_purge( struct sr_instance* sr, int is_static ) {
    fprintf( stderr, "not yet implemented: rtable_purge\n" );
}

Need functions for doing LPM (returning an outgoing interface?)
 
 We use the mask to find longest set of matches. e.g. 
 
 So we need an ordered list?
 
 We also need to remember to do ntohl
 
 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "sr_base_internal.h"
#include "sr_router.h"
#include "sr_protocol.h"
#include "sr_help.h"
#include "sr_if.h"
#include "sr_rt.h"


char* longest_prefix(struct sr_instance *, uint32_t);


#endif
