#include <glog/logging.h>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	string time = "00_";
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO, time.c_str());
	google::SetLogFilenameExtension(".txt");
	int count = 0;
	while (1)
	{
		if (count++ % 100 == 0)
		{
			time = to_string(count-1);
			google::SetLogDestination(google::GLOG_INFO, time.c_str());
		}
		LOG(ERROR)<<"ERROR Hello world";
		LOG(INFO)<<"INFO Hello world";
		LOG(WARNING)<<"WARNING Hello world";
		usleep(10000);
		if (count >= 1000)
		{
			break;
		}
	}
	
	return 0;
}