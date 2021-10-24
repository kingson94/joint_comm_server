#include <glog/logging.h>
#include <string>

using namespace std;
int main(int argc, char **argv)
{
	google::InitGoogleLogging(argv[0]);
	DLOG(INFO) << "Hello";
	char c=0;
	return 0;
}