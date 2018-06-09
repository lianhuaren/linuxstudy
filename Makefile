all : tcpserv01 tcpcli01

tcpcli01 : tcpcli01.o libwrap.o
		gcc tcpcli01.c libwrap.c -o tcpcli01 -lpthread -g
tcpserv01 : tcpserv01.o libwrap.o
		gcc tcpserv01.c libwrap.c -o tcpserv01 -lpthread -g
daytimetcpcli01 : daytimetcpcli01.o libwrap.o
		gcc daytimetcpcli01.c libwrap.c -o daytimetcpcli01 -lpthread -g
daytimetcpsrv01 : daytimetcpsrv01.o libwrap.o
		gcc daytimetcpsrv01.c libwrap.c -o daytimetcpsrv01 -lpthread -g

clean :
		-rm tcpserv01 tcpcli01

