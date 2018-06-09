#include	"unp.h"

int 
tcp_listen(const char * host, const char *serv, socklen_t *addrlenp)
{
	int listenfd, n;
	const int on = 1;
	struct addrinfo hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("tcp_listen error for %s, %s: %s",
			     host, serv, gai_strerror(n));

	ressave = res;

	do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (listenfd < 0)
			continue;

		Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
			break;

		Close(listenfd);
	} while((res = res->ai_next) != NULL);

	if(res == NULL)
		err_sys("tcp_listen error for %s, %s", host, serv);

	Listen(listenfd, LISTENQ);

	if (addrlenp)
		*addrlenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return listenfd;
}


ssize_t
writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while(nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if (errno = EINTR)
				nwritten = 0;
			else 
				return -1;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

void
Writen(int fd, void *ptr, size_t nbytes)
{
	if(writen(fd, ptr, nbytes) != nbytes)
		err_sys("writen error");
}


int 
Tcp_listen(const char * host, const char *serv, socklen_t *addrlenp)
{
	return tcp_listen(host, serv, addrlenp);
}

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		line[MAXLINE];

	for ( ; ; ) {
		if ( (n = Readline(sockfd, line, MAXLINE)) == 0)
			return;		/* connection closed by other end */

		Writen(sockfd, line, n);
	}
}

// static ssize_t
// my_read(int fd, char *ptr)
// {
// 	static int	read_cnt = 0;
// 	static char	*read_ptr;
// 	static char	read_buf[MAXLINE];

// 	if (read_cnt <= 0) {
// again:
// 		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
// 			if (errno == EINTR)
// 				goto again;
// 			return(-1);
// 		} else if (read_cnt == 0)
// 			return(0);
// 		read_ptr = read_buf;
// 	}

// 	read_cnt--;
// 	*ptr = *read_ptr++;
// 	return(1);
// }

// ssize_t
// readline(int fd, void *vptr, size_t maxlen)
// {
// 	int		n, rc;
// 	char	c, *ptr;

// 	ptr = vptr;
// 	for (n = 1; n < maxlen; n++) {
// 		if ( (rc = my_read(fd, &c)) == 1) {
// 			*ptr++ = c;
// 			if (c == '\n')
// 				break;	/* newline is stored, like fgets() */
// 		} else if (rc == 0) {
// 			if (n == 1)
// 				return(0);	/* EOF, no data read */
// 			else
// 				break;		/* EOF, some data was read */
// 		} else
// 			return(-1);		/* error, errno set by read() */
// 	}

// 	*ptr = 0;	/* null terminate like fgets() */
// 	return(n);
// }
// /* end readline */

// ssize_t
// Readline(int fd, void *ptr, size_t maxlen)
// {
// 	ssize_t		n;

// 	if ( (n = readline(fd, ptr, maxlen)) < 0)
// 		err_sys("readline error");
// 	return(n);
// }


static pthread_key_t rl_key;
static pthread_once_t rl_once = PTHREAD_ONCE_INIT;


static void
readline_destructor(void *ptr)
{
	free(ptr);
}

static void
readline_once(void)
{
	Pthread_key_create(&rl_key, readline_destructor);
}

typedef struct {
	int rl_cnt;
	char *rl_bufptr;
	char rl_buf[MAXLINE];
}Rline;

static ssize_t
my_read(Rline *tsd, int fd, char *ptr)
{
	if (tsd->rl_cnt <= 0) {
again:
		if ((tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0) {
			if (errno = EINTR)
				goto again;
			return -1;
		} else if (tsd->rl_cnt == 0) {
			return 0;
		}

		tsd->rl_bufptr = tsd->rl_buf;
	}

	tsd->rl_cnt--;
	*ptr = *tsd->rl_bufptr++;

	return 1;	
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	int n, rc;
	char c, *ptr;
	Rline *tsd;

	Pthread_once(&rl_once, readline_once);
	if ((tsd = pthread_getspecific(rl_key)) == NULL) {
		tsd = Calloc(1, sizeof(Rline));
		Pthread_setspcific(rl_key, tsd);
	}

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ((rc = my_read(tsd, fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return 0;
			else 
				break;
		} else {
			return -1;
		}
	}

	*ptr = 0;
	return n;
}

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t n;

	if ((n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return n;
}






void *
Calloc(size_t n, size_t size)
{
	void	*ptr;

	if ( (ptr = calloc(n, size)) == NULL)
		err_sys("calloc error");
	return(ptr);
}

int
Fcntl(int fd, int cmd, int arg)
{
	int	n;

	if ( (n = fcntl(fd, cmd, arg)) == -1)
		err_sys("fcntl error");
	return(n);
}

void *
Malloc(size_t size)
{
	void *ptr;

	if ((ptr = malloc(size)) == NULL)
		err_sys("malloc error");
	return ptr;
}


void
Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		err_sys("write error");
}





void
Pthread_create(pthread_t *tid, const pthread_attr_t *attr,
			   void * (*func)(void *), void *arg)
{
	int		n;

	if ( (n = pthread_create(tid, attr, func, arg)) == 0)
		return;
	errno = n;
	err_sys("pthread_create error");
}

void Pthrad_join(pthread_t tid, void **status)
{
	int n;

	if ((n = pthread_join(tid, status)) == 0)
		return;
	errno = n;
	err_sys("pthread_join error");
}

void
Pthread_detach(pthread_t tid)
{
	int		n;

	if ( (n = pthread_detach(tid)) == 0)
		return;
	errno = n;
	err_sys("pthread_detach error");
}

void
Pthread_kill(pthread_t tid, int signo)
{
	int n;

	if ((n = pthread_kill(tid, signo)) == 0)
		return;
	errno = n;
	err_sys("pthread_kill error");
}

void
Pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
	int n;

	if ((n = pthread_mutexattr_init(attr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutexattr_init error");
}

void
Pthread_mutex_init(pthread_mutex_t *mptr, pthread_mutexattr_t *attr)
{
	int n;

	if ((n = pthread_mutex_init(mptr, attr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_init error");
}

void
Pthread_mutex_lock(pthread_mutex_t *mptr)
{
	int n;

	if ((n = pthread_mutex_lock(mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_lock error");
}

void
Pthread_mutex_unlock(pthread_mutex_t *mptr)
{
	int n;

	if ((n = pthread_mutex_unlock(mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_unlock error");
}

void
Pthread_cond_broadcast(pthread_cond_t *cptr)
{
	int n;

	if ((n = pthread_cond_broadcast(cptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_broadcast error");
}

void
Pthread_cond_signal(pthread_cond_t *cptr)
{
	int n;

	if ((n = pthread_cond_signal(cptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_signal error");
}

void
Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
	int n;

	if ((n = pthread_cond_wait(cptr, mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_wait error");
}

void
Pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr,
					   const struct timespec *tsptr)
{
	int n;

	if ((n = pthread_cond_timedwait(cptr, mptr, tsptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_timedwait error");
}					   

void
Pthread_once(pthread_once_t *ptr, void (*func)(void))
{
	int		n;

	if ( (n = pthread_once(ptr, func)) == 0)
		return;
	errno = n;
	err_sys("pthread_once error");
}	

void
Pthread_key_create(pthread_key_t *key, void (*func)(void *))
{
	int n;

	if ((n = pthread_key_create(key, func)) == 0)
		return;
	errno = n;
	err_sys("pthread_key_create error");
}	

void
Pthread_setspcific(pthread_key_t key, const void *value)
{
	int n;

	if ((n = pthread_setspecific(key, value)) == 0)
		return;
	errno = n;
	err_sys("pthread_setspecific error");
}	








int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int n;

again:
	if ((n = accept(fd, sa, salenptr)) < 0)
	{
		if (errno == ECONNABORTED)
		{
			goto again;
		}
		else 
		{
			err_sys("accept error");

		}
	}

	return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
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

void
Listen(int fd, int backlog)
{
	char *ptr;

	if ((ptr = getenv("LISTENQ")))
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}

int Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
	int n;

	if ((n = poll(fdarray, nfds, timeout)) < 0)
		err_sys("poll error");

	return n;
}

ssize_t
Recv(int fd, void *ptr, size_t nbytes, int flags)
{
	ssize_t n;

	if ((n = recv(fd, ptr, nbytes, flags)) < 0)
		err_sys("recv error");
	return n;
}

ssize_t
Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
		 struct sockaddr *sa, socklen_t *salenptr)		 
{
	ssize_t n;

	if ((n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
		err_sys("recvfrom error");
	return n;
}

ssize_t
Recvmsg(int fd, struct msghdr *msg, int flags)
{
	ssize_t n;

	if ((n = recvmsg(fd, msg, flags)) < 0)
		err_sys("recvmsg error");
	return n;
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
	   struct timeval *timeout)
{
	int n;

	if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		err_sys("select error");

	return n;
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
	int i;
	ssize_t nbytes;

	nbytes = 0;
	for (i = 0; i < msg->msg_iovlen; ++i)
	{
		nbytes += msg->msg_iov[i].iov_len;
	}

	if (sendmsg(fd, msg, flags) != nbytes)
		err_sys("sendmsg error");
}

void
Setsockopt(int fd, int level, int optname, const void * optval, socklen_t optlen)
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

// int
// Sockatmark(int fd)
// {
// 	int n;

// 	if ((n = sockatmark(fd)) < 0)
// 		err_sys("sockatmark error");

// 	return n;
// }	

int
Socket(int family, int type, int protocol)
{
	int n;

	if ((n = socket(family, type, protocol)) < 0)
		err_sys("socket error");

	return n;
}	

void
Socketpair(int family, int type, int protocol, int *fd)
{
	int n;

	if ((n = socketpair(family, type, protocol, fd)) < 0)
		err_sys("Socketpair error");

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
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, LOG_INFO, fmt, ap);
	va_end(ap);
}

void
err_sys(const char *fmt, ...)
{
	va_list ap;

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
	int errno_save, n;
	char buf[MAXLINE];

	errno_save = errno;

	vsprintf(buf, fmt, ap);
	n = strlen(buf);
	if (errnoflag)
		snprintf(buf+n, sizeof(buf)-n, ": %s", strerror(errno_save));
	strcat(buf, "\n");

	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}

	return;

}



int
inet_aton(const char *cp, struct in_addr *ap)
{
    int dots = 0;
    register u_long acc = 0, addr = 0;

    do {
	register char cc = *cp;

	switch (cc) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    acc = acc * 10 + (cc - '0');
	    break;

	case '.':
	    if (++dots > 3) {
		return 0;
	    }
	    /* Fall through */

	case '\0':
	    if (acc > 255) {
		return 0;
	    }
	    addr = addr << 8 | acc;
	    acc = 0;
	    break;

	default:
	    return 0;
	}
    } while (*cp++) ;

    /* Normalize the address */
    if (dots < 3) {
	addr <<= 8 * (3 - dots) ;
    }

    /* Store it if requested */
    if (ap) {
	ap->s_addr = htonl(addr);
    }

    return 1;    
}

int		 inet_aton(const char *, struct in_addr *);

/* include inet_pton */
int
inet_pton(int family, const char *strptr, void *addrptr)
{
    if (family == AF_INET) {
    	struct in_addr  in_val;

        if (inet_aton(strptr, &in_val)) {
            memcpy(addrptr, &in_val, sizeof(struct in_addr));
            return (1);
        }
		return(0);
    }
	errno = EAFNOSUPPORT;
    return (-1);
}
/* end inet_pton */

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


void
Fclose(FILE *fp)
{
	if (fclose(fp) != 0)
		err_sys("fclose error");
}

FILE *
Fdopen(int fd, const char *type)
{
	FILE	*fp;

	if ( (fp = fdopen(fd, type)) == NULL)
		err_sys("fdopen error");

	return(fp);
}

char *
Fgets(char *ptr, int n, FILE *stream)
{
	char	*rptr;

	if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
		err_sys("fgets error");

	return (rptr);
}

FILE *
Fopen(const char *filename, const char *mode)
{
	FILE	*fp;

	if ( (fp = fopen(filename, mode)) == NULL)
		err_sys("fopen error");

	return(fp);
}

void
Fputs(const char *ptr, FILE *stream)
{
	if (fputs(ptr, stream) == EOF)
		err_sys("fputs error");
}













