#include "C_AudioDcp600.h"

using namespace std;
int main()
{
	C_AudioDCP600 Cli;
	int iValue, iMute, iChanel;
	vector<string> vecChanelList;
	string strName="wanglan", strPasswd="8888";	
	string strIp="127.0.0.1";
	string strVersion;
	unsigned short unPort = 8888;
	Cli.fnInit(strIp, unPort, strName, strPasswd);
	Cli.fnGetVersion(strVersion);
	Cli.fnGetAudioStatus(iValue, iMute, iChanel, vecChanelList);
	cout <<"Version=" << strVersion;
    
	
}
