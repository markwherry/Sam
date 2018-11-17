#ifndef __AUdpSocket_h__
#define __AUdpSocket_h__

#include "APlatform.h"
#include "ATypes.h"

namespace Atom {

/*union floatchar
{
	float f;
	char c[4];
};*/

class AUdpSocket
{
public:
	AUdpSocket();
	int open(int portNumber, char *address = 0, char *addrDest = 0);
	int close(void);
	int sendBuffer(char *_ipAddress, char *_buffer, long _bufferSize);
	int receive(char *_buffer, long _bufferSize);
	int isSomethingThere(long microSeconds = 4);
	void somethingThereCleanup(void);

	int waitForBuffer(char *buffer, int bufferSize, unsigned long *address);
	int receiveBuffer(char *buffer, int bufferSize, unsigned long *address, int flags =0);

	//static void floatToChar(float *fInput, long numFrames, int frameSize, char *cOutput, int offset);
	static void floatToChar(float *fInput, long size, char *cOutput, int offset);
	static void shortToChar(short *value, int size, char *output, int offset);
	static void charToShort(char *buffer, int size, short *output, int offset);

	char *lastReceivedAddress(void) { return inet_ntoa(si_other.sin_addr); }

	bool isConnected(void) { return m_connected; }

private:

//#ifdef WINDOWS32
	SOCKET s;
	WSADATA wsaData;
//#endif

#ifdef __APPLE__
	int s;
#endif
	bool m_connected;
	int slen;
	fd_set socketCheck;
	//timeval timeToWait;
	sockaddr_in si_me, si_other, si_send, sockaddrOther;
	int m_portNumber;

	char m_lastReceivedAddress[16];
};

}

#endif // __AUdpSocket_h__