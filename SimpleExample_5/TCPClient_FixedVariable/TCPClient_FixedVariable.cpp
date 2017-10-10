//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

struct myDataStruct {
	unsigned int len;
	int type;
};

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
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
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
	int retval;
	myDataStruct myData;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	char addrStr[100];
	char fileStr[100];

	std::cout << "---TCP/IP 파일 전송 프로그램---" << std::endl;
	printf(" IP를입력하세요 : ");
	fgets(addrStr, sizeof(addrStr), stdin);
	addrStr[strlen(addrStr) - 1] = '\0';

	printf(" 파일 이름을 제발 정확히 입력하세요 (ex : MovieFile_5.mp4, ImageFile_1.png, TextFile_1.txt ) : ");
	fgets(fileStr, sizeof(fileStr), stdin);
	fileStr[strlen(fileStr) - 1] = '\0';

	printf(" 파일 종류를 입력해주세요. \n 1 --> mp4, 2 --> png, 3 --> txt  (서버 소켓 상태 listen 여부 확인) : ");
	std::cin >> myData.type;

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(addrStr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	int len;

	// 보낼 데이터 만들기
	FILE *in;
	int ch;
	std::vector<int> cont;

	// test.bin 이라는 이진파일을 읽기 전용으로 열기
	if ((in = fopen(fileStr, "rb")) == NULL) {
		fputs("파일 열기 에러!", stderr);
		exit(1);
	}

	fseek(in, 0, SEEK_END);

	unsigned long fileSize = ftell(in);
	
	char *buf = new char[fileSize];

	rewind(in);

	fread(buf, sizeof(char), fileSize, in);

	len = fileSize;//strlen(cont);
	myData.len = fileSize;

	// 데이터 보내기(고정 길이)
	retval = send(sock, (char *)&(myData), sizeof(myDataStruct), 0);
	if(retval == SOCKET_ERROR){
			err_display("send()");
		}

	// 데이터 보내기(가변 길이)
	retval = send(sock, buf, len, 0);
	if(retval == SOCKET_ERROR){
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