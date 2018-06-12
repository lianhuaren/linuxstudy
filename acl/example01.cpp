#include "acl_include.h"

//using acl;

int main(int argc, char* argv[])
{


	acl::socket_stream client;
	std::string addr = "www.baidu.com:80";//"mail.51iker.com:80";
	std::string request = "GET / HTTP/1.1\r\n"
		"HOST: www.baidu.com\r\n"
		"Connection: close\r\n\r\n";
	std::string respond;

	// acl::acl_cpp_init();

	if (client.open(addr.c_str(), 0, 0) == false) {
		//printf("connect %s error(%s)\n", addr.c_str(), acl_last_serror());
		printf("input any key to exit\n");
		getchar();
		return (0);
	}

	if (client.write(request) == -1) {
		//printf("write %s to %s error(%s)\n", request.c_str(), addr.c_str(), acl_last_serror());
		printf("input any key to exit\n");
		getchar();
	}
	printf("%s", request.c_str());

	while (1) {
		if (client.read(respond, false) == false)
			break;
		printf("%s", respond.c_str());
	}

	printf("ok, enter any key to exit\n");
	getchar();
	return 0;
}