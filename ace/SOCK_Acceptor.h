#ifndef ACE_SOCK_ACCEPTOR_H
#define ACE_SOCK_ACCEPTOR_H

#include "ACE_include.h"

// Defines a factory that creates new ACE_Streams passively.

// The ACE_SOCK_Acceptor has its own "passive-mode" socket.
// This serves as a factory to create so-called "data-mode"
// sockets, which are what the ACE_SOCK_Stream encapsulates.
// Therefore, by inheriting from ACE_SOCK, ACE_SOCK_Acceptor
// gets its very own socket.

class ACE_Export ACE_SOCK_Acceptor
{
public:
	
	
};


#endif