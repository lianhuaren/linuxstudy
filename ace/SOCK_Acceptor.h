#ifndef ACE_SOCK_ACCEPTOR_H
#define ACE_SOCK_ACCEPTOR_H

#include "ACE_include.h"
#include "Addr.h"
#include "SOCK.h"
#include "Time_Value.h"

// Defines a factory that creates new ACE_Streams passively.

// The ACE_SOCK_Acceptor has its own "passive-mode" socket.
// This serves as a factory to create so-called "data-mode"
// sockets, which are what the ACE_SOCK_Stream encapsulates.
// Therefore, by inheriting from ACE_SOCK, ACE_SOCK_Acceptor
// gets its very own socket.

class ACE_Export ACE_SOCK_Acceptor : public ACE_SOCK
{
public:
	
	 // = Initialzation and termination methods.
	  // Default constructor.
	ACE_SOCK_Acceptor (void);
  /**
   * Initialize a passive-mode BSD-style acceptor socket (no QoS).
   * @a local_sap is the address that we're going to listen for
   * connections on.  If @a reuse_addr is 1 then we'll use the
   * @c SO_REUSEADDR to reuse this address.
   */
  ACE_SOCK_Acceptor (const ACE_Addr &local_sap,
                     int reuse_addr = 0,
                     int protocol_family = PF_UNSPEC,
                     int backlog = ACE_DEFAULT_BACKLOG,
                     int protocol = 0);
					 
	// Initialize a passive-mode BSD-style acceptor socket (no Qos).
	// @a local_sap is the address that we're going to listen for 
	// connections on. If @a resus_addr is 1 then we'll use the 
	// @c SO_REUSEDADDR to reuse this address. Returns 0 on success and 
	// -1 on failure.
	int open (const ACE_Addr &local_sap,
				   int reuse_addr = 0,
				   int protocol_family = PF_UNSPEC,
				   int blacklog = ACE_DEFAULT_BACKLOG,
				   int protocol = 0);

	/// Close the socket. Returns
	int close (void);

	/// Default dtor.
	~ACE_SOCK_Acceptor (void);
	
	// = Passive connection <accept> methods.
	
	// Accept a new ACE_SOCK_Stream connection.  A @a timeout of 0
	// means block forever, a @a timeout of {0, 0} means poll. @a restart 
	// == true means "restart if interrupted," i.e., if errno == EINTR.
	// Note that @a new_stream inherits the "blocking mode" of @c this 
	// ACE_SOCK_Acceptor, i.e., if @c this acceptor factory is in 
	// non-blocking mode, the @a new_stream will be in non-blocking mode
	// and vice versa.
	int accept (ACE_SOCK_Stream &new_stream,
					  ACE_Addr *remote_addr = 0,
					  ACE_Time_Value *timeout = 0,
					  bool restart = true,
					  bool reset_new_handle = false) const;

protected:
	/// Perform operations that must occur before <ACE_OS::accept> is
	/// called.
	int shared_accept_start (ACE_Time_Value *timeout,
											bool restart,
											int &in_blocking_mode) const;
											
	///
	int shared_accept_finish (ACE_SOCK_Stream new_stream,
											int in_blocking_mode,
											bool reset_new_handle) const;
											
	// This method factors out the common <open> code and is called by 
	// both the Qos-enabled <open> method and the BSD-style <open>
	// method.
	int shared_open (const ACE_Addr &local_sap,
							  int protocol_family,
							  int backlog);
	
	
	
	
	
};


#endif