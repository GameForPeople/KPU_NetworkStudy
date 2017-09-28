#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
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
	if ((in = fopen("MovieFile_3.mp4", "rb")) == NULL) {
		fputs("���� ���� ����!", stderr);
		exit(1);
	}

	fseek(in, 0, SEEK_END);

	// ���� ����, 1����Ʈ�� �о�, ���� ���
	//while ((ch = fgetc(in)) != EOF) {
	//	printf("%02X ", ch);
	//}
	unsigned long fileSize = ftell(in);
	
	char *buf = new char[fileSize];

	rewind(in);

	fread(buf, sizeof(char), fileSize, in);

	//while ((ch = fgetc(in)) != EOF) {
	//	cont.push_back(ch);
	//}
	
	//int* ptr = &(cont[0]);
		//memcpy(buf, ptr, 500000);

	// ������ ������ ���
		// ������ �Է�(�ùķ��̼�)
		len = fileSize;//strlen(cont);
		
		// ������ ������(���� ����)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
		}

		// ������ ������(���� ����)
		retval = send(sock, buf, len, 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
		}
		
		printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
			"���½��ϴ�.\n", sizeof(int), retval);
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();

	fclose(in); // ���� �ݱ�

	return 0;
}