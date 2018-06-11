#include "SOCK_Acceptor.h"


int ACE_SOCK_Acceptor::shared_open (const ACE_Addr &local_sap,
														int protocol_family,
														int backlog)
{
	ACE_TRACE ("ACE_SOCK_Acceptor::shared_open");
	int error = 0;
	
	if (protocol_family == PF_INET)
	{
		sockaddr_in local_inet_addr;
		memset (reinterpret_cast<void *> (&local_inet_addr),
								0,
								sizeof(local_inet_addr));
								
		if (local_sap == ACE_Addr::sap_any) {
			local_inet_addr.sin_port = 0;
		} else 
			local_inet_addr = *reinterpret_cast<sockaddr_in *> (local_sap.get_addr ());
		
		if (local_inet_addr.sin_port == 0) {

			ACE_UINT32 ip_addr =  ntohl (ACE_UINT32 (local_inet_addr.sin_addr.s_addr));
			
			ACE_INET_Addr addr;
			
			addr = ACE_INET_Addr ((u_short)0, ip_addr);
			
			if (bind (this->get_handle (),
											(sockaddr*)addr.get_addr(),
											addr.get_size()) == -1 ) 
			error = 1;
			
		} else if (bind (this->get_handle (),
											reinterpret_cast<sockaddr *> (&local_inet_addr),
											sizeof(local_inet_addr)) == -1 ) 
			error = 1;
	}  else if (bind (this->get_handle (),
                         (sockaddr *) local_sap.get_addr (),
                         local_sap.get_size ()) == -1) {
			error = 1;
	}
	
	if (error != 0
		|| listen (this->get_handle (),
									backlog) == -1)
	{
      // ACE_Errno_Guard g (errno);    // Preserve across close() below.
      error = 1;
      this->close ();			
	}
	
	return error ? -1 : 0;
}	


int
ACE_SOCK_Acceptor::open (const ACE_Addr &local_sap,
                         int reuse_addr,
                         int protocol_family,
                         int backlog,
                         int protocol)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::open");
 
    if (local_sap != ACE_Addr::sap_any)
		protocol_family = local_sap.get_type ();
    else if (protocol_family == PF_UNSPEC)
    {
		protocol_family = PF_INET;
    }
    
    if (ACE_SOCK::open (SOCK_STREAM,
   							  protocol_family,
   							  protocol,
   							  reuse_addr) == -1)
		return -1;
	else 
		return this->shared_open (local_sap,
												protocol_family,
												backlog);
}

// General purpose routine for performing server ACE_SOCK creation.

ACE_SOCK_Acceptor::ACE_SOCK_Acceptor (const ACE_Addr &local_sap,
                                      int reuse_addr,
                                      int protocol_family,
                                      int backlog,
                                      int protocol)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::ACE_SOCK_Acceptor");
  if (this->open (local_sap,
                  reuse_addr,
                  protocol_family,
                  backlog,
                  protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_SOCK_Acceptor")));
}

int
ACE_SOCK_Acceptor::close (void)
{
  return ACE_SOCK::close ();
}

int
ACE_SOCK_Acceptor::shared_accept_start (ACE_Time_Value *timeout,
                                        bool restart,
                                        int &in_blocking_mode) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::shared_accept_start");


	return 0;
}  

int
ACE_SOCK_Acceptor::shared_accept_finish (ACE_SOCK_Stream new_stream,
                                         int in_blocking_mode,
                                         bool reset_new_handle) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::shared_accept_finish ()");
 

	return 0;
}  

int
ACE_SOCK_Acceptor::accept (ACE_SOCK_Stream &new_stream,
                           ACE_Addr *remote_addr,
                           ACE_Time_Value *timeout,
                           bool restart,
                           bool reset_new_handle) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::accept");
  
	int in_blocking_mode = 0;
	if (this->shared_accept_start (timeout,
													restart,
													in_blocking_mode) == -1)
		return -1;
	else {
      // On Win32 the third parameter to <accept> must be a NULL
      // pointer if we want to ignore the client's address.

		int *len_ptr = 0;
		sockaddr *addr = 0;
		int len = 0;
		
		if (remote_addr != 0)
		{
			len = remote_addr->get_size ();
			len_ptr = &len;
			addr = (sockaddr *) remote_addr->get_addr ();
		}

		do {
			// new_stream.set_handle (accept (this->get_handle (),
																		// addr,
																		// len_ptr));
			new_stream.set_handle (::accept (this->get_handle (),
                                    addr,
                                    (ACE_SOCKET_LEN *) len_ptr));
																		
																		
		} while (new_stream,get_handle () == ACE_INVALID_HANDLE
					&& restart
					&& errno == EINTR
					&& timeout == 0);
		
      // Reset the size of the addr, so the proper UNIX/IPv4/IPv6 family
      // is known.	
	  
		if (new_stream.get_handle () != ACE_INVALID_HANDLE
			 && remote_addr != 0)
		{
			remote_addr->set_size (len);
			if (addr)
				remote_addr->set_type (addr->sa_family);
		}
		
		return this->shared_accept_finish (new_stream,
															  in_blocking_mode,
															  reset_new_handle);
	}

	return 0;
}  







