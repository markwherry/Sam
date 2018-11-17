// udpsocket.cpp
// A class for managing a UDP socket
// MW Started: 09/11/03

//#ifndef WINDOWS32
//#define WINDOWS32
//#endif

//#ifdef WINDOWS32
//#include <windows.h>
//#endif

//#if MACOSX
//#include <CoreServices/CoreServices.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <sys/select.h>
//#include <time.h>
//#include <arpa/inet.h>
//#include <sys/unistd.h>
//#endif

#include <stdio.h>

#include "AUdpSocket.h"
#include "ALog.h"

namespace Atom {

// ----------------------------------------------------------------------------------------------------
AUdpSocket::AUdpSocket()
{
	m_connected = false;
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::open(int portNumber, char *address, char *addrDest)
{
	
	m_portNumber = portNumber;

	//#ifdef WINDOWS32
	if ( WSAStartup (0x101, &wsaData)!= 0){
		aLog("WSAStartup Failed.\n");
		return 0; 
	}

	aLog("WSAStartup OK.\n");

	//#endif
	
	/*s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET){
		#ifdef DEBUG
		printf ("Invalid Socket.\n");
		#endif
		return 0;
	} */

	if ((s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) <0){
		aLog("DEBUG: Invalid Socket.\n");
		return 0;
	}

	aLog("Socket OK.\n");

	bool bOptVal = true;
	int bOptLen = sizeof(bool);
	aLog("Setting SockOpt SO_BROADCAST...");
	int ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
	ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen);
	if(ret==SOCKET_ERROR) {
		aLog("SockOpt Error!");
		aLog("WSAGetLastError = %d\n", WSAGetLastError());
	} else {
		aLog("OK");
	}

	/*u_long iMode = 1;
	if(ioctlsocket(s, FIONBIO, &iMode)==SOCKET_ERROR) {
		csDebug("ioctlsocket Error");
	}
	csDebug("ioctlsocket OK");//*/



	si_me.sin_family = AF_INET;
	si_me.sin_port = htons (portNumber);
	if(!address) {
		si_me.sin_addr.s_addr = 0;
	} else {
		unsigned long ip = inet_addr(address);
		aLog("ip = %u", ip);
		if(ip==INADDR_NONE) {
			aLog("ip = INADDR_NONE");
		}
		si_me.sin_addr.s_addr = ip;
	}

	if (bind (s, (struct sockaddr*)&si_me, sizeof(si_me)) !=0 ){
		//#ifdef DEBUG
		aLog("Bind failed.\n");
		aLog("WSAGetLastError = %d\n", WSAGetLastError());
		//#endif
		return 0;
	}
	//#ifdef DEBUG
	aLog("Bind OK.\n");
	//#endif

	if(addrDest) {
		sockaddrOther.sin_family = AF_INET;
		sockaddrOther.sin_port = htons(portNumber);
		unsigned long ipOther = inet_addr(addrDest);
		//printf("ip = %u", ip);
		if(ipOther==INADDR_NONE) {
			aLog("ipOther = INADDR_NONE");
		}
		sockaddrOther.sin_addr.s_addr = ipOther;


		if(connect(s, (struct sockaddr*)&sockaddrOther, sizeof(sockaddrOther)) == SOCKET_ERROR) {
			aLog("Connect failed.\n");
			aLog("WSAGetLastError = %d\n", WSAGetLastError());
			return 0;
		}

		aLog("Connect OK.\n");
	}

	// Now the socket's initialised, let's set up some values.

	slen = sizeof(si_other);

	somethingThereCleanup();

	//timeToWait.tv_sec = 0;
	//timeToWait.tv_usec = 4;

	

	m_connected = true;

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::close(void)
{
//#ifdef WINDOWS32
	closesocket(s);
	WSACleanup ();
//#endif

//#ifdef MACOSX
//	shutdown (s, 2);
//#endif

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::sendBuffer(char *_ipAddress, char *_buffer, long _bufferSize)
{	
	//csDebug("CsUdpSocket::send()");

	si_send.sin_family = AF_INET;
	si_send.sin_port = htons (m_portNumber);
	si_send.sin_addr.s_addr = inet_addr (_ipAddress);

	int ret = sendto(s, _buffer, _bufferSize, 0, (sockaddr*)&si_send, sizeof(si_send));//*/

	//int ret = send(s, _buffer, _bufferSize, 0);

	if(ret==SOCKET_ERROR) {
		aLog("Send Error!");
		aLog("WSAGetLastError = %d\n", WSAGetLastError());
	} else {
		//csDebug("Packet Sent OK");
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::receive(char *_buffer, long _bufferSize)
{
	int rBufferSize = recvfrom (s, _buffer, _bufferSize, MSG_PEEK, (struct sockaddr*)&si_other, &slen);
	//csDebug("recvfrom=%d\n",recvfrom (s, buf, sizeof(buf), MSG_PEEK, (struct sockaddr*)&si_other, &slen));
	//csDebug("WSAGetLastError = %d\n", WSAGetLastError());
	recvfrom (s, _buffer, rBufferSize, 0, (struct sockaddr*)&si_other, &slen);
	//printf ("recvfrom=%d\n", recvfrom (s, buf, 6, 0, (struct sockaddr*)&si_other, &slen));

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::isSomethingThere(long microSeconds)
{
	timeval timeToWait;

	timeToWait.tv_sec = 0;
	timeToWait.tv_usec = microSeconds;

	return select (0, &socketCheck, NULL, NULL, &timeToWait);
}

// ----------------------------------------------------------------------------------------------------
void AUdpSocket::somethingThereCleanup (void)
{
	FD_ZERO (&socketCheck);
	FD_SET (s, &socketCheck);
}

// ----------------------------------------------------------------------------------------------------
void AUdpSocket::floatToChar(float *fInput, long size, char *cOutput, int offset)
{
	//csDebug("CsUdpSocket::floatToChar()");

	int bufferCounter = 0;

	for(int i=0; i<size; i++) {
		//csDebug("%d/%d",i, size);
		floatchar fc;
		fc.f = fInput[i];

		for (int j=0; j<4; j++) {
			cOutput[bufferCounter+offset]= fc.c[j];
			bufferCounter++;
		}
	}
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::waitForBuffer(char *buffer, int bufferSize, unsigned long *address)
{
	//csDebug("CsUdpSocket::waitForBuffer()");

	int ret = recvfrom(s, buffer, bufferSize, MSG_PEEK, (struct sockaddr*)&si_other, &slen);
	if(ret==SOCKET_ERROR) {
		aLog("wait Error!");
		aLog("WSAGetLastError = %d\n", WSAGetLastError());
		return 0;
	} else {
		//csDebug("Packet Sent OK");
	}

	*address = si_other.sin_addr.s_addr;

	//csDebug("bufferSize = %d", ret);

	return ret;
}

// ----------------------------------------------------------------------------------------------------
int AUdpSocket::receiveBuffer(char *buffer, int bufferSize, unsigned long *address, int flags)
{
	//csDebug("CsUdpSocket::receiveBuffer()");

	int ret = recvfrom(s, buffer, bufferSize, 0, (struct sockaddr*)&si_other, &slen);
	//int ret = recv(s, buffer, bufferSize, flags);
	if(ret==SOCKET_ERROR) {
		aLog("receiveBuffer Error!");
		aLog("WSAGetLastError = %d\n", WSAGetLastError());
		return 0;
	} else {
		//csDebug("Packet Sent OK");
	}

	*address = si_other.sin_addr.s_addr;

	/*m_lastReceivedAddress = inet_ntoa(si_other.sin_addr);
	if(m_lastReceivedAddress==0) {
		csDebug("m_lastReceivedAddress == NULL");
	} else {
		csDebug("m_lastReceivedAddress = [%s]", m_lastReceivedAddress);
	}*/

	return ret;
}

// ----------------------------------------------------------------------------------------------------
void AUdpSocket::shortToChar(short *value, int size, char *output, int offset)
{
	int bufferCounter = 0;

	for(int i=0; i<size; i++) {
		output[bufferCounter+offset] = (unsigned char)value[i];
		output[bufferCounter+offset+1] = (unsigned char)(value[i]>>8);
		bufferCounter+=2;
	}
}

// ----------------------------------------------------------------------------------------------------
void AUdpSocket::charToShort(char *buffer, int size, short *output, int offset)
{
	int bufferCounter = 0;

	for(int i=0; i<size/2; i++) {
		output[i] = (unsigned char)buffer[bufferCounter+offset+1]<<8;
		output[i] |= (unsigned char)buffer[bufferCounter+offset];
		bufferCounter += 2;
	}
}

}