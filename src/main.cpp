#include <glog/logging.h>
#include <string>

using namespace std;
int main(int argc, char **argv)
{
	string time = "joint";
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::GLOG_INFO, time.c_str());
	google::SetStderrLogging(google::NUM_SEVERITIES);
	google::SetLogFilenameExtension(".log_");
	
	int count = 0;
	while (1)
	{
		if (count++ % 20 == 0)
		{
			google::SetLogDestination(google::GLOG_INFO, (time + to_string(count)).c_str());
		}
		LOG(ERROR)<<"ERROR Hello world";
		LOG(INFO)<<"INFO Hello world";
		LOG(WARNING)<<"WARNING Hello world";
		usleep(10000);
		if (count >= 100)
		{
			break;
		}
	}
	
	return 0;
}