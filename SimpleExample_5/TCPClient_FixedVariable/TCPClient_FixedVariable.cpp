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

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	char addrStr[100];
	char fileStr[100];

	std::cout << "---TCP/IP ���� ���� ���α׷�---" << std::endl;
	printf(" IP���Է��ϼ��� : ");
	fgets(addrStr, sizeof(addrStr), stdin);
	addrStr[strlen(addrStr) - 1] = '\0';

	printf(" ���� �̸��� ���� ��Ȯ�� �Է��ϼ��� (ex : MovieFile_5.mp4, ImageFile_1.png, TextFile_1.txt ) : ");
	fgets(fileStr, sizeof(fileStr), stdin);
	fileStr[strlen(fileStr) - 1] = '\0';

	printf(" ���� ������ �Է����ּ���. \n 1 --> mp4, 2 --> png, 3 --> txt  (���� ���� ���� listen ���� Ȯ��) : ");
	std::cin >> myData.type;

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(addrStr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	int len;

	// ���� ������ �����
	FILE *in;
	int ch;
	std::vector<int> cont;

	// test.bin �̶�� ���������� �б� �������� ����
	if ((in = fopen(fileStr, "rb")) == NULL) {
		fputs("���� ���� ����!", stderr);
		exit(1);
	}

	fseek(in, 0, SEEK_END);

	unsigned long fileSize = ftell(in);
	
	char *buf = new char[fileSize];

	rewind(in);

	fread(buf, sizeof(char), fileSize, in);

	len = fileSize;//strlen(cont);
	myData.len = fileSize;

	// ������ ������(���� ����)
	retval = send(sock, (char *)&(myData), sizeof(myDataStruct), 0);
	if(retval == SOCKET_ERROR){
			err_display("send()");
		}

	// ������ ������(���� ����)
	retval = send(sock, buf, len, 0);
	if(retval == SOCKET_ERROR){
			err_display("send()");
		}
	
	printf("[TCP Ŭ���̾�Ʈ] %d + %d����Ʈ�� "
		"���½��ϴ�.\n", sizeof(int), retval);
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();

	fclose(in); // ���� �ݱ�

	return 0;
}