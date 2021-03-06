// server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "SocketQueue.h"
#include <WinSock2.h>
#include <windows.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
/*
DWORD WINAPI sendData(LPARAM *lparam)
{
	SOCKET *clientSocket = (SOCKET*)lparam;
	string sendData;
	const char *str;
	int ret;
	while(true)
	{
		getline(cin, sendData);
		str = sendData.c_str();
		ret = send(*clientSocket, str, strlen(str) + 1, 0);
		//cout << sizeof(*str) << endl;
		if (ret == SOCKET_ERROR)
		{
			cout << "发送错误！" << endl;
			//return 1;
		}
	}
	//closesocket(clientSocket);
	return 0;
}
DWORD WINAPI recvData(LPARAM *lparam)
{
	SOCKET *clientSocket = (SOCKET*)lparam;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	while (true)
	{
		int ret = recv(*clientSocket, buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR)
		{
			cout << "接收错误！" << endl;
			//closesocket(clientSocket);
			//return 1;
			Sleep(5000);
		}
		else
			cout << "client：" << buf << endl;
	}

	//closesocket(clientSocket);
	return 0;
}

int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		cout << "socket error !" << endl;
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据
	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	printf("等待连接...\n");
	bool flag = 0;
	while (1)
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		if (!flag)
		{
			HANDLE th1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvData, (LPVOID*)&sClient, 0, 0);
			HANDLE th2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendData, (LPVOID*)&sClient, 0, 0);
			flag = 1;
		}
	}
	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}*/

int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		cout << "socket error !" << endl;
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	SocketQueue sockQueue(slisten, sin);
	sockQueue.initSocket();
	sockQueue.start();
	return 0;
}