//
//  main.cpp
//  unp01
//
//  Created by temp on 15/10/9.
//  Copyright (c) 2015年 temp. All rights reserved.
//

#include <iostream>
#include	"unpipc.h"
// #include	"mutex.h"
#include	"condition.h"

//int
//main(int argc, char **argv)
//{
//    int					i, maxi, maxfd, listenfd, connfd, sockfd;
//    int					nready, client[FD_SETSIZE];
//    ssize_t				n;
//    fd_set				rset, allset;
//    char				line[MAXLINE];
//    socklen_t			clilen;
//    struct sockaddr_in	cliaddr, servaddr;
//    
//    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
//    
//    bzero(&servaddr, sizeof(servaddr));
//    servaddr.sin_family      = AF_INET;
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    servaddr.sin_port        = htons(SERV_PORT);
//    
//    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
//    
//    Listen(listenfd, LISTENQ);
//    
//    maxfd = listenfd;			/* initialize */
//    maxi = -1;					/* index into client[] array */
//    for (i = 0; i < FD_SETSIZE; i++)
//        client[i] = -1;			/* -1 indicates available entry */
//    FD_ZERO(&allset);
//    FD_SET(listenfd, &allset);
//    /* end fig01 */
//    
//    /* include fig02 */
//    for ( ; ; ) {
//        rset = allset;		/* structure assignment */
//        nready = Select(maxfd+1, &rset, NULL, NULL, NULL);
//        
//        if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
//            clilen = sizeof(cliaddr);
//            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
//
//            printf("new client: %s, port %d\n",
//                   "new",
//                   ntohs(cliaddr.sin_port));
//#ifdef	NOTDEF
//            printf("new client: %s, port %d\n",
//                   Inet_ntop(AF_INET, &cliaddr.sin_addr, 4),
//                   ntohs(cliaddr.sin_port));
//#endif
//            
//            for (i = 0; i < FD_SETSIZE; i++)
//                if (client[i] < 0) {
//                    client[i] = connfd;	/* save descriptor */
//                    break;
//                }
//            if (i == FD_SETSIZE)
//                err_quit("too many clients");
//            
//            FD_SET(connfd, &allset);	/* add new descriptor to set */
//            if (connfd > maxfd)
//                maxfd = connfd;			/* for select */
//            if (i > maxi)
//                maxi = i;				/* max index in client[] array */
//            
//            if (--nready <= 0)
//                continue;				/* no more readable descriptors */
//        }
//        
//        for (i = 0; i <= maxi; i++) {	/* check all clients for data */
//            if ( (sockfd = client[i]) < 0)
//                continue;
//            if (FD_ISSET(sockfd, &rset)) {
//                if ( (n = Readline(sockfd, line, MAXLINE)) == 0) {
//                    /*4connection closed by client */
//                    Close(sockfd);
//                    FD_CLR(sockfd, &allset);
//                    client[i] = -1;
//                } else
//                    Writen(sockfd, line, n);
//                
//                if (--nready <= 0)
//                    break;				/* no more readable descriptors */
//            }
//        }
//    }
//}




#define	MAXNITEMS 		1000000
#define	MAXNTHREADS			100

/* globals shared by threads */
int		nitems;				/* read-only by producer and consumer */
int		buff[MAXNITEMS];
struct {
    pthread_mutex_t	mutex;
    int				nput;	/* next index to store */
    int				nval;	/* next value to store */
} put = { PTHREAD_MUTEX_INITIALIZER };

struct {
   pthread_mutex_t	mutex;
   pthread_cond_t	cond;
   int				nready;	/* number ready for consumer */
} nready = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER };
// struct {
// //    pthread_mutex_t	mutex;
// //    pthread_cond_t	cond;
//     int				nready;	/* number ready for consumer */
// } nready;
// Condition mCondition;
// Mutex mMutex;

/* end globals */

void	*produce(void *), *consume(void *);

/* include main */
int
main(int argc, char **argv)
{
    int			i, nthreads, count[MAXNTHREADS];
    pthread_t	tid_produce[MAXNTHREADS], tid_consume;
    
    if (argc != 3)
        err_quit("usage: prodcons6 <#items> <#threads>");
    nitems = min(atoi(argv[1]), MAXNITEMS);
    nthreads = min(atoi(argv[2]), MAXNTHREADS);
    
    Set_concurrency(nthreads + 1);
    /* 4create all producers and one consumer */
    for (i = 0; i < nthreads; i++) {
        count[i] = 0;
        Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }
    Pthread_create(&tid_consume, NULL, consume, NULL);
    
    /* wait for all producers and the consumer */
    for (i = 0; i < nthreads; i++) {
        Pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }
    Pthread_join(tid_consume, NULL);
    
    exit(0);
}
/* end main */

/* include prodcons */
void *
produce(void *arg)
{
    for ( ; ; ) {
        Pthread_mutex_lock(&put.mutex);
        if (put.nput >= nitems) {
            Pthread_mutex_unlock(&put.mutex);
            return(NULL);		/* array is full, we're done */
        }
        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;
        Pthread_mutex_unlock(&put.mutex);
        
       Pthread_mutex_lock(&nready.mutex);
       if (nready.nready == 0)
           Pthread_cond_signal(&nready.cond);
       nready.nready++;
       Pthread_mutex_unlock(&nready.mutex);

        // Mutex::Autolock _l(mMutex);
        // if (nready.nready == 0)
        // {
        //     mCondition.signal();
        // }
        // nready.nready++;
        
        *((int *) arg) += 1;
    }
}

void *
consume(void *arg)
{
    int		i;
    
    for (i = 0; i < nitems; i++) {
       Pthread_mutex_lock(&nready.mutex);
       while (nready.nready == 0)
           Pthread_cond_wait(&nready.cond, &nready.mutex);
       nready.nready--;
       Pthread_mutex_unlock(&nready.mutex);
        
        // Mutex::Autolock _l(mMutex);
        
        // while (nready.nready == 0)
        // {
        //     mCondition.wait(mMutex);
        // }
        // nready.nready--;
        
        
        if (buff[i] != i)
            printf("buff[%d] = %d\n", i, buff[i]);
    }
    return(NULL);
}
/* end prodcons */











int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int		n;
    
again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
            if (errno == ECONNABORTED)
#endif
                goto again;
            else
                err_sys("accept error");
    }
    return(n);
}

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind error");
}

void
Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (connect(fd, sa, salen) < 0)
        err_sys("connect error");
}

void
Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if (getpeername(fd, sa, salenptr) < 0)
        err_sys("getpeername error");
}

void
Getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    if (getsockname(fd, sa, salenptr) < 0)
        err_sys("getsockname error");
}

void
Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
    if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
        err_sys("getsockopt error");
}


/* include Listen */
void
Listen(int fd, int backlog)
{
    char	*ptr;
    
    /*4can override 2nd argument with environment variable */
    if ( (ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    
    if (listen(fd, backlog) < 0)
        err_sys("listen error");
}
/* end Listen */

#ifdef	HAVE_POLL
int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
    int		n;
    
    if ( (n = poll(fdarray, nfds, timeout)) < 0)
        err_sys("poll error");
    
    return(n);
}
#endif

ssize_t
Recv(int fd, void *ptr, size_t nbytes, int flags)
{
    ssize_t		n;
    
    if ( (n = recv(fd, ptr, nbytes, flags)) < 0)
        err_sys("recv error");
    return(n);
}

ssize_t
Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
         struct sockaddr *sa, socklen_t *salenptr)
{
    ssize_t		n;
    
    if ( (n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
        err_sys("recvfrom error");
    return(n);
}

ssize_t
Recvmsg(int fd, struct msghdr *msg, int flags)
{
    ssize_t		n;
    
    if ( (n = recvmsg(fd, msg, flags)) < 0)
        err_sys("recvmsg error");
    return(n);
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
    int		n;
    
    if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
        err_sys("select error");
    return(n);		/* can return 0 on timeout */
}

void
Send(int fd, const void *ptr, size_t nbytes, int flags)
{
    if (send(fd, ptr, nbytes, flags) != nbytes)
        err_sys("send error");
}

void
Sendto(int fd, const void *ptr, size_t nbytes, int flags,
       const struct sockaddr *sa, socklen_t salen)
{
    if (sendto(fd, ptr, nbytes, flags, sa, salen) != nbytes)
        err_sys("sendto error");
}

void
Sendmsg(int fd, const struct msghdr *msg, int flags)
{
    int			i;
    ssize_t		nbytes;
    
    nbytes = 0;	/* must first figure out what return value should be */
    for (i = 0; i < msg->msg_iovlen; i++)
        nbytes += msg->msg_iov[i].iov_len;
    
    if (sendmsg(fd, msg, flags) != nbytes)
        err_sys("sendmsg error");
}

void
Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
        err_sys("setsockopt error");
}

void
Shutdown(int fd, int how)
{
    if (shutdown(fd, how) < 0)
        err_sys("shutdown error");
}

int
Sockatmark(int fd)
{
    int		n;
    
    if ( (n = sockatmark(fd)) < 0)
        err_sys("sockatmark error");
    return(n);
}

/* include Socket */
int
Socket(int family, int type, int protocol)
{
    int		n;
    
    if ( (n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return(n);
}
/* end Socket */

void
Socketpair(int family, int type, int protocol, int *fd)
{
    int		n;
    
    if ( (n = socketpair(family, type, protocol, fd)) < 0)
        err_sys("socketpair error");
}



ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
    size_t		nleft;
    ssize_t		nwritten;
    const char	*ptr;
    
    ptr = (const char	*)vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (errno == EINTR)
                nwritten = 0;		/* and call write() again */
            else
                return(-1);			/* error */
        }
        
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */

void
Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        err_sys("writen error");
}



static ssize_t
my_read(int fd, char *ptr)
{
    static int	read_cnt = 0;
    static char	*read_ptr;
    static char	read_buf[MAXLINE];
    
    if (read_cnt <= 0) {
    again:
        if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR)
                goto again;
            return(-1);
        } else if (read_cnt == 0)
            return(0);
        read_ptr = read_buf;
    }
    
    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
    int		n, rc;
    char	c, *ptr;
    
    ptr = (char *)vptr;
    for (n = 1; n < maxlen; n++) {
        if ( (rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;	/* newline is stored, like fgets() */
        } else if (rc == 0) {
            if (n == 1)
                return(0);	/* EOF, no data read */
            else
                break;		/* EOF, some data was read */
        } else
            return(-1);		/* error, errno set by read() */
    }
    
    *ptr = 0;	/* null terminate like fgets() */
    return(n);
}
/* end readline */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t		n;
    
    if ( (n = readline(fd, ptr, maxlen)) < 0)
        err_sys("readline error");
    return(n);
}


void
Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

void
err_ret(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(1, LOG_INFO, fmt, ap);
    va_end(ap);
    return;
}

/* Fatal error related to a system call.
 * Print a message and terminate. */

void
err_sys(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */

void
err_dump(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();		/* dump core and terminate */
    exit(1);		/* shouldn't get here */
}

/* Nonfatal error unrelated to a system call.
 * Print a message and return. */

void
err_msg(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(0, LOG_INFO, fmt, ap);
    va_end(ap);
    return;
}

/* Fatal error unrelated to a system call.
 * Print a message and terminate. */

void
err_quit(const char *fmt, ...)
{
    va_list		ap;
    
    va_start(ap, fmt);
    err_doit(0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

static void
err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int		errno_save, n;
    char	buf[MAXLINE];
    
    errno_save = errno;		/* value caller might want printed */
#ifdef	HAVE_VSNPRINTF
    vsnprintf(buf, sizeof(buf), fmt, ap);	/* this is safe */
#else
    vsprintf(buf, fmt, ap);					/* this is not safe */
#endif
    n = strlen(buf);
    if (errnoflag)
        snprintf(buf+n, sizeof(buf)-n, ": %s", strerror(errno_save));
    strcat(buf, "\n");
    
    {
        fflush(stdout);		/* in case stdout and stderr are the same */
        fputs(buf, stderr);
        fflush(stderr);
    }
    return;
}


const char *
Inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
    const char	*ptr;
    
    if (strptr == NULL)		/* check for old code */
        err_quit("NULL 3rd argument to inet_ntop");
    if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
        err_sys("inet_ntop error");		/* sets errno */
    return(ptr);
}

void
Inet_pton(int family, const char *strptr, void *addrptr)
{
    int		n;
    
    if ( (n = inet_pton(family, strptr, addrptr)) < 0)
        err_sys("inet_pton error for %s", strptr);	/* errno set */
    else if (n == 0)
        err_quit("inet_pton error for %s", strptr);	/* errno not set */
    
    /* nothing to return */
}
