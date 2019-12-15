#include "stdafx.h"
#include "SocketQueue.h"
#include <iostream>
#include <thread>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
using std::cout;
using std::endl;
using std::thread;
SocketQueue::SocketQueue(SOCKET socket, SOCKADDR_IN addr): serverSock(socket), serverAddr(addr)
{
	socketSet.clear();
	while (!msgQueue.empty())
	{
		msgQueue.pop();
	}

}


SocketQueue::~SocketQueue()
{
	if (!socketSet.empty())
	{
		for (auto &sock : socketSet)
		{
			closesocket(sock.first);
		}
	}
	closesocket(serverSock);
	WSACleanup();
}

void SocketQueue::initSocket()
{
	int ret = bind(serverSock, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
	if ( ret == SOCKET_ERROR)
	{
		cout << "bind error !" << endl;
	}
	cout << "bind error !" << WSAGetLastError() << endl;
	//¿ªÊ¼¼àÌý
	if (listen(serverSock, MAX_LISTEN_NUM) == SOCKET_ERROR)
	{
		cout << "listen error !" << WSAGetLastError() << endl;
	}
	DWORD ul = 1;
	if (ioctlsocket(serverSock, FIONBIO, (unsigned long *)&ul) == SOCKET_ERROR)
	{
		cout << "ioctlsocket() failed with error" << endl;
	}
}

void SocketQueue::start()
{
	thread h1(acceptSocket, this);
	thread h2(recvMsg, this);
	h1.join();
	h2.join();
}

size_t SocketQueue::findSocket(const SOCKET & socket)
{
	size_t index = -1;
	for (auto &sock : socketSet)
	{
		index++;
		if (sock.first == socket)
		{
			return index;
		}
	}
	return -1;
}

void SocketQueue::acceptSocket(SocketQueue* sq)
{
	cout << "start acceptSocket thread!\n";

	SOCKADDR_IN remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	SOCKET sClient;
	T_SocketInfo socketInfo;
	while (true)
	{
		sClient = accept(sq->serverSock, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			cout << "nothing!\n";
			Sleep(500);
			continue;
		}
		if (sq->findSocket(sClient) == -1)
		{
			std::lock_guard<std::mutex> lk(sq->mx);
			socketInfo.first = sClient;
			socketInfo.second = remoteAddr;
			sq->socketSet.emplace_back(socketInfo);
			cout << "this is a new connection! ip: " << inet_ntoa(remoteAddr.sin_addr)
				<< ", port: " << remoteAddr.sin_port << endl;
		}
	}

}

void SocketQueue::recvMsg(SocketQueue* sq)
{
	cout << "start recvMsg thread!\n";
	char buf[1024];
	while (true)
	{
		for (auto iter = sq->socketSet.begin(); iter != sq->socketSet.end();)
		{
			SOCKET& sock = iter->first;
			//memset(buf, 0, sizeof(buf));
			std::lock_guard<std::mutex> lk(sq->mx);
			int ret = recv(sock, buf, sizeof(buf), 0);
			if (ret == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				switch (err)
				{
					case WSAEWOULDBLOCK: break;
					case WSAETIMEDOUT:
						cout << "connection (" << inet_ntoa(iter->second.sin_addr) << ") timeout!\n"; 
						closesocket(sock);
						iter = sq->socketSet.erase(iter);
						continue;
						break;
					case WSAENETDOWN: 
						cout << "connection ("<< inet_ntoa(iter->second.sin_addr) <<  ") has broken!\n";
						closesocket(sock);
						iter = sq->socketSet.erase(iter);
						continue;
						break;
					default:
						cout << "connection (" << inet_ntoa(iter->second.sin_addr) << ") unknown error\n"; 
						closesocket(sock);
						iter = sq->socketSet.erase(iter);
						continue; 
						break;
				}
			}
			else
			{
				cout << "client(" << inet_ntoa(iter->second.sin_addr) << "): " << buf << endl;
			}
			++iter;
		}
	}
}
