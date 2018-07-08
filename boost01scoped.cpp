#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
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
	//scoped_ptr<posix_file> p(new posix_file("/tmp/a.txt"));
	
	array<int, 10> ar;
	ar[0] = 1;
	ar.back() = 10;
	assert(ar[ar.max_size() - 1] == 10);
	
	ar.assign(777);
	for (auto x : ar)
	{
		cout << x << "x"; 
	}
	
	int *p = ar.c_array();
	*(p+5) = 253;
	cout << ar[5] << endl;
	
	ar.at(8) = 666;
	sort(ar.begin(), ar.end());
	
	

	return 0;
}