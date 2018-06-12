#ifndef SOCKET_STREAM_H
#define SOCKET_STREAM_H

#include "acl_include.h"

namespace acl {

class ACL_CPP_API stream
{
public:
	stream(void);
	virtual ~stream(void) = 0;

	/**
	 * 调用本函数关闭流连接
	 * @return {bool} true: 关闭成功; false: 关闭失败
	 */
	bool close(void);
};	
struct ACL_VSTREAM {
};

class ACL_CPP_API socket_stream : public stream
{
public:


	socket_stream();
	virtual ~socket_stream();

	/**
	 * 根据 ACL_VSTREAM 流打开网络流
	 * @param vstream {ACL_VSTREAM*}
	 * @param udp_mode {bool} 是否是 UDP 方式
	 * @return {bool} 连接是否成功
	 */
	bool open(ACL_VSTREAM* vstream, bool udp_mode = false);

	/**
	 * 连接远程服务器并打开网络连接流
	 * @param addr {const char*} 服务器地址, 若连接域套接口服务器(仅UNIX平台),
	 *  域套接地址：/tmp/test.sock; 如果连接一个TCP服务器，则地址格式为:
	 *  [${local_ip}@]${remote_addr}, 如: 60.28.250.199@www.sina.com:80,
	 *  意思是绑定本的网卡地址为: 60.28.250.199, 远程连接 www.sina.com 的 80,
	 *  如果由OS自动绑定本地 IP 地址，则可以写为：www.sina.com:80
	 * @param conn_timeout {int} 连接超时时间(秒)
	 * @param rw_timeout {int} 读写超时时间(秒)
	 * @return {bool} 连接是否成功
	 */
	bool open(const char* addr, int conn_timeout, int rw_timeout);

	/**
	 * 输出缓冲区中的数据
	 * @param s {const std::string&}
	 * @param loop {bool} 是否要求全部输出完才返回
	 * @return {int} 输出数据的长度，返回 -1 表示出错
	 */
	int write(const std::string& s, bool loop = true);

	/**
	 * 写数据至输出流中
	 * @param data {const void*} 数据指针地址
	 * @param size {size_t} data 数据长度(字节)
	 * @param loop {bool} 是否保证数据全部输出才返回，如果为 true,
	 *  则该函数直至数据全部输出或出错才会返回；否则仅写一次便返回，
	 *  但并不保证数据全部写完
	 * @param buffed {bool} 是否先缓存待写的数据
	 * @return {int} 真实写入的数据量, 返回 -1 表示出错
	 */
	int write(const void* data, size_t size, bool loop = true,
		bool buffed = false);

	/**
	 * 从输入流中读数据至缓冲区中
	 * @param s {std::string*} 缓冲区，内部会首先自动清空该缓冲区
	 * @param loop {bool} 是否阻塞式读满整个缓冲，缓冲区
	 *  的容量为 s.capacity()
	 * @return {bool} 读数据是否成功
	 */
	bool read(std::string& s, bool loop = true);

	/**
	 * 从输入流中读数据
	 * @param buf {void*} 用户缓冲区
	 * @param size {size_t} 用户缓冲区长度
	 * @param loop {bool} 是否读满 size 后才返回
	 * @return {int} 读操作结果, -1 表示关闭或出错, > 0 表示成功
	 */
	int read(void* buf, size_t size, bool loop = true);	
				
};

}

#endif