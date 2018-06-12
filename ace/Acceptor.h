#ifndef ACE_ACCEPTOR_H
#define ACE_ACCEPTOR_H

#include "ACE_include.h"

 // @class ACE_Acceptor
 
 // @brief Abstract factory for creating a service handler
 // (SVC_HANDLER), accepting into the SVC_HANDLER, and 
 // activating the SVC_HANDLER. 
 
 // Implements the basic strategy for passively establishing
 // connections with clients. An ACE_Acceptor is parameterized
 // by concrete types that conform to the interfaces of 
 // PEER_ACCEPTOR and SVC_HANDLER. The PEER_ACCEPTOR is 
 // instantiated with a transport mechanism that passively
 // establishes connections. The SVC_HANDLER is instantiated 
 // with a concrete type that perfroms the application-specific
 // service. An ACE_Acceptor inherits from ACE_Service_Object,
 // which in turn inherits from ACE_Event_Handler. This enables 
 // the ACE_Reactor to dispatch the ACE_Acceptor's handle_input
 // method when connection events occur. The handle_input method
 // performs the ACE_Acceptor's default creation, connection 
 // establishment, and service activation strategies. These 
 // strategies can be overridden by subclasses individually or as 
 // a group.
 
template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class ACE_Acceptor 
{
public:
	/// "Do-nothing" constructor.
	ACE_Acceptor (ACE_Reactor * = 0,
						  int use_select = 1);


	 // Open the contained @c PEER_ACCEPTOR object to begin listening , and
	 // register with the specified reactor for accept events. An 
	 // acceptor can only listen to one port at a time, so make sure to 
	 // @c close() the acceptor before calling @c open() again.
	 
	 // The @c PEER_ACCEPTOR handle is put into non-blocking mode as a 
	 // safeguard against the race condition that can otherwise occur
	 // between the time when the passive-mode socket handle is "ready" 
	 // and when the actual @c accept() call is made. During this 
	 // interval, the client can shutdown the connection, in which case,
	 // the @c accept() call can hang.
	 
	 // @param local_addr The address to listen at.
	 // @param reactor      Pointer to the ACE_Reactor instance to register
							     // this object with. The default is the singleton.
	 // @param flags         Flags to control what mode an accepted socket
								 // will be put into after it is accepted. The only
								 // legal value for this argument is @c ACE_NONBLOCK,
								 // which enables non-blocking mode on the accepted
								 // peer stream object in @c SVC_HANDLER. The default
								 // is 0.
	 // @param use_select Affects behavior when called back the reactor
								 // when a connection can be accepted. If non-zero,
								 // this object will accept all pending connections,
								 // instead of just the one the triggered the reactor
								 // callback. Uses ACE_OS::select() internally to 
								 // detect any remaining acceptable connections.
								 // The default is 1.
	 // @param reuse_addr Passed to the @c PEER_ACCEPTOR::open() method with
								 // @p local_addr. Generally used to request that the 
								 // OS allow reuse of the listen port. The default is 1.
								 
	 // @retval 0  Success
	 // @retval -1 Failure, @c errno contains an error code.

	virtual int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
								ACE_Reactor *reactor = ACE_Reactor::instance (),
								int flags = 0,
								int use_select = 1,
								int reuse_addr = 1);

	/// Close down the Acceptor's resources.
	virtual ~ACE_Acceptor (void);

	/// Close down the Acceptor
	virtual int close (void);

protected:
	
	 // = Demultiplexing hooks.
	 // Perform termination activities when {this} is removed from the 
	 // {reactor}.
	virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
											ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
	
	 // Accepts all pending connections from clients, and creates and 
	 // activates SVC_HANDLERs.
	virtual int handle_input (ACE_HANDLE);










};

#endif