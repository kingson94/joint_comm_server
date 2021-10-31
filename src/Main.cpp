#include <glog/logging.h>
#include <string>
#include <AppManager.h>
#include <signal.h>

using namespace std;

void SigpipeHanlder(int iSignal)
{
   
}


int main(int argc, char **argv)
{
	struct sigaction stSigAction;
    struct sigaction stSigActionOut;

    stSigAction.sa_handler = &SigpipeHanlder;
    // Restart interrupted system calls
    stSigAction.sa_flags = SA_RESTART;

    // Block every signal during the handler
    sigemptyset(&stSigAction.sa_mask);

    if (sigaction(SIGPIPE, &stSigAction, &stSigActionOut) < 0)
    {
        return -1;
    }

	google::InitGoogleLogging(argv[0]);
	// Create app instance
	int iResult = core::AppManager::CreateInstance();
	static core::AppManager *pAppInstance = NULL;
	if (iResult != 0)
	{
		DLOG(ERROR) << "Create instance failed";
	}
	pAppInstance = core::AppManager::GetInstance();

	// Register app component
	pAppInstance->RegisterComponents();

	// Run app
	if (pAppInstance)
	{
		pAppInstance->Run();
	}

	return 0;
}