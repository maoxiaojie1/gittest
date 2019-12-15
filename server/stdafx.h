// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <string>
constexpr auto MAX_LISTEN_NUM = 5;
typedef struct T_MsgData
{
	SOCKET sock;
	SOCKADDR_IN addr;
	std::string str;
}T_MsgData;

typedef struct T_SocketInfo
{
	SOCKET first;
	SOCKADDR_IN second;
}T_SocketInfo;


// TODO: 在此处引用程序需要的其他头文件
