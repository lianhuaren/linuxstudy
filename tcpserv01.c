#include	"unp.h"

static void *doit(void *);

int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t addrlen, len;
	struct sockaddr *cliaddr;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("useage: tcpserv01 [ <host> ] <service or port>");

	cliaddr = Malloc(addrlen);

	for (; ; ) {
		len = addrlen;
		connfd = Accept(listenfd, cliaddr, &len);

		Pthread_create(NULL, NULL, &doit, (void *) connfd);
	}

	return 0;
}

static void *
doit(void *arg)
{
	Pthread_detach(pthread_self());
	str_echo((int) arg);
	Close((int) arg);
	return NULL;
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