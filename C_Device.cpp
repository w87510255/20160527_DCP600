#include "C_Device.h"

using namespace std;

int C_Device::fnSetError(const int iErrorNum, const string &strErrorInfo)
{
	m_strErrorInfo = strErrorInfo;
	m_iErrorNum = iErrorNum;
	
	return E_OK; 
}

int C_Device::fnGetError(string &strErrorInfo) const 
{
	strErrorInfo = m_strErrorInfo;
	
	return m_iErrorNum;
	
}

C_Device::C_Device()
{
	m_iReadInterval = 1;
}

C_Device::~C_Device()
{
}






