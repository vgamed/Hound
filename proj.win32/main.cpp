#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

#ifdef  UNICODE

bool TSTR2A(LPCTSTR p_tstr, std::string &ret)
{
	do{
		if (p_tstr == nullptr)
			break;

		int len = WideCharToMultiByte(CP_ACP, 0, p_tstr, -1, 0, 0, nullptr, nullptr);
		if (len > 0)
		{
			char * buf = new char[len+1];
			buf[0] = '\0';
			WideCharToMultiByte(CP_ACP, 0, p_tstr, -1, buf, len+1, nullptr, nullptr);
			ret = buf;
			delete [] buf;
			return true;
		}
		else break;
	}while(0);

	ret = "";
	return false;
}

#else //UNICODE

bool TSTR2A(LPCTSTR p_tstr, std::string &ret)
{
	if (p_tstr == nullptr)
	{
		ret = "";
	}
	else
	{
		ret = p_tstr;
	}

	return true;
}

#endif //UNICODE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	std::string cmd_line;
	TSTR2A(lpCmdLine, cmd_line);
	if (cmd_line.find("-gendb") != std::string::npos)
	{
		DataCenter::getInstance()->generateDummyDatabase();
		return 0;
	}

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
