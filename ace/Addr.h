#ifndef ACE_ADDR_H
#define ACE_ADDR_H

#include "ACE_include.h"

class ACE_Export ACE_Addr
{
public:
  // = Initialization and termination methods.
  /// Initializes instance variables.
  ACE_Addr (int type = -1, int size = -1);

  /// Destructor.
  virtual ~ACE_Addr (void);

  // = Get/set the size of the address.

  /// Return the size of the address.
  int get_size (void) const;

  /// Sets the size of the address.
  void set_size (int size);

  // = Get/set the type of the address.

  /// Get the type of the address.
  int get_type (void) const;

  /// Set the type of the address.
  void set_type (int type);
  
  /// Return a pointer to the address.
  virtual void *get_addr (void) const;

  /// Set a pointer to the address.
  virtual void set_addr (void *, int len);
  
  
  // = Equality/inequality tests
  /// Check for address equality.
  bool operator == (const ACE_Addr &sap) const;

  /// Check for address inequality.
  bool operator != (const ACE_Addr &sap) const;
  
  /// Initializes instance variables.
  void base_set (int type, int size);

  /// Wild-card address.
  static const ACE_Addr sap_any;
  
protected:
  /// e.g., AF_UNIX, AF_INET, AF_SPIPE, etc.
  int addr_type_;

  /// Number of bytes in the address.
  int addr_size_;
};

ACE_INLINE bool
ACE_Addr::operator == (const ACE_Addr &sap) const
{
  return (sap.addr_type_ == this->addr_type_ &&
          sap.addr_size_ == this->addr_size_   );
}

ACE_INLINE bool
ACE_Addr::operator != (const ACE_Addr &sap) const
{
  return (sap.addr_type_ != this->addr_type_ ||
          sap.addr_size_ != this->addr_size_   );
}

/// Return the size of the address.
ACE_INLINE int
ACE_Addr::get_size (void) const
{
  return this->addr_size_;
}

/// Sets the size of the address.
ACE_INLINE void
ACE_Addr::set_size (int size)
{
  this->addr_size_ = size;
}

/// Return the type of the address.
ACE_INLINE int
ACE_Addr::get_type (void) const
{
  return this->addr_type_;
}

/// Set the type of the address.
ACE_INLINE void
ACE_Addr::set_type (int type)
{
  this->addr_type_ = type;
}

/**
 * @class ACE_INET_Addr
 *
 * @brief Defines a C++ wrapper facade for the Internet domain address
 * family format.
 */
class ACE_Export ACE_INET_Addr : public ACE_Addr
{
public:
  /// Default constructor.
  ACE_INET_Addr (void);
  /**
   * Creates an ACE_INET_Addr from a @a port_number and an Internet
   * @a ip_addr.  This method assumes that @a port_number and @a ip_addr
   * are in host byte order. If you have addressing information in
   * network byte order, @see set().
   */
  explicit ACE_INET_Addr (u_short port_number,
                          ACE_UINT32 ip_addr = INADDR_ANY);
	

  /// Return a pointer to the underlying network address.
  virtual void *get_addr (void) const;
  int get_addr_size(void) const;

  /// Set a pointer to the address.
  virtual void set_addr (void *, int len);

  /// Set a pointer to the address.
  virtual void set_addr (void *, int len, int map);
  
   /**
   * Initializes an ACE_INET_Addr from a @a port_number and an Internet
   * @a ip_addr.  If @a encode is non-zero then the port number and IP address
   * are converted into network byte order, otherwise they are assumed to be
   * in network byte order already and are passed straight through.
   *
   * If @a map is non-zero and IPv6 support has been compiled in,
   * then this address will be set to the IPv4-mapped IPv6 address of it.
   */
  int set (u_short port_number,
           ACE_UINT32 ip_addr = INADDR_ANY,
           int encode = 1,
           int map = 0); 
   /**
   * Sets the port number without affecting the host name.  If
   * @a encode is enabled then @a port_number is converted into network
   * byte order, otherwise it is assumed to be in network byte order
   * already and are passed straight through.
   */
  void set_port_number (u_short,
                        int encode = 1);

  /**
   * Sets the address without affecting the port number.  If
   * @a encode is enabled then @a ip_addr is converted into network
   * byte order, otherwise it is assumed to be in network byte order
   * already and are passed straight through.  The size of the address
   * is specified in the @a len parameter.
   * If @a map is non-zero, IPv6 support has been compiled in, and
   * @a ip_addr is an IPv4 address, then this address is set to the IPv4-mapped
   * IPv6 address of it.
   */
  int set_address (const char *ip_addr,
                   int len,
                   int encode = 1,
                   int map = 0);
				   
private:
  int determine_type (void) const;

  /// Initialize underlying inet_addr_ to default values
  void reset (void);
  
  union
  {
    sockaddr_in  in4_;
#if defined (ACE_HAS_IPV6)
    sockaddr_in6 in6_;
#endif /* ACE_HAS_IPV6 */
  } inet_addr_;
};
#endif
