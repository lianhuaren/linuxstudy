#ifndef	ACL_VSTREAM_INCLUDE_H
#define	ACL_VSTREAM_INCLUDE_H

#ifdef  __cplusplus
extern "C" {
#endif

# define	ACL_SOCKET	SOCKET
# define	ACL_SOCKET_INVALID	INVALID_SOCKET	

# define	ACL_ETIMEDOUT		WSAETIMEDOUT
# define	ACL_ENOMEM		WSAENOBUFS
# define	ACL_EINVAL		WSAEINVAL

# define	ACL_EWOULDBLOCK		WSAEWOULDBLOCK
# define	ACL_EAGAIN		ACL_EWOULDBLOCK	/* xxx */

#define	ACL_VSTREAM_EOF		(-1)	

# define ACL_API

typedef int (WINAPI *acl_close_socket_fn)(ACL_SOCKET);
typedef int (WINAPI *acl_recv_fn)(ACL_SOCKET, char *, int, int);
typedef int (WINAPI *acl_send_fn)(ACL_SOCKET, const char *, int, int);

static acl_recv_fn   __sys_recv   = recv;
static acl_send_fn   __sys_send   = send;
	
#  define ACL_HAS_POLL
typedef int (WINAPI *acl_poll_fn)(struct pollfd*, unsigned long, int);
// ACL_API void acl_set_poll(acl_poll_fn fn);

static acl_poll_fn __sys_poll = WSAPoll;


#ifdef  __cplusplus
}
#endif

#endif