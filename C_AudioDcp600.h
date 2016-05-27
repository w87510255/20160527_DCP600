#ifndef AUDIODCP600_H
#define AUDIODCP600_H

#include "C_AudioDevice.h"
#include "head.h"
#include "UdpTransport.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

const int g_iDcp600_Offset_MuteState = 0x369;
const int g_iDcp600_Offset_ChannelState = 0x35a;
const int g_iDcp600_Offset_Vol_Ch00 = 0x35d;
const int g_iDcp600_Offset_Vol_Ch01 = 0x35e;
const int g_iDcp600_Offset_Vol_Ch02 = 0x35f;
const int g_iDcp600_Offset_Vol_Ch03 = 0x360;

#define DEBUG

class C_AudioDCP600 : public C_AudioDevice
{
	std::vector<std::string> m_vecChanelList;
	std::string m_strIp;
	unsigned short m_unPort;
	std::string m_strUserName;
	std::string m_strPassWd;
	
	bool m_bConnected;
	char m_szChannel_VolList[8];
	int m_iCurChannelNum;
	int m_iIsLoadFunc_GetAudioStatus;
public:
	C_AudioDCP600();
	virtual ~C_AudioDCP600();
	virtual int fnInit(const std::string &strIp,  unsigned short unPort,
					   const std::string &strUserName,
					   const std::string &strPasswd);
	virtual int fnClose();
	virtual int fnGetAudioStatus(int &iValue, int &iMute, int &iChanel,
								std::vector<std::string> &vecChanelList);
	virtual int fnGetVersion(std::string &strVersion);
	virtual bool fnGetConnectState();
	virtual int fnSetValue(int iValue); 	
	virtual int fnSetChanel(int iChanel); 	
	virtual int fnSetChanel(const std::string &strChanel); 	
	virtual int fnSetValueAndMute(int iValue, int iMute);
	virtual int fnSetMute(int iMute);
private:
	int fnGetChanel(int &iChanel);
	int fnGetVolume(int &iVolue);
	int fnSetVolume(int iVolume);
	int fnSetChanel_type(int iType);
	int fnGetAll(int &iVolume, int &iMute, int &iChannel);
	int fnSendAndRecv(const char *pSendBuffer, int iSendDataLen, 
					   char *pRecvBuffer, int &iRecvDataLen);
	int fnGetMuteRet(const char *pInfo, int &iMute, int iLen);
	int fnGetChanelType(char *pInfo, int &iType, int iLen);
	int fnGetVolumeRet(const char *pInfo, int iLen);
};

#endif



