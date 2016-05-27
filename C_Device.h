#ifndef C_DEVICE_H
#define C_DEVICE_H

#include "head.h"

class C_Device
{
    int m_iErrorNum;
	std::string m_strErrorInfo;
protected:
	int m_iReadInterval;
	int m_iWriteInterval;
	int m_iReadWait;
public:
	C_Device();
	virtual ~C_Device();
	virtual int fnInit(const std::string &strIp, unsigned short unPort,
					   const std::string &strUserName, 
					   const std::string &strPassWd) = 0;		 
	int fnSetError(const int iErrorNum, const std::string &strErrorInfo); 
	int fnGetError(std::string &strErrorInfo) const;

	int fnGetReadInterval()
	{
		return m_iReadInterval;
	}
	int fnGetWriteInterval()
	{
		return m_iWriteInterval;
	}
	int fnGetReadWait()
	{
		return m_iReadWait;
	}
	void fnSetReadInterval(int iReadInterval)
	{
		m_iReadInterval = iReadInterval;
	}
	void fnSetWriteInterval(int iWriteInterval)
	{
		m_iWriteInterval = iWriteInterval;
	}
	void fnSetReadWait(int iReadWait)
	{
		m_iReadWait = iReadWait;
	}
		 
	virtual bool fnGetConnectState() = 0;
	virtual int fnClose() = 0;
	virtual int fnGetVersion(std::string &strVersion) = 0;
};


#endif
