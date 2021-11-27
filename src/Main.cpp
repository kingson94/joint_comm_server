#include <string>
#include <AppManager.h>
#include <signal.h>
#include <iostream>
#include "tcp/TcpServer.h"
#include "tcp/TcpClient.h"
#include "util/Utils.h"
#include "AppDefine.h"

#ifndef BUILD_MAJOR_VERSION
#define BUILD_MAJOR_VERSION "1"
#endif // BUILD_MAJOR_VERSION
#ifndef BUILD_MINOR_VERSION
#define BUILD_MINOR_VERSION "0"
#endif // BUILD_MINOR_VERSION
#ifndef BUILD_PATCH_VERSION
#define BUILD_PATCH_VERSION "0. Start program"
#endif // BUILD_PATCH_VERSION
using namespace std;

void SigpipeHanlder(int iSignal)
{
   SLOG2(slog::LL_DEBUG, "[App] Receive signal: %d", iSignal);
}

int main(int argc, char **argv)
{
	bool bIsBenchmark = false;
	int iBenchTimes = 100000;
	int iRunningMode = -1;
	std::string strAlias = "";
	
	if(argc > 1)
    {
     	for (int i = 0; i < argc ; i++)
        {
            if (strcmp(argv[i], "--version") == 0)
            {
             	std::cout << BUILD_MAJOR_VERSION << "." << BUILD_MINOR_VERSION << "." << BUILD_PATCH_VERSION << std::endl;
                return 0;
            }

			if (strcmp(argv[i], "--help") == 0)
            {
             	std::cout << "Using options: \"-client\" to run as Client. Client mode: \"-benchmark [request times]\" to perform server benchmark\n";
				std::cout << "Using options: \"-server\" to run as Server\n";
                return 0;
            }

			if (strcmp(argv[i], "-benchmark") == 0)
            {
				bIsBenchmark = true;
				
				if (argc > i + 1)
				{
					iBenchTimes = std::stoi(argv[i + 1]);
				}
				continue;
            }

			if (strcmp(argv[i], "-server") == 0)
            {
				// Server
				iRunningMode = RUNNING_MODE_SERVER;
				continue;
            }

			if (strcmp(argv[i], "-client") == 0)
            {
				// Client
				iRunningMode = RUNNING_MODE_CLIENT;
				continue;
            }

			if (strcmp(argv[i], "-alias") == 0)
            {
				// Client
				if (argc > i + 1)
				{
					strAlias = argv[i + 1];
				}
				continue;
            }
		}

		if (iRunningMode < 0)
		{
			SLOG2(slog::LL_DEBUG, "[App] Running mode invalid. Run with option \"--help\" for more info.");
		}
	}

	static core::AppManager *pAppInstance = NULL;
	struct sigaction stSigAction;
    struct sigaction stSigActionOut;
	int iResult = -1;

    stSigAction.sa_handler = &SigpipeHanlder;
    // Restart interrupted system calls
    stSigAction.sa_flags = SA_RESTART;

    // Block every signal during the handler
    sigemptyset(&stSigAction.sa_mask);

    if (sigaction(SIGPIPE, &stSigAction, &stSigActionOut) < 0)
    {
        return -1;
    }

	// Create app instance
	iResult = core::AppManager::CreateInstance();
	if (iResult != 0)
	{
		printf("Create instance failed\n");
	}
	pAppInstance = core::AppManager::GetInstance();

	// Run app
	if (pAppInstance)
	{
		pAppInstance->SetRunningMode(iRunningMode);

		// Register app component
		pAppInstance->RegisterComponents();
	
		if (iRunningMode == RUNNING_MODE_CLIENT)
		{
			pAppInstance->SetProfile(strAlias);
		}

		pAppInstance->Init();
		pAppInstance->Run();
	}

	if (iRunningMode == RUNNING_MODE_CLIENT)
	{
		if (bIsBenchmark)
		{
			std::string strMessage = "The quick brown fox jumps over the lazy dog ";
			for (int i = 0; i < iBenchTimes; i++)
			{
				pAppInstance->SendMessageToEndpoint(strMessage + std::to_string(i));
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		else
		{
			while (true)
			{
				std::string strTerChars = "";
				printf("Enter message (Q/q to exit): ");
				std::getline(std::cin, strTerChars);

				if (strTerChars == "Q" || strTerChars == "q")
				{
					return 0;
				}

				pAppInstance->SendMessageToEndpoint(strTerChars);
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
		}
	}
	else
	{
		// Server is a read only app =))
	}
	
	pAppInstance->DestroyInstance();
	SLOG(slog::LL_DEBUG, "[App] Application stopped");
	return 0;
}