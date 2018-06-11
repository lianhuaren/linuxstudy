#ifndef ACE_SOCK_H
#define ACE_SOCK_H

#include "ACE_include.h"
 //*
 //* An abstract class that forms the basis for more specific 
 //* classes, such as ACE_SOCK_Acceptor and ACE_SOCK_Stream.
 //* Do not instantiate this class.
 //* 
 //* This class provides functions that are common to all of the 
 //* <ACE_SOCK_*> classes. ACE_SOCK provides the ability to get 
 //* and set socket options. get the local and remote addresses,
 //* and open and close a socket handle.
 //*


class ACE_Export ACE_SOCK
{
public:
	int set_option (int level,
			int option,
			void *optval,
			int optlen) const;

	int get_option (int level,
			int option,
			void *optval,
			int *optlen) const;
 //* Close the socket.
 //* This method also sets the object's handle value to ACE_INVALID_HANDLE.
 //* 
 //* return The result of closing the socket; 0 if the handle value
 //* 	was already ACE_INVALID_HANDLE.
	int close (void);
		
	int open (int type,
		  int protocol_family,
		  int protocol,
		  int reuse_addr);

	int open (int type,
		  int protocol_family,
		  int protocol,
		  //ACE_Protocol_Info *protocolinfo,
		  //ACE_SOCK_GROUP g,
		  u_long flags,
		  int reuse_addr);
protected:
	ACE_SOCK (int type,
		  int protocol_family,
		  int protocol,
		  int reuse_addr);

	// ACE_SOCK (int type,
	// 	  int protocol_family,
	// 	  int protocol,
	// 	  //ACE_Protocol_Info *protocolinfo,
	// 	  //ACE_SOCK_GROUP g,
	// 	  u_long flags,
	// 	  int reuse_addr);

//Default constructor is protected to prevent instances of this class
//from being defined.
ACE_SOCK (void);

 //* Protected destructor
 //* Not a virtual destructor. Protected destructor to prevent
 //* operator delete() from being called through a base class ACE_SOCK
 //* pointer/reference.
~ACE_SOCK (void);



};

ACE_INLINE
ACE_SOCK::~ACE_SOCK (void)
{
  // ACE_TRACE ("ACE_SOCK::~ACE_SOCK");
}

ACE_INLINE int
ACE_SOCK::set_option (int level,
                      int option,
                      void *optval,
                      int optlen) const
{
  ACE_TRACE ("ACE_SOCK::set_option");
  return ACE_OS::setsockopt (this->get_handle (), level,
                             option, (char *) optval, optlen);
}

// Provides access to the ACE_OS::getsockopt system call.

ACE_INLINE int
ACE_SOCK::get_option (int level,
                      int option,
                      void *optval,
                      int *optlen) const
{
  ACE_TRACE ("ACE_SOCK::get_option");
  return ACE_OS::getsockopt (this->get_handle (), level,
                             option, (char *) optval, optlen);
}

#endif
