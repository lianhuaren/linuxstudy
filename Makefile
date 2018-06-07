all : tcpserv01

tcpserv01 : tcpserv01.o libwrap.o
		gcc tcpserv01.c libwrap.c -o tcpserv01 -lpthread

clean :
		-rm tcpserv01