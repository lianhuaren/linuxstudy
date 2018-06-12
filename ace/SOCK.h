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
		  
  /// Get the underlying handle.
  ACE_HANDLE get_handle (void) const;

  /// Set the underlying handle.
  void set_handle (ACE_HANDLE handle);
  
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



private:
  /// Underlying I/O handle.
  ACE_HANDLE handle_;

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
  // return ACE_OS::setsockopt (this->get_handle (), level,
                             // option, (char *) optval, optlen);
							 
	return ::setsockopt ((ACE_SOCKET) this->get_handle (),
                              level,
                              option,
                              (char *) optval,
                              optlen);						 
}

// Provides access to the ACE_OS::getsockopt system call.

ACE_INLINE int
ACE_SOCK::get_option (int level,
                      int option,
                      void *optval,
                      int *optlen) const
{
  ACE_TRACE ("ACE_SOCK::get_option");
  // return ACE_OS::getsockopt (this->get_handle (), level,
                             // option, (char *) optval, optlen);
	return ::getsockopt ((ACE_SOCKET) this->get_handle (),
                                     level,
                                     option,
                                     optval,
                                     (ACE_SOCKET_LEN *) optlen);
}


ACE_INLINE ACE_HANDLE
ACE_SOCK::get_handle (void) const
{
  ACE_TRACE ("ACE_IPC_SAP::get_handle");
  return this->handle_;
}

// Used to set the underlying handle_.

ACE_INLINE void
ACE_SOCK::set_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_IPC_SAP::set_handle");
  this->handle_ = handle;
}

 // @class ACE_SOCK_IO
 
 // @brief Defines the methods for the ACE socket wrapper I/O routines
  // (e.g., send/recv).
  
  // if @a timeout == 0, then the call behaves as a normal 
  // send/recv call, i.e., for blocking sockets, the call will 
  // block until action is possible; for non-blocking sockets,
  // -1 will be returned with errno == EWOULDBLOCK if no ation is 
  // immediately possible.
  // If @a timeout != 0, the call will wait until the relative time
  // specified in *@a timeout elapses.
  // Errors are reported by -1 and 0 return values. If the 
  // operation times ot, -1 is returned with @c errno == ETIME.
class ACE_Export ACE_SOCK_IO : public ACE_SOCK
{
	
	
};


 // @class ACE_SOCK_Stream
 
 // @brief Defines the methods in the ACE_SOCK_Stream abstraction.
 
 // This adds additional wrapper methods atop the ACE_SOCK_IO
 // class.
class ACE_Export ACE_SOCK_Stream : public ACE_SOCK_IO
{
public:
	
	 // @name Counted send/receive methods
	 
	 // The counted send/receive methods attempt to transfer a specified number
	 // of bytes even if they must block and retry the operation in order to 
	 // transfer the entire amount. The time spent blocking for the entire
	 // transfer can be limited by a specified ACE_Time_Value object which is 
	 // a relative time (i.e., a fixed amount of time, not an absolute time 
	 // of day). These methods return the count of transferred bytes, or -1
	 // if an error occurs or the operation times out before the entire requested 
	 // amount of data has been transferred.  In error or timeout situations it's 
	 // possible that some data was transferred before the error 
	 // or timeout. The @c bytes_transferred parameter is used to obtain the 
	 // count of bytes transferred before the error or timeout occurred. If the 
	 // total specified number of bytes is transfered without error, the 
	 // method return value should equal the value of @c bytes_transferred.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;



};


#endif
