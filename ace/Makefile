all : chapter03

chapter03 : chapter03.o SOCK.o Addr.o SOCK_Acceptor.o
		g++ chapter03.cpp SOCK.cpp Addr.cpp SOCK_Acceptor.cpp -g

clean :
		-rm chapter03

