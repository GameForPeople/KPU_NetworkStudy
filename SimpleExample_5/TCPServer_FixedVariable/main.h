#pragma once

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <atlimage.h>
#include <thread>
#include <atomic>
#include <mutex>

#pragma region [Create Static]
#define	SCREEN_WIDTH		1280
#define	SCREEN_HEIGHT		720

#define	SERVERPORT			9000

#define	MAX_POWER_MAN		999999999
#define BASE_BUF_SIZE       100000000

#define BIG_DATA_COUNT		20
#define SMALL_DATA_COUNT	10

#define OVERALL_LENGTH		860

int							BUF_SIZE = BASE_BUF_SIZE;

std::atomic<int>			typeUI = 0;
//std::atomic<int>			distanceUI = 0;
volatile int				distanceUI = 0;

std::mutex					mylock;

struct myDataStruct {
	long long len;
	int type;
	char name[256] = { 0 };
};

myDataStruct myData;
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

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags, int funcFlag)
{
	int received;
	char *ptr = buf;
	int left = len;

	if (funcFlag == 0) {
		while (left > 0) {
			received = recv(s, ptr, left, flags);
			if (received == SOCKET_ERROR)
				return SOCKET_ERROR;
			else if (received == 0)
				break;
			left -= received;
			ptr += received;
		}
	}
	else if (funcFlag == 1) {
		while (left > 0) {

			received = recv(s, ptr, left, flags);
			if (received == SOCKET_ERROR)
				return SOCKET_ERROR;
			else if (received == 0)
				break;
			left -= received;
			ptr += received;
		}

	}

	return (len - left);
}

void Socket(SOCKET& listen_sock) {
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) { err_quit("socket()"); }
}

void Bind(SOCKADDR_IN& serveraddr, SOCKET& listen_sock, int& retval) {
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
}

void Listen(int &retval, SOCKET& listen_sock) {
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

void Recv(int &retval, SOCKET& listen_sock, SOCKET& client_sock, SOCKADDR_IN& clientaddr, int& len, int& addrlen) {
		int count;
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			return;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ���� ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// ������ �ޱ�(������! �� ��ӵ� ����!)
			retval = recvn(client_sock, (char *)&myData, sizeof(myDataStruct), 0, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			FILE *fp = NULL;

			if (myData.type == 1) {
				BUF_SIZE = myData.len / BIG_DATA_COUNT;
			}
			else if (myData.type != 1) {
				BUF_SIZE = myData.len / SMALL_DATA_COUNT;
			}

			typeUI = myData.type;
			
			std::cout << myData.name << "�� �޴� ���Դϴ�. " << std::endl;
			fp = fopen(myData.name, "wb");
			
			char* buf = new char[BUF_SIZE];

			count = myData.len / BUF_SIZE;
			
			int partDistance = OVERALL_LENGTH / count;

			// ������ �ޱ�(���� ����)
			while (count) {
				retval = recvn(client_sock, buf, BUF_SIZE, 0, 1);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				else if (retval == 0)
					break;

				fwrite(buf, 1, BUF_SIZE, fp);

				mylock.lock();
				distanceUI += partDistance;
				mylock.unlock();

				count--;
			}

			count = myData.len - (myData.len / BUF_SIZE)* BUF_SIZE;

			std::cout << "���� �޸� ��" << count << std::endl;
			if (count) {
				retval = recvn(client_sock, buf, count, 0, 1);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
			}

			mylock.lock();
			distanceUI = OVERALL_LENGTH;
			mylock.unlock();

			fwrite(buf, 1, count, fp);
			fclose(fp);
			// ���� ������ ���
			//buf[retval] = '\0';
			//printf("[TCP/%s:%d] %02x\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
			std::cout << "������ ���½��ϴ�..." << std::endl;
			//ofstream���� �ѹ��� ���� ���� ���̳ʸ� ��ȣȭ?? ����!!! �Ͼ�...
			//std::ofstream outFile("new2.mp4", std::ofstream::binary);
			//outFile.write(buf, len);
		}
}
