在unpv2的第7章使用了异步通信框架---非阻塞网络编程

1）结构定义：ACL_AIO, ACL_ASTREAM

2）主要函数：

创建异步句柄：
```c
ACL_AIO *acl_aio_create(int event_mode); 

ACL_AIO *acl_aio_create2(int event_mode, unsigned int nMsg);
```
    event_mode: ACL_EVENT_SELECT, ACL_EVENT_POLL, 
                     ACL_EVENT_KERNEL(epoll/devpool/kqueue/iocp), ACL_EVENT_WMSG(win32)

异步循环：void acl_aio_loop(ACL_AIO *aio);

释放异步句柄：void acl_aio_free(ACL_AIO *aio);

创建异步通信流：ACL_ASTREAM *acl_aio_open(ACL_AIO *aio, ACL_VSTREAM *stream);

异步IO完成后关闭异步流：void acl_aio_iocp_close(ACL_ASTREAM *astream);

设置异步流的参数：void acl_aio_ctl(ACL_ASTREAM *astream, int name, ...);

主动检查延迟关闭连接：void acl_aio_check(ACL_AIO *aio);

设置持续读状态：void acl_aio_set_keep_read(ACL_AIO *aio, int onoff);

设置异步定时器：acl_int64 acl_aio_request_timer(ACL_AIO *aio,
                                 ACL_AIO_TIMER_FN timer_fn, void *context, acl_int64 idle_limit);

取消异步定时器：acl_int64 acl_aio_cancel_timer(ACL_AIO *aio,
                                 ACL_AIO_TIMER_FN timer_fn, void *context);

参考：lib_acl/include/aio/acl_aio.h




设置异步监听流的回调函数：
方法一： 
```c
static int __accept_callback(ACL_ASTREAM *client, void *context)
{
do_something(client, context);
return 0;
}
acl_aio_ctl(ACL_ASTREAM *, ACL_AIO_CTL_ACCEPT_FN, __accept_callback, ACL_AIO_CTL_CTX, xxx, ACL_AIO_CTL_END);
```
方法二：
```c
static int  __listen_callback(ACL_ASTREAM *sstream, void *context)
{
ACL_ASTREAM* client;
ACL_VSTREAM* cstream = acl_vstream_accept(acl_aio_vstream(sstream));  // 接收客户端连接
If (cstream == NULL)
return (0);
client = acl_aio_open(acl_aio_handle(sstream), cstream);  // 将客户端连接转为异步流
do_something(client, context);
return 0;
}
acl_aio_ctl(ACL_ASTREAM *, ACL_AIO_CTL_LISTEN_FN, __listen_callback, ACL_AIO_CTL_CTX, xxx, ACL_AIO_CTL_END);
```
示例：
```c
ACL_AIO* aio = acl_aio_create(ACL_EVENT_KERNEL);  // 创建本机支持的内核级异步事件引擎对象
ACL_ASTREAM* sstream;
ACL_VSTREAM* server = acl_vstream_listen(“127.0.0.1:8080”, 128);  //  监听
acl_assert(server); 
sstream = acl_aio_open(aio, server);  // 将监听流转为异步流
acl_aio_ctl(sstream, ACL_AIO_CTL_ACCEPT_FN, __accept_callback, ACL_AIO_CTL_END);  // 设置异步服务流的回调函数
while (1) {
acl_aio_loop(aio);  // 异步事件循环
}
```




