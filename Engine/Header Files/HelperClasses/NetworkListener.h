#pragma once
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

class NetworkListener
{
private:
	SOCKET mListenSocket;
	SOCKET mTransferSocket;
	sockaddr_in mPeer;

	void CreateSocket(const int pPort);
	void Listen();
public:
	//Structors
	NetworkListener(const int pPort);
	~NetworkListener();
};