#include "Addr.h"

// Note: this object requires static construction and destruction.
/* static */
const ACE_Addr ACE_Addr::sap_any (AF_ANY, -1);

// Initializes instance variables.  Note that 0 is an unspecified
// protocol family type...

ACE_Addr::ACE_Addr (int type, int size) :
  addr_type_ (type),
  addr_size_ (size)
{
}

ACE_Addr::~ACE_Addr (void)
{
}



void *
ACE_Addr::get_addr (void) const
{
  return 0;
}

void
ACE_Addr::set_addr (void *, int)
{
}


// Initializes instance variables.

void
ACE_Addr::base_set (int type, int size)
{
  this->addr_type_ = type;
  this->addr_size_ = size;
}


void *
ACE_INET_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr");
  return (void*)&this->inet_addr_;
}

void
ACE_INET_Addr::set_addr (void *addr, int len)
{
  this->set_addr (addr, len, 0);
}

// Set a pointer to the address.
void
ACE_INET_Addr::set_addr (void *addr, int /* len */, int map)
{
  ACE_TRACE ("ACE_INET_Addr::set_addr");
  
  
}


ACE_INET_Addr::ACE_INET_Addr (void)
  : ACE_Addr (determine_type (), sizeof (inet_addr_))
{
  // ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->reset ();
}

// Creates a ACE_INET_Addr from a PORT_NUMBER and an Internet address.

ACE_INET_Addr::ACE_INET_Addr (u_short port_number,
                              ACE_UINT32 inet_address)
  : ACE_Addr (determine_type (), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->reset ();
  if (this->set (port_number, inet_address) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_INET_Addr::ACE_INET_Addr")));
}

int
ACE_INET_Addr::set (u_short port_number,
                    ACE_UINT32 inet_address,
                    int encode,
                    int map)
{
  ACE_TRACE ("ACE_INET_Addr::set");
  this->set_address (reinterpret_cast<const char *> (&inet_address),
                     sizeof inet_address,
                     encode, map);
  this->set_port_number (port_number, encode);

  return 0;
}


int ACE_INET_Addr::set_address (const char *ip_addr,
                                int len,
                                int encode /* = 1 */,
                                int map /* = 0 */)
{
  ACE_TRACE ("ACE_INET_Addr::set_address");
  // This is really intended for IPv4. If the object is IPv4, or the type
  // hasn't been set but it's a 4-byte address, go ahead. If this is an
  // IPv6 object and <encode> is requested, refuse.
  // if (encode && len != 4)
    // {
      // errno = EAFNOSUPPORT;
      // return -1;
    // }

  if (len == 4)
    {
      ACE_UINT32 ip4 = *reinterpret_cast<const ACE_UINT32 *> (ip_addr);
      if (encode)
	  {
        // ip4 = ACE_HTONL (ip4);
		ip4 = htonl (ip4);
	  }

      if (this->get_type () == AF_INET && map == 0) {
        this->base_set (AF_INET, sizeof (this->inet_addr_.in4_));
#ifdef ACE_HAS_SOCKADDR_IN_SIN_LEN
        this->inet_addr_.in4_.sin_len = sizeof (this->inet_addr_.in4_);
#endif
        this->inet_addr_.in4_.sin_family = AF_INET;
        this->set_size (sizeof (this->inet_addr_.in4_));
        memcpy (&this->inet_addr_.in4_.sin_addr,
                        &ip4,
                        len);
      }

      return 0;
    }   /* end if (len == 4) */
	
  // Here with an unrecognized length.
  // errno = EAFNOSUPPORT;
  return -1;

}	

void
ACE_INET_Addr::set_port_number (u_short port_number,
                                int encode)
{
  ACE_TRACE ("ACE_INET_Addr::set_port_number");

  if (encode)
  {
    // port_number = ACE_HTONS (port_number);
    port_number = htons (port_number);
  }

  this->inet_addr_.in4_.sin_port = port_number;
}


ACE_INLINE int
ACE_INET_Addr::determine_type (void) const
{

  return AF_INET;

}

ACE_INLINE void
ACE_INET_Addr::reset (void)
{
  ::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
  if (this->get_type() == AF_INET)
    {
#ifdef ACE_HAS_SOCKADDR_IN_SIN_LEN
      this->inet_addr_.in4_.sin_len = sizeof (this->inet_addr_.in4_);
#endif
      this->inet_addr_.in4_.sin_family = AF_INET;
    }

}