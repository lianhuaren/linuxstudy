#ifndef _ACE_INCLUDE_H_H
#define _ACE_INCLUDE_H_H

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */
# include	<sys/select.h>	/* for convenience */
# include	<pthread.h>

#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */


#    define ACE_Export
#    define ACE_SINGLETON_DECLARATION(T)
#    define ACE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)

# define ACE_VERSIONED_NAMESPACE_NAME
# define ACE_BEGIN_VERSIONED_NAMESPACE_DECL
# define ACE_END_VERSIONED_NAMESPACE_DECL

#  define ACE_INLINE inline

#  define ACE_TRACE(X)

      typedef unsigned char u_char;
      typedef unsigned short u_short;
      typedef unsigned int u_int;
      typedef unsigned long u_long;

      typedef unsigned char uchar_t;
      typedef unsigned short ushort_t;
      typedef unsigned int  uint_t;
      typedef unsigned long ulong_t;
	  
	typedef void *ACE_SOCKOPT_TYPE1;
	
typedef socklen_t ACE_SOCKET_LEN;	
typedef int ACE_HANDLE;
typedef ACE_HANDLE ACE_SOCKET;
# define ACE_INVALID_HANDLE -1

// #define AF_UNSPEC       0               /* unspecified */
// #define AF_UNIX         1               /* local to host (pipes, portals) */
// #define AF_INET         2               /* internetwork: UDP, TCP, etc. */

// #  define PF_UNSPEC 0
// #  define PF_INET AF_INET

// #  define SOCK_STREAM 1

typedef unsigned long         ACE_UINT32;

#   define ACE_DEFAULT_BACKLOG 5

#  define AF_ANY (-1)

# define ACE_TEXT(STRING) STRING

#define ACE_ERROR(x)



// Handle ACE_Svc_Handler
#   define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_2 _ACE_PEER_STREAM
#   define ACE_PEER_STREAM _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_ADDR typename _ACE_PEER_STREAM::PEER_ADDR

// Handle ACE_Acceptor
#   define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_ADDR typename _ACE_PEER_ACCEPTOR::PEER_ADDR

// Handle ACE_Connector
#   define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_ADDR typename ACE_PEER_CONNECTOR::PEER_ADDR
#   define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_ADDR_TYPEDEF::sap_any

// Handle ACE_SOCK_*
#   define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor
#   define ACE_SOCK_CONNECTOR ACE_SOCK_Connector
#   define ACE_SOCK_STREAM ACE_SOCK_Stream
#   define ACE_SOCK_DGRAM ACE_SOCK_Dgram
#   define ACE_SOCK_DGRAM_BCAST ACE_SOCK_Dgram_Bcast
#   define ACE_SOCK_DGRAM_MCAST ACE_SOCK_Dgram_Mcast

#endif