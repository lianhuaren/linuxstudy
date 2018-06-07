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
		if (nwritten = writen(fd, ptr, nleft) <= 0) {
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
	ssize_t n;
	char line[MAXLINE];

	for ( ; ; ) {
		if ((n = Readline(sockfd, line, MAXLINE)) == 0)
			return;

		Writen(sockfd, line, n);
	}
}


static ssize_t
my_read(int fd, char *ptr)
{
	static int read_cnt = 0;
	static char *read_ptr;
	static char *read_buf[MAXLINE];

	if (read_cnt <= 0) {
again:
		if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno = EINTR)
				goto again;
			return -1;
		} else if (read_cnt == 0) {
			return 0;
		}

		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;

	return 1;
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	int n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ((rc = my_read(fd, &c)) == 1) {
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
Malloc(size_t size)
{
	void *ptr;

	if ((ptr = malloc(size)) == NULL)
		err_sys("malloc error");
	return ptr;
}





void
Pthread_create(pthread_t *tid, const pthread_attr_t *attr,
			   void * (*func)(void *), void *arg)
{
	int n;

	if ((n = pthread_create(tid, attr, func, arg)) == 0)
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
	int n;

	if ((n = pthread_detach(tid)) == 0)
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

// void
// Pthread_once(pthread_once_t *ptr, void (*func)(void))
// {
// 	int n;

// 	if ((n = pthread_cond_once(ptr, func)) == 0)
// 		return;
// 	errno = n;
// 	err_sys("pthread_once error");
// }	

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





















