//
//  unpipc.h
//  unp01
//
//  Created by temp on 2018/6/3.
//  Copyright © 2018年 temp. All rights reserved.
//

#ifndef unpipc_h
#define unpipc_h

# include	<pthread.h>

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

#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */

#define	MAXLINE		4096	/* max text line length */
#define	SA	struct sockaddr
#define	SERV_PORT		 9877
#define	LISTENQ		1024

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))

typedef int status_t;

enum {
    OK,
    NO_ERROR = 0,
    BAD_INDEX,
    BAD_VALUE,
    INVALID_OPERATION,
    NOT_ENOUGH_DATA,
    UNKNOWN_ERROR,
    DAMAGED_DATA,
};

void	 Close(int);

/* prototypes for our socket wrapper functions: see {Sec errors} */
int		 Accept(int, SA *, socklen_t *);
void	 Bind(int, const SA *, socklen_t);
void	 Connect(int, const SA *, socklen_t);
void	 Getpeername(int, SA *, socklen_t *);
void	 Getsockname(int, SA *, socklen_t *);
void	 Getsockopt(int, int, int, void *, socklen_t *);
int		 Isfdtype(int, int);
void	 Listen(int, int);
#ifdef	HAVE_POLL
int		 Poll(struct pollfd *, unsigned long, int);
#endif
ssize_t	 Readline(int, void *, size_t);
ssize_t	 Readn(int, void *, size_t);
ssize_t	 Recv(int, void *, size_t, int);
ssize_t	 Recvfrom(int, void *, size_t, int, SA *, socklen_t *);
ssize_t	 Recvmsg(int, struct msghdr *, int);
int		 Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void	 Send(int, const void *, size_t, int);
void	 Sendto(int, const void *, size_t, int, const SA *, socklen_t);
void	 Sendmsg(int, const struct msghdr *, int);
void	 Setsockopt(int, int, int, const void *, socklen_t);
void	 Shutdown(int, int);
int		 Sockatmark(int);
int		 Socket(int, int, int);
void	 Socketpair(int, int, int, int *);
void	 Writen(int, void *, size_t);

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);

static void	err_doit(int, int, const char *, va_list);

const char		*Inet_ntop(int, const void *, char *, size_t);
void			 Inet_pton(int, const char *, void *);


/* prototypes for our pthread wrapper functions */
void	 Pthread_attr_init(pthread_attr_t *);
void	 Pthread_attr_destroy(pthread_attr_t *);
void	 Pthread_attr_setdetachstate(pthread_attr_t *, int);
void	 Pthread_attr_setscope(pthread_attr_t *, int);
void	 Pthread_create(pthread_t *, const pthread_attr_t *,
                        void * (*)(void *), void *);
void	 Pthread_join(pthread_t, void **);
void	 Pthread_detach(pthread_t);
void	 Pthread_kill(pthread_t, int);
void	 Pthread_setcancelstate(int, int *);

void	 Pthread_mutexattr_init(pthread_mutexattr_t *);
void	 Pthread_mutexattr_destroy(pthread_mutexattr_t *);
void	 Pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
void	 Pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
void	 Pthread_mutex_destroy(pthread_mutex_t *);
void	 Pthread_mutex_lock(pthread_mutex_t *);
void	 Pthread_mutex_unlock(pthread_mutex_t *);

void	 Pthread_condattr_init(pthread_condattr_t *);
void	 Pthread_condattr_destroy(pthread_condattr_t *);
void	 Pthread_condattr_setpshared(pthread_condattr_t *, int);
void	 Pthread_cond_broadcast(pthread_cond_t *);
void	 Pthread_cond_signal(pthread_cond_t *);
void	 Pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
void	 Pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *,
                                const struct timespec *);

void	 Pthread_key_create(pthread_key_t *, void (*)(void *));
void	 Pthread_setspecific(pthread_key_t, const void *);
void	 Pthread_once(pthread_once_t *, void (*)(void));
long	 pr_thread_id(pthread_t *);


void	 Set_concurrency(int);

#endif /* unpipc_h */
