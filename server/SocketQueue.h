#pragma once
#ifndef _SOCKETQUEUE_H_
#define _SOCKETQUEUE_H_
#include "stdafx.h"
#include <queue>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <mutex>

class SocketQueue
{
	typedef std::vector<T_SocketInfo> SOCKETSET;
	typedef std::queue<T_MsgData> MSGQUEUE;
public:
	std::mutex mx;
	SocketQueue(SOCKET socket, SOCKADDR_IN addr);
	virtual ~SocketQueue();
	void initSocket();
	void start();
	size_t findSocket(const SOCKET &socket);
	static void acceptSocket(SocketQueue* sq);
	static void recvMsg(SocketQueue* sq);
private:
	SOCKETSET socketSet;
	MSGQUEUE msgQueue;
	SOCKET serverSock;
	SOCKADDR_IN serverAddr;
};

#endif // !_SOCKETQUEUE_H_



