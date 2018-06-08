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
		printf("1111--%d\n", connfd);
	// str_echo(connfd);
	// Close(connfd);

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











