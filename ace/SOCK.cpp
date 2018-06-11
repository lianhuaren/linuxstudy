#include "SOCK.h"

ACE_SOCK::ACE_SOCK (void)
{
  // ACE_TRACE ("ACE_SOCK::ACE_SOCK");
}

int 
ACE_SOCK::close (void)
{
	//ACE_TRACE ("ACE_SOCK::close");
	int result = 0;

	if (this->get_handle () != ACE_INVALID_HANDLE)
	{
		result = ACE_OS::closesocket (this->get_handle ());
		this->set_handle (ACE_INVALID_HANDLE);
	}
	return result;
}

int
ACE_SOCK::open (int type,
		int protocol_family,
		int protocol,
		int reuse_addr)
{
	//ACE_TRACE ("ACE_SOCK::open);
	int one = 1;

	this->set_handle (ACE_OS::socket (protocol_family,
					  type,
					  protocol));
	
	if (this->get_handle () == ACE_INVALID_HANDLE)
		return -1;

	if (protocol_family != PF_UNIX
		&& reuse_addr
		&& this->set_option (SOL_SOCKET,
							 SO_REUSEADDR,
							 &one,
							 sizeof(one)) == -1)
	{
		this->close ();
		return -1;
	}

	return 0;
}

// General purpose constructor for performing server ACE_SOCK
// creation.

ACE_SOCK::ACE_SOCK (int type,
                    int protocol_family,
                    int protocol,
                    int reuse_addr)
{
  // ACE_TRACE ("ACE_SOCK::ACE_SOCK");
  if (this->open (type,
                  protocol_family,
                  protocol,
                  reuse_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_SOCK::ACE_SOCK")));
}