#pragma once

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <Commctrl.h>
#include <string.h>
#include <atomic>

#pragma region [Create Static]
#define SERVERIP			"127.0.0.1"
#define SERVERPORT			9000
#define BASE_BUF_SIZE       100000000
#define BIG_DATA_COUNT		20
#define SMALL_DATA_COUNT	10

struct ThreadStruct {
	//SOCKET		   sock;
	char fileName[256] = { 0 };
	//char fileName2[256] = { 0 };
	//char fileName3[256] = { 0 };
	//char fileName4[256] = { 0 };
	//char fileName5[256] = { 0 };
	//char fileName[5][256] = { 0 };
};

struct FileListStruct {
	//char fileName1[256] = { 0 };
	//char fileName2[256] = { 0 };
	//char fileName3[256] = { 0 };
	//char fileName4[256] = { 0 };
	//char fileName5[256] = { 0 };
	char fileName[5][256] = { 0 };

	LPCSTR PrintFileNames() {
		char* returnString{};

		for (int i = 0; i < 5; i++) {
			strcat(returnString, fileName[0]);
		}

		return returnString;
	}
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


//
//int main(int argc, char *argv[]) {
//	int retVal;
//
//#pragma region [// 윈속 초기화]
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		return 1;
//#pragma endregion
//
//#pragma region [ socket() ]
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET) err_quit("socket()");
//#pragma endregion
//
//#pragma region [ connect() ]
//	SOCKADDR_IN serverAddr;
//	ZeroMemory(&serverAddr, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
//	serverAddr.sin_port = htons(SERVERPORT);
//	retVal = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
//	if (retVal == SOCKET_ERROR) err_quit("bind()");
//#pragma endregion
//
//	FileListStruct fileList;
//	retVal = recv(sock, (char*)&fileList, sizeof(fileList), 0);
//
//	std::cout << "받고 싶은 파일을 선택하세요! " << std::endl;
//	std::cout << "1. " << fileList.fileName[0] << std::endl;
//	std::cout << "2. " << fileList.fileName[1] << std::endl;
//	std::cout << "3. " << fileList.fileName[2] << std::endl;
//	std::cout << "4. " << fileList.fileName[3] << std::endl;
//	std::cout << "5. " << fileList.fileName[4] << std::endl;
//
//	int inputFileNumber{};
//
//	rewind(stdin);
//	std::cin >> inputFileNumber;
//	rewind(stdin);
//
//	NeedDataStruct needData;
//	needData.type = inputFileNumber - 1;
//
//	retVal = send(sock, (char *)&needData, sizeof(needData), 0);
//
//	FixDataStruct fixData;
//	retVal = recv(sock, (char *)&(fixData), sizeof(fixData), 0);
//
//	long long int bufSize = fixData.size / BIG_DATA_COUNT;	
//	char* buf = new char[bufSize];
//	int count = fixData.size / bufSize;
//
//	FILE *fp = NULL;
//	fp = fopen(fileList.fileName[needData.type], "wb");
//
//	while (count) {
//		retVal = recvn(sock, buf, bufSize, 0);
//		//if (retVal == SOCKET_ERROR) {err_display("recv()");	break;}
//		fwrite(buf, 1, bufSize, fp);
//
//		count--;
//	}
//
//	count = fixData.size - (fixData.size / bufSize)* bufSize;
//
//	if (count) {
//		retVal = recvn(sock, buf, count, 0);
//		fwrite(buf, 1, count, fp);
//	}
//
//	fclose(fp);
//
//	std::cout << "전송을 끝냈습니다..." << std::endl;
//
//}
