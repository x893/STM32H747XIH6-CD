/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Global definitions for the INET interface module.
 *
 * Version:	@(#)if.h	1.0.2	04/18/93
 *
 * Authors:	Original taken from Berkeley UNIX 4.3, (c) UCB 1982-1988
 *		Ross Biro
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _LINUX_IF_H
#define _LINUX_IF_H

#include <lwip/sockets.h>
#include "kernel_misc.h"

#define	IFNAMSIZ	16

/**
 * enum net_device_flags - &struct net_device flags
 *
 * These are the &struct net_device flags, they can be set by drivers, the
 * kernel and some can be triggered by userspace. Userspace can query and
 * set these flags using userspace utilities but there is also a sysfs
 * entry available for all dev flags which can be queried and set. These flags
 * are shared for all types of net_devices. The sysfs entries are available
 * via /sys/class/net/<dev>/flags. Flags which can be toggled through sysfs
 * are annotated below, note that only a few flags can be toggled and some
 * other flags are always preserved from the original net_device flags
 * even if you try to set them via sysfs. Flags which are always preserved
 * are kept under the flag grouping @IFF_VOLATILE. Flags which are volatile
 * are annotated below as such.
 *
 * You should have a pretty good reason to be extending these flags.
 *
 * @IFF_UP: interface is up. Can be toggled through sysfs.
 * @IFF_BROADCAST: broadcast address valid. Volatile.
 * @IFF_DEBUG: turn on debugging. Can be toggled through sysfs.
 * @IFF_LOOPBACK: is a loopback net. Volatile.
 * @IFF_POINTOPOINT: interface is has p-p link. Volatile.
 * @IFF_NOTRAILERS: avoid use of trailers. Can be toggled through sysfs.
 *	Volatile.
 * @IFF_RUNNING: interface RFC2863 OPER_UP. Volatile.
 * @IFF_NOARP: no ARP protocol. Can be toggled through sysfs. Volatile.
 * @IFF_PROMISC: receive all packets. Can be toggled through sysfs.
 * @IFF_ALLMULTI: receive all multicast packets. Can be toggled through
 *	sysfs.
 * @IFF_MASTER: master of a load balancer. Volatile.
 * @IFF_SLAVE: slave of a load balancer. Volatile.
 * @IFF_MULTICAST: Supports multicast. Can be toggled through sysfs.
 * @IFF_PORTSEL: can set media type. Can be toggled through sysfs.
 * @IFF_AUTOMEDIA: auto media select active. Can be toggled through sysfs.
 * @IFF_DYNAMIC: dialup device with changing addresses. Can be toggled
 *	through sysfs.
 * @IFF_LOWER_UP: driver signals L1 up. Volatile.
 * @IFF_DORMANT: driver signals dormant. Volatile.
 * @IFF_ECHO: echo sent packets. Volatile.
 */
enum net_device_flags {
    IFF_UP              = 1<<0,  /* sysfs */
    IFF_BROADCAST       = 1<<1,  /* volatile */
    IFF_DEBUG           = 1<<2,  /* sysfs */
    IFF_LOOPBACK        = 1<<3,  /* volatile */
    IFF_POINTOPOINT     = 1<<4,  /* volatile */
    IFF_NOTRAILERS      = 1<<5,  /* sysfs */
    IFF_RUNNING         = 1<<6,  /* volatile */
    IFF_NOARP           = 1<<7,  /* sysfs */
    IFF_PROMISC         = 1<<8,  /* sysfs */
    IFF_ALLMULTI        = 1<<9,  /* sysfs */
    IFF_MASTER          = 1<<10, /* volatile */
    IFF_SLAVE           = 1<<11, /* volatile */
    IFF_MULTICAST       = 1<<12, /* sysfs */
    IFF_PORTSEL         = 1<<13, /* sysfs */
    IFF_AUTOMEDIA       = 1<<14, /* sysfs */
    IFF_DYNAMIC         = 1<<15, /* sysfs */
    IFF_LOWER_UP        = 1<<16, /* volatile */
    IFF_DORMANT         = 1<<17, /* volatile */
    IFF_ECHO            = 1<<18, /* volatile */
};

#define IFF_UP              IFF_UP
#define IFF_BROADCAST       IFF_BROADCAST
#define IFF_DEBUG           IFF_DEBUG
#define IFF_LOOPBACK        IFF_LOOPBACK
#define IFF_POINTOPOINT     IFF_POINTOPOINT
#define IFF_NOTRAILERS      IFF_NOTRAILERS
#define IFF_RUNNING         IFF_RUNNING
#define IFF_NOARP           IFF_NOARP
#define IFF_PROMISC         IFF_PROMISC
#define IFF_ALLMULTI        IFF_ALLMULTI
#define IFF_MASTER          IFF_MASTER
#define IFF_SLAVE           IFF_SLAVE
#define IFF_MULTICAST       IFF_MULTICAST
#define IFF_PORTSEL         IFF_PORTSEL
#define IFF_AUTOMEDIA       IFF_AUTOMEDIA
#define IFF_DYNAMIC         IFF_DYNAMIC

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */

struct ifreq {
#define IFHWADDRLEN	6
    union
    {
        char    ifrn_name[IFNAMSIZ];        /* if name, e.g. "en0" */
    } ifr_ifrn;

    union {
        struct  sockaddr ifru_addr;
        struct  sockaddr ifru_dstaddr;
        struct  sockaddr ifru_broadaddr;
        struct  sockaddr ifru_netmask;
        struct  sockaddr ifru_hwaddr;
        short   ifru_flags;
        int     ifru_ivalue;
        int     ifru_mtu;
        char    ifru_slave[IFNAMSIZ];	/* Just fits the size */
        char    ifru_newname[IFNAMSIZ];
        void __user *   ifru_data;
    } ifr_ifru;
};

#define	ifr_name    ifr_ifrn.ifrn_name  /* interface name   */
#define	ifr_hwaddr  ifr_ifru.ifru_hwaddr    /* MAC address      */
#define	ifr_addr    ifr_ifru.ifru_addr  /* address      */
#define	ifr_dstaddr ifr_ifru.ifru_dstaddr   /* other end of p-p lnk */
#define	ifr_broadaddr   ifr_ifru.ifru_broadaddr /* broadcast address    */
#define	ifr_netmask ifr_ifru.ifru_netmask   /* interface net mask   */
#define	ifr_flags   ifr_ifru.ifru_flags /* flags        */
#define	ifr_metric  ifr_ifru.ifru_ivalue    /* metric       */
#define	ifr_mtu     ifr_ifru.ifru_mtu   /* mtu          */
#define	ifr_map     ifr_ifru.ifru_map   /* device map       */
#define	ifr_slave   ifr_ifru.ifru_slave /* slave device     */
#define	ifr_data    ifr_ifru.ifru_data  /* for use by interface */
#define	ifr_ifindex ifr_ifru.ifru_ivalue    /* interface index  */
#define	ifr_bandwidth   ifr_ifru.ifru_ivalue    /* link bandwidth   */
#define	ifr_qlen    ifr_ifru.ifru_ivalue    /* Queue length     */
#define	ifr_newname ifr_ifru.ifru_newname   /* New name         */
#define	ifr_settings    ifr_ifru.ifru_settings  /* Device/proto settings*/

#endif /* _LINUX_IF_H */
