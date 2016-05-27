#ifndef C_AUDIODEVICE_H
#define C_AUDIODEVICE_H

#include "C_Device.h"

class C_AudioDevice : public C_Device
{
public:
	C_AudioDevice():C_Device(){};
	virtual ~C_AudioDevice(){}

	enum
	{
		GAMECHANNEL = 100
	};

	virtual int fnGetAudioStatus(int &iValue, int &iMute, int &iChanel,
								std::vector<std::string> &vecChanelList) = 0;
	virtual int fnSetValue( int iValue) = 0;
	virtual int fnSetMute(int iMute ) = 0;
	virtual int fnSetChanel(int iChanel) = 0;
	virtual int fnSetValueAndMute(int iValue, int iMute) = 0;
	virtual int fnSetChanel( const std::string &strChanel) =0;
};
			






#endif
