#include "C_AudioDcp600.h"

using namespace std;

const unsigned short UDP_PORT=8888;
const int BUFFERLENGTH = 1024*1;
const int MINDELYTIME = 30;

C_AudioDCP600::C_AudioDCP600():C_AudioDevice(), m_bConnected(0)
{
	m_vecChanelList.clear();
	m_vecChanelList.push_back("Digital8");	
	m_vecChanelList.push_back("Anglog8");	
	m_vecChanelList.push_back("NonSync");	
	m_vecChanelList.push_back("Aux");	
	m_vecChanelList.push_back("unknow");	
}

C_AudioDCP600::~C_AudioDCP600()
{
	m_bConnected = 0;
}

int C_AudioDCP600::fnInit(const string &strIp, unsigned short unPort,
						 const string &strUserName, const string &strPasswd)
{
	m_strIp = strIp;
	m_unPort = unPort;
	m_strUserName = strUserName;
	m_strPassWd = strPasswd;

	m_iIsLoadFunc_GetAudioStatus = 0;							  

	m_bConnected = true;

#ifdef LOG
	WDC600LOG( m_strIp + "DCP-600_Init" );
#endif

	return E_OK;
}

int C_AudioDCP600 :: fnClose()
{
	return E_OK;
}	

bool C_AudioDCP600::fnGetConnectState()
{
	return m_bConnected;
}	

int C_AudioDCP600::fnGetVersion(string &strVersion)
{
	strVersion = "ENPAR DCP-600";
	return E_OK;
}	
int C_AudioDCP600::fnGetAudioStatus(int &iValue, int &iMute, int &iChanel,
								  vector<string> &vecChanelList)
{
	int iResult = 0;
	int iCurrentChanel = 0;
	int iCurrentVolume = 0;
	int iCurrentMute = 0;
	
#ifdef LOG
	WDCP600LOG(m_strIp + "DCP600_GetAudioStatus");
#endif   
	
	iResult = fnGetAll(iCurrentVolume, iCurrentMute, iCurrentChanel);
	if(0 == iResult)
	{
		iValue = iCurrentVolume;
		iMute = iCurrentMute;
		iChanel = iCurrentChanel;

		vecChanelList = m_vecChanelList;
		m_bConnected = true;
		m_iIsLoadFunc_GetAudioStatus = 1;
		m_iCurChannelNum = iCurrentChanel;
	
		return E_OK;
	}
	else
	{
		m_bConnected = false;
		fnClose();
		return E_FAIL;
	}
}

int C_AudioDCP600::fnGetAll(int &iVolume, int &iMute, int &iChanel)
{
	int iResult = 0;
	int iRecvDataLen = 0;
	char szSendBuf[BUFFERLENGTH];
	char szRecvBuf[BUFFERLENGTH];
	
	memset(szSendBuf, 0x00, sizeof(szSendBuf));
	memset(szRecvBuf, 0x00, sizeof(szRecvBuf));

	szSendBuf[0] = 0x53;
	szSendBuf[1] = 0x65;
	szSendBuf[2] = 0x74;
	szSendBuf[3] = 0x50;
	szSendBuf[4] = 0x61;
	szSendBuf[5] = 0x72;
	szSendBuf[6] = 0x61;
	szSendBuf[7] = 0x00;
	szSendBuf[8] = 0x00;
	szSendBuf[9] = 0x00;
	szSendBuf[10] = 0x44;
	szSendBuf[11] = 0x43;
	szSendBuf[12] = 0x50;
	szSendBuf[13] = 0x36;
	szSendBuf[14] = 0x30;
	szSendBuf[15] = 0x30;
	szSendBuf[16] = 0x43;
	szSendBuf[17] = 0x6f;
	szSendBuf[18] = 0x6e;
	szSendBuf[19] = 0x66;
	szSendBuf[20] = 0x69;
	szSendBuf[21] = 0x67;
	szSendBuf[22] = 0x44;
	szSendBuf[23] = 0x61;
	szSendBuf[24] = 0x74;
	szSendBuf[25] = 0x61;
	szSendBuf[26] = 0x00;
	szSendBuf[27] = 0x00;
	szSendBuf[28] = 0x00;
	szSendBuf[29] = 0x00;
	szSendBuf[30] = 0xca;
	szSendBuf[31] = 0x03;
	szSendBuf[32] = 0x00;
	szSendBuf[33] = 0x00;
	szSendBuf[34] = 0x58;
	szSendBuf[35] = 0x03;
	szSendBuf[36] = 0x00;
	szSendBuf[37] = 0x00;

	int iRequestSize = 38;
	int iCmd_GetStatusLen = iRequestSize;

	iResult = fnSendAndRecv(szSendBuf, iCmd_GetStatusLen,
						  szRecvBuf, iRecvDataLen);
	if (iResult != 0)
		return iResult;
	
	iResult = fnGetMuteRet(szRecvBuf, iMute, iRecvDataLen);
	if (iResult != 0)
		return iResult;
	
	iResult = fnGetChanelType(szRecvBuf, iChanel, iRecvDataLen);
	if (iResult != 0)
		return iResult;

	iResult = fnGetVolumeRet(szRecvBuf, iRecvDataLen);
	if (0 == iResult)
	{
		if (0 == iChanel)
			iVolume = m_szChannel_VolList[0];
		else if (1 == iChanel)
			iVolume = m_szChannel_VolList[1];
		else if (2 == iChanel)
			iVolume = m_szChannel_VolList[2];
		else if (3 == iChanel)
			iVolume = m_szChannel_VolList[3];
		else 
			iVolume = 70;
	}
	else 
		return iResult;

	return E_OK;
}

int C_AudioDCP600::fnSendAndRecv(const char*pSendBuf, int iSendDataLen, 
								 char *pRecvBuf, int &iRecvDataLen)
{
	int iResult = -1;
	int iRecvLen = 0;
	int iSendLen = 0;

	udp::UdpTransport udp;

	iResult = udp.fnCreate(m_strIp.c_str(), m_unPort);
	if (iResult != 0)
		return iResult;

	iResult = udp.fnUdpSend(pSendBuf, iSendDataLen);
	if (iResult > 0)
		iSendLen = iResult;

	if((iResult <= 0) || (iSendDataLen != iResult))
	{
		m_bConnected = false;
		fnClose();
		udp.fnClose();

		if (iResult <= 0)
			return iResult;     //待定
	}
	
	iResult = -1;
	iRecvLen = 0;
	iResult = udp.fnUdpRecv(pRecvBuf, BUFFERLENGTH);
	if (iResult > 0)
	{
		iRecvLen = iResult;
	//	iRecvDataLen += iRecvLen; //待定
	}
	else
	{
		udp.fnClose();
		fnClose();
		m_bConnected = false;

		return iResult;
	}
		
	udp.fnClose();
	m_bConnected = true;

	return E_OK;
}
	
int C_AudioDCP600::fnGetMuteRet(const char *pInfo, int &iMute, int iLen)
{
	if (g_iDcp600_Offset_MuteState > iLen)
		return E_FAIL;
	
	iMute = pInfo[g_iDcp600_Offset_MuteState] == 0x01 ? 1 : 0;
	return E_OK;
}

int C_AudioDCP600::fnGetVolumeRet(const char *pInfo, int iLen)
{
	if (g_iDcp600_Offset_Vol_Ch00 > iLen)
		return E_FAIL;
	memset( m_szChannel_VolList, pInfo[g_iDcp600_Offset_Vol_Ch00], 8);
	return E_FAIL;
}	
int C_AudioDCP600::fnGetChanelType(char *pInfo, int &iType, int iLen)
{
}
int C_AudioDCP600::fnSetValue(int iValue)
{
	if (iValue > 100 || iValue < 0)
		return E_FAIL;
	int iResult = 0;
}
int C_AudioDCP600::fnSetVolume( int iVolume)
{
}
int C_AudioDCP600::fnSetMute(int iMute)
{
}
int C_AudioDCP600::fnSetChanel(const string &strChanel)
{
}
int C_AudioDCP600::fnSetChanel( int iChanel)
{
}
		

int C_AudioDCP600::fnSetValueAndMute(int iValue, int iMute)
{
}













