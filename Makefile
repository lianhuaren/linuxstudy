all : tcpserv01 tcpcli01

tcpcli01 : tcpcli01.o libwrap.o
		gcc tcpcli01.c libwrap.c -o tcpcli01 -g
tcpserv01 : tcpserv01.o libwrap.o
		gcc tcpserv01.c libwrap.c -o tcpserv01 -lpthread -g

clean :
		-rm tcpserv01 tcpcli01