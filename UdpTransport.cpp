#include "UdpTransport.h"

using namespace std;

namespace udp
{
	const int BUFFERSIZE = 1024;
	
	UdpTransport::UdpTransport():m_iFd(-1)
	{
		memset(&addr, 0x00, sizeof(addr));
		memset(&addr_Recv,0x00, sizeof(addr_Recv));
		fnInit();
	}

	UdpTransport::~UdpTransport()
	{
		if (fnBeConnected())
		{
			close(m_iFd);
			m_iFd = E_FAIL;
		}
	}

	int UdpTransport::fnSetAddr(const char* pIp, const unsigned short unPort)
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(unPort);
		addr.sin_addr.s_addr = inet_addr(pIp);
		return E_OK;
	}

	int UdpTransport::fnCreate()
	{
		if (m_iFd != -1)
		{
			close(m_iFd);
			m_iFd = -1;
		}

		m_iFd = socket(AF_INET, SOCK_DGRAM, 0);
		if( m_iFd < 0)
		{
			printf("socket is wrong\n");
			return E_FAIL;
		}

		timeval timeo;
		timeo.tv_sec = MAXWAITTIME;
		timeo.tv_usec = 0;
		setsockopt(m_iFd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeo, sizeof(timeo));
		return E_OK;
	}

	int UdpTransport::fnCreate( const char *pIp, unsigned short unPort)
	{
		if(pIp == NULL || unPort < 1024)
			return E_FAIL;

		if (m_iFd != -1)
		{
			close(m_iFd);
			m_iFd = -1;
		}

		m_iFd = socket(AF_INET, SOCK_DGRAM, 0);
		if( m_iFd < 0)
		{
			printf("socket is wrong\n");
			return E_FAIL;
		}

		timeval timeo;
		timeo.tv_sec = MAXWAITTIME;
		timeo.tv_usec = 0;
		fnSetAddr(pIp,unPort);
		setsockopt(m_iFd, SOL_SOCKET, SO_SNDTIMEO,(const char *) &timeo, sizeof(timeo)); 
		return E_OK;
	}

	bool UdpTransport::fnBeConnected() const
	{
		return (-1 == m_iFd)  ? false : true;
	}

	int UdpTransport :: fnClose()
	{
		if(fnBeConnected())
		{
			if( -1 == close(m_iFd))
			{
				m_iFd = -1;
				return E_FAIL;
			}
			m_iFd = -1;
		}
		return E_OK;
	}

	void UdpTransport::fnInit()
	{
	}
	
	int UdpTransport:: fnUdpRecv(char *pBuf,int iSize, const int iSecs)
	{
		return fnReceiveDatagram(pBuf, iSize, (sockaddr*)&addr_Recv, iSecs);
	}

	int UdpTransport::fnReceiveDatagram(char *pBuf,const int iSize, LPSOCKADDR psa,
										const int iSecs)
	{
		if (NULL == pBuf || m_iFd <= 0)
			return E_OK;
		
		timeval timeOut = {iSecs, 0};
		fd_set fdSet;
		FD_ZERO(&fdSet);
		FD_SET(m_iFd, &fdSet);

		int iResult = select(m_iFd+1, &fdSet, NULL, NULL, &timeOut);
		if (iResult < 0)
		{
			close (m_iFd);
			m_iFd = -1;
			return E_FAIL;
		}

		if(0 == iResult )
		{
			return E_FAIL;
		}
		socklen_t iFromSize = sizeof(SOCKADDR);
		int iBytesReceived = recvfrom(m_iFd, pBuf, iSize, 0, psa, &iFromSize);
		if (iBytesReceived < 0)
		{
			close (m_iFd);
			m_iFd = E_FAIL;
			return E_FAIL;
		}

		return iBytesReceived;
	}

	
	int UdpTransport::fnUdpSend(const char *pBuf,const int iSize, const int iSecs)
	{
		return fnSendDatagram(pBuf, iSize, (struct sockaddr*)&addr, iSecs);
	}


	int UdpTransport::fnSendDatagram(const char *pBuf,const int iSize, LPCSOCKADDR psa,
										const int iSecs)
	{
		if (NULL == pBuf || m_iFd <= 0)
			return E_OK;
		
		timeval timeOut = {iSecs, 0};
		fd_set fdSet;
		FD_ZERO(&fdSet);
		FD_SET(m_iFd, &fdSet);

		int iResult = select(m_iFd+1, NULL,&fdSet, NULL, &timeOut);
		if (iResult < 0)
		{
			close (m_iFd);
			m_iFd = -1;
			return E_FAIL;
		}

		if(0 == iResult )
		{
			return E_FAIL;
		}
		int iBytesSent = sendto(m_iFd, pBuf, iSize, 0, psa,sizeof(SOCKADDR));
		if (iBytesSent < 0)
		{
			close (m_iFd);
			m_iFd = E_FAIL;
			return E_FAIL;
		}

		return iBytesSent;
	}

	int UdpTransport::fnWrite(const char *pBuf, const int iSize, const int iSecs)
	{
		int iBytesSent = 0;
		int iBytesThisTime = 0;
		const char * pch1 = pBuf;
		while(iBytesSent <= iSize);
		{
			iBytesThisTime = fnUdpSend(pch1, iSize-iBytesSent, iSecs);
			iBytesSent += iBytesThisTime;
			pch1 += iBytesThisTime;
		}
		
		return iBytesSent;
	}
};
