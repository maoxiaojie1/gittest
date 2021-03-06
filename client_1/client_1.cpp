// client_1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "socket error !" << endl;
		return 0;
	}

	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(8888);
	cout << "connecting..." << endl;
	if (connect(clientSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		cout << "connect error!" << endl;
		return 0;
	}
	char *buff = new char[MAX_MSG_LEN];
	memset(buff, 0, sizeof(char)*MAX_MSG_LEN);
	int msgLen = 0;
	while (true)
	{
		msgLen = recv(clientSocket, buff, sizeof(char)*MAX_MSG_LEN, 0);
		if (msgLen != 0)
		{
			cout << "kk: " << buff << endl;
		}
	}

    return 0;
}

