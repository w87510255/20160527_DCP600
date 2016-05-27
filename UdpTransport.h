#ifndef UDPTRANSPORT_H
#define UDPTRANSPORT_H

#include "head.h"

typedef struct sockaddr* LPSOCKADDR;
typedef const struct sockaddr* LPCSOCKADDR;
typedef struct sockaddr SOCKADDR;

namespace udp
{
	const int MAXWAITTIME = 10;

	class UdpTransport
	{
		void fnInit();
	public:
		UdpTransport();
		~UdpTransport();

		int fnCreate(const char *pIp, unsigned short unPort);
		int fnCreate();
		int fnSetAddr(const char *pIp, const unsigned short unPort);
		bool fnBeConnected() const;
		int fnClose();
		int fnWrite(const char *pBuf, const int iSize, const int iSecs = MAXWAITTIME);
		int fnUdpRecv(char *pBuf, int iSize, const int iSecs = MAXWAITTIME);		
		int fnUdpSend(const char *pBuf,const int iSize, const int iSecs = MAXWAITTIME);	
		int fnSendDatagram(const char *pBuf, const int iSize, LPCSOCKADDR psa,
						   const int iSecs);
		int fnReceiveDatagram(char *pBuf, const int iSize, LPSOCKADDR psa,
							  const int iSecs);
	private:
		int m_iFd;
		sockaddr_in addr;
		sockaddr_in addr_Recv;
	};
};
		
		




#endif
