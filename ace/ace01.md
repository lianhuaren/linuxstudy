
配置环境变量： 
vi ~/.bashrc

ACE_ROOT=~/Software/ACE_wrappers
export ACE_ROOT
LD_LIBARY_PATH=$ACE_ROOT/ace:$LD_LIBARY_PATH
export LD_LIBARY_PATH

source ~/.bashrc //生效环境变量

[https://blog.csdn.net/hou512504317/article/details/70224691](https://blog.csdn.net/hou512504317/article/details/70224691)

接着使用如下方法才编译成功ACE+epoll，方法如下：

1、cd $ACE_ROOT/ace

     vi config.h   --新建

 添加：#define ACE_HAS_STANDARD_CPP_LIBRARY 1

         #define ACE_HAS_EVENT_POLL 1
         #include "config-linux.h"

2、cd $ACE_ROOT/include/makeinclude

    vi platform_macros.GNU   --新建

添加：include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU

3、cd $ACE_ROOT/ace

     make -f GNUmakefile

[https://www.cnblogs.com/huanghuang/archive/2012/09/25/2701103.html](https://www.cnblogs.com/huanghuang/archive/2012/09/25/2701103.html)

g++ Server.cpp -g -L$ACE_ROOT/lib -lACE -lrt

ubuntu安装ACE过程

../configure --disable-ssl

如果共享库文件安装到了/usr/local/lib(很多开源的共享库都会安装到该目录下)或其它"非/lib或/usr/lib"目录下, 那么在执行ldconfig命令前, 还要把新共享库目录加入到共享库配置文件/etc/ld.so.conf中, 如下:

# cat /etc/ld.so.conf
include ld.so.conf.d/*.conf
# echo "/usr/local/lib" >> /etc/ld.so.conf
# ldconfig


[https://www.cnblogs.com/Anker/p/3209876.html](https://www.cnblogs.com/Anker/p/3209876.html)

网络编程
[https://blog.csdn.net/russell_tao/article/details/17119729](https://blog.csdn.net/russell_tao/article/details/17119729)

2011年10月14日 16:37:53

     描述下本人电脑情况：

     虚拟机版本：VMware-workstation-full-v7.1.4；

     ACE版本：ACE6.0.0

     虚拟机Linux版本：Ubuntu10.10 Desktop；



     安装ACE，最简单的方法，是直接apt-get安装


sudo apt-get install libace-dev  
    这样，ace文件则位于/usr/include下，libACE.so位于/usr/lib下，楼主机器的libace-dev版本为5.6.3-6
 

   也可以手动下载源码编译安装：

    1、到 http://download.dre.vanderbilt.edu/下载ACE源码，拷贝到虚拟机目录/home/xia/，并解压


xia@ubuntu:~$ tar -xvf ACE-6.0.0.tar.bz2   
    2、进入ACE顶级目录，并创建文件夹


xia@ubuntu:~$ cd ACE_wrappers/  
xia@ubuntu:~/ACE_wrappers$ mkdir objdir  
xia@ubuntu:~/ACE_wrappers$ cd objdir  
xia@ubuntu:~/ACE_wrappers/objdir$   
   3、配置


xia@ubuntu:~/ACE_wrappers/objdir$ ../configure  
   配置完成后，objdir文件就下会有很多文件，包括Makefile，如下：



  4、编译


xia@ubuntu:~/ACE_wrappers/objdir$ sudo make  
    这里直接make会得到以下错误：error: openssl/bio.h: no such file or dictionary，参考文档ACE-SSL，需要安装openssl，于是安装

xia@ubuntu:~/ACE_wrappers/objdir$ sudo apt-get install openssl  
     安装后问题依旧，安装libssl-dev，ssl开发包

xia@ubuntu:~/ACE_wrappers/objdir$ sudo apt-get install libssl-dev  
     再次make，遇到错误，undefined reference to `SSL_read'
   全部是ssl前缀的项找不到，于是修改makefile文件，让其编译时加载ssl库，找到Makefile中如下内容：


LIBS = -lrt -ldl    
    在其后添加上-lssl，再次make，通过

(补充于11月13日：之后再次安装openssl的时候，采用的是下载源码安装，可以参考：http://www.cnblogs.com/blueclue/archive/2010/12/21/1911603.html 的解决方法，手动在test/SSL下的Makefile和examples/IPC_SAP/SSL_SAP中的Makefile中指定ssl库，如编译源码安装，则路径完全与以上参考所示相同)

5、安装



xia@ubuntu:~/ACE_wrappers/objdir$ sudo make install  
    这里采用sudo ，而不是直接make install，因为，直接make install，在ubuntu下，会得到permission deny的错误。
  至此，搞定，ace文件在/usr/local/include下，而libACE.so位于/usr/local/lib下    



 ps：这里安装的ACE6.0与直接apt-get方式安装的ACE，头文件会有一些不同的地方，如编译安装方式，则include/ace下没有OS.h和OS_String.h这两个文件，很多ACE的示范代码也是以前的，所以会遇到一些问题，不过不大



参考网址：

1、博客园http://www.cnblogs.com/blueclue/archive/2010/12/21/1911603.html

2、ACE在线文档：http://www.dre.vanderbilt.edu/~schmidt/DOC_ROOT/ACE/ACE-INSTALL.html#unix_traditional

另外ACE文档推荐网站：http://www.acejoy.com/，其中有个帖子貌似不错http://www.acejoy.com/bbs/viewthread.php?tid=1260&extra=page%3D1

[https://blog.csdn.net/coder_xia/article/details/6873813](https://blog.csdn.net/coder_xia/article/details/6873813)