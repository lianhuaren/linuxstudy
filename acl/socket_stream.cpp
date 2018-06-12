#include "socket_stream.h"

namespace acl {

stream::stream(void)
// : hook_(NULL)
// , stream_(NULL)
// , eof_(true)
// , opened_(false)
// , default_ctx_(NULL)
{
}

stream::~stream(void)
{
	// if (hook_)
	// 	hook_->destroy();
	// if (stream_)
	// 	acl_vstream_free(stream_);
}

bool stream::close()
{
	return true;
}





socket_stream::socket_stream()
{
	// dummy_[0] = 0;
	// peer_ip_[0] = 0;
	// local_ip_[0] = 0;
}

socket_stream::~socket_stream()
{
	close();
}


bool socket_stream::open(const char* addr, int conn_timeout, int rw_timeout)
{
	return true;
	// ACL_VSTREAM* conn = acl_vstream_connect(addr, ACL_BLOCKING,
	// 	conn_timeout, rw_timeout, 8192);
	// if (conn == NULL)
	// 	return false;

	// return open(conn);
}

bool socket_stream::open(ACL_VSTREAM* vstream, bool udp_mode /* = false */)
{
	// // 先关闭旧的流对象
	// if (stream_)
	// 	acl_vstream_close(stream_);
	// stream_ = vstream;
	// eof_ = false;
	// opened_ = true;

	// if (udp_mode)
	// 	acl_vstream_set_udp_io(stream_);
	return true;
}

int socket_stream::write(const std::string& s, bool loop)
{
	return write(s.c_str(), s.length(), loop);
}

int socket_stream::write(const void* data, size_t size, bool loop /* = true */,
	bool buffed /* = false */)
{
	return true;
	//int   ret;
	// if (loop)
	// {
	// 	if (buffed)
	// 		ret = acl_vstream_buffed_writen(stream_, data, size);
	// 	else
	// 		ret = acl_vstream_writen(stream_, data, size);
	// }
	// else
	// 	ret = acl_vstream_write(stream_, data, (int) size);

	// if (ret == ACL_VSTREAM_EOF)
	// 	eof_ = true;
	//return ret;
}

bool socket_stream::read(std::string& s, bool loop /* = true */)
{
	return false;
	//s.clear();
	//int   ret;

	//if ((ret = read(s.buf(), s.capacity(), loop)) == -1)
	//	return false;
	//s.set_offset(ret);
	//return true;
}

int socket_stream::read(void* buf, size_t size, bool loop /* = true */)
{
	return true;
	//int   ret;
	// if (loop && size > 1)
	// 	ret = acl_vstream_readn(stream_, buf, size);
	// else
	// 	ret = acl_vstream_read(stream_, buf, size);
	// if (ret == ACL_VSTREAM_EOF)
	// {
	// 	CHECK_ERROR(errno);
	// 	return -1;
	// } else
	//	return ret;
}


}