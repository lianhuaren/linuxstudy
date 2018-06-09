#include "unp.h"

int
main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	// if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	// 	err_sys("connect error");

	if (connect_nonb(sockfd, (SA *)&servaddr, sizeof(servaddr), 0) < 0)
		err_sys("connect error");

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error");

	exit(0);
	
	return 0;
}


int
connect_nonb(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	int flags, n, error;
	socklen_t len;
	fd_set rset, wset;
	struct timeval tval;

	flags = Fcntl(sockfd, F_GETFL, 0);
	Fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	error = 0;
	if ((n = connect(sockfd, (struct sockaddr *)saptr, salen)) < 0)
		if (errno != EINPROGRESS)
			return -1;

	if (n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	tval.tv_sec = nsec;
	tval.tv_usec = 0;

	if ((n = Select(sockfd+1, &rset, &wset, NULL,
				    nsec ? &tval : NULL)) == 0) {
		close(sockfd);
		errno = ETIMEDOUT;
		return -1;
	}

	if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
		len = sizeof(error);

	} else {
		err_quit("select error : sockfd not set");
	}

done:
	Fcntl(sockfd, F_SETFL, flags);

	if (error) {
		close(sockfd);
		errno = error;
		return -1;
	}

	return 0;

}