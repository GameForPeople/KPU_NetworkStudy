
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>

#pragma region [Create Static]
#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BASE_BUF_SIZE       100000000

struct myDataStruct {
	long long len;
	int type;
	char name[256] = { 0 };
};
#pragma endregion

#pragma region [Error Function]
// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

#pragma region [User Interface]
	char addrStr[100];
	char fileStr[256];

	std::cout << "---TCP/IP ���� ���� ���α׷�---" << std::endl;
	printf(" IP���Է��ϼ��� : ");
	fgets(addrStr, sizeof(addrStr), stdin);
	addrStr[strlen(addrStr) - 1] = '\0';

	printf(" ���� �̸��� ���� ��Ȯ�� �Է��ϼ��� (ex : MovieFile_5.mp4, ImageFile_1.png, TextFile_1.txt ) : ");
	fgets(fileStr, sizeof(fileStr), stdin);
	fileStr[strlen(fileStr) - 1] = '\0';

	printf(" ���� ������ �Է����ּ���. \n 1 --> mp4, 2 --> png, 3 --> txt  (���� ���� ���� listen ���� Ȯ��) : ");
	std::cin >> myData.type;

#pragma endregion

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
	unsigned long long fileSize;

	// test.bin �̶�� ���������� �б� �������� ����
	if ((in = fopen(fileStr, "rb")) == NULL) {
		fputs("���� ���� ����!", stderr);
		exit(1);
	}

	fseek(in, 0, SEEK_END);
	fileSize = _ftelli64(in);
	fseek(in, 0, SEEK_SET);
	//rewind(in);
	std::cout << fileStr << std::endl;
	memcpy(myData.name, fileStr, strlen(fileStr - 1));
	std::cout << myData.name << std::endl;

	if (myData.type == 1) {
		len = fileSize / 20;//strlen(cont);
	}
	else if (myData.type != 1) {
		len = fileSize / 10;//strlen(cont);
	}

	char *buf = new char[len];

	myData.len = fileSize;

	// ������ ������(���� ����)
	retval = send(sock, (char *)&(myData), sizeof(myDataStruct), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	if (myData.type == 1) {
	// ������ ������(���� ����)
	for (int i = 0; i < 20; i++) {
		fread(buf, sizeof(char), len, in);
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
		}
	if(myData.len - myData.len / 20 * 20)
		retval = send(sock, buf, myData.len - myData.len / 20 * 20, 0);
	}
	else if (myData.type != 1) {
		for (int i = 0; i < 10; i++) {
			fread(buf, sizeof(char), len, in);
			retval = send(sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
		if(myData.len - myData.len / 20 * 20)
			retval = send(sock, buf, myData.len - myData.len / 10 * 10, 0);
	}
	if (retval == SOCKET_ERROR) {
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