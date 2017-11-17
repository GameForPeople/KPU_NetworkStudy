#pragma once

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>

#pragma region [Create Static]
#define SERVERIP			"127.0.0.1"
#define SERVERPORT			9000
#define BASE_BUF_SIZE       100000000
#define BIG_DATA_COUNT		20
#define SMALL_DATA_COUNT	10

struct ThreadStruct {
	SOCKET		   sock;
	//char fileName1[256] = { 0 };
	//char fileName2[256] = { 0 };
	//char fileName3[256] = { 0 };
	//char fileName4[256] = { 0 };
	//char fileName5[256] = { 0 };
	char fileName[5][256] = { 0 };
};

struct FileListStruct {
	//char fileName1[256] = { 0 };
	//char fileName2[256] = { 0 };
	//char fileName3[256] = { 0 };
	//char fileName4[256] = { 0 };
	//char fileName5[256] = { 0 };
	char fileName[5][256] = { 0 };
};

struct NeedDataStruct {
	int type;
};

struct FixDataStruct {
	long long int size;
};

#pragma endregion

#pragma region [Error Function]
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
#pragma endregion 

