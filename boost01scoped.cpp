#include <boost/smart_ptr.hpp>
#include <iostream>
using namespace boost;
using namespace std;

struct posix_file
{
	posix_file(const char * file_name)
	{
		cout << "open file:" << file_name << endl;
	}
	~posix_file()
	{
		cout << "close file" << endl;
	}
};

int main()
{
	scoped_ptr<posix_file> p(new posix_file("/tmp/a.txt"));
	return 0;
}