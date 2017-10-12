
#pragma comment(lib, "ws2_32")
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

struct myDataStruct {
	long long len;
	int type;
	char name[256] = { 0 };
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

int main(int argc, char *argv[])
{
	myDataStruct myData;
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	#pragma region [ socket() ]
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	#pragma endregion

	#pragma region [User Interface]
	char addrStr[100];
	char fileStr[256];

	std::cout << "---TCP/IP 파일 전송 프로그램---" << std::endl;
	printf(" IP를입력하세요 : ");
	fgets(addrStr, sizeof(addrStr), stdin);
	addrStr[strlen(addrStr) - 1] = '\0';

	printf(" 파일 이름을 제발 정확히 입력하세요 (ex : MovieFile_5.mp4, ImageFile_1.png, TextFile_1.txt ) : ");
	fgets(fileStr, sizeof(fileStr), stdin);
	fileStr[strlen(fileStr) - 1] = '\0';

	printf(" 파일 종류를 입력해주세요. \n 1 --> 동영상, 2 --> 이미지, 3 --> 텍스트  (입력 전, listen 여부 확인) : ");
	std::cin >> myData.type;

#pragma endregion
	
	#pragma region [ connect() ]

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(addrStr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	#pragma endregion

	// 데이터 통신에 사용할 변수
	int len;

	// 보낼 데이터 만들기
	FILE *in;
	int ch;
	std::vector<int> cont;
	unsigned long long fileSize;

	if ((in = fopen(fileStr, "rb")) == NULL) {
		fputs("파일 열기 에러!", stderr);
		exit(1);
	}

	fseek(in, 0, SEEK_END);
	fileSize = _ftelli64(in);
	fseek(in, 0, SEEK_SET);
	//rewind(in);
	memcpy(myData.name, fileStr, strlen(fileStr - 1));
	std::cout << myData.name << "를 전송중입니다. " << std::endl;

	if (myData.type == 1) {
		len = fileSize / BIG_DATA_COUNT;//strlen(cont);
	}
	else if (myData.type != 1) {
		len = fileSize / SMALL_DATA_COUNT;//strlen(cont);
	}

	char *buf = new char[len];

	myData.len = fileSize;

	// 데이터 보내기(고정 길이)
	retval = send(sock, (char *)&(myData), sizeof(myDataStruct), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	if (myData.type == 1) {
	// 데이터 보내기(가변 길이)
	for (int i = 0; i < BIG_DATA_COUNT; i++) {
		fread(buf, sizeof(char), len, in);
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
		}
	if(myData.len - myData.len / BIG_DATA_COUNT * BIG_DATA_COUNT)
		retval = send(sock, buf, myData.len - myData.len / BIG_DATA_COUNT * BIG_DATA_COUNT, 0);
	}
	else if (myData.type != 1) {
		for (int i = 0; i < SMALL_DATA_COUNT; i++) {
			fread(buf, sizeof(char), len, in);
			retval = send(sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
		if(myData.len - myData.len / SMALL_DATA_COUNT * SMALL_DATA_COUNT)
			retval = send(sock, buf, myData.len - myData.len / SMALL_DATA_COUNT * SMALL_DATA_COUNT, 0);
	}
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	printf("[TCP 클라이언트] %d + %d바이트를 "
		"보냈습니다.\n", sizeof(int), retval);
	
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();

	fclose(in); // 파일 닫기

	return 0;
}