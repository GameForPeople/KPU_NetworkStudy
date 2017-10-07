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

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SERVERPORT 9000

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

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

//	if (flags == 0) {
		while (left > 0) {
			received = recv(s, ptr, left, flags);
			if (received == SOCKET_ERROR)
				return SOCKET_ERROR;
			else if (received == 0)
				break;
			left -= received;
			ptr += received;
		}
//	}

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

void Recv(int &retval, SOCKET& listen_sock, SOCKET& client_sock, SOCKADDR_IN& clientaddr, char *buf, int& len, int& addrlen) {
	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			std::cout << "���̴� : " << len << std::endl;
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %02x\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			std::ofstream outFile("new2.mp4", std::ofstream::binary);
			outFile.write(buf, len);
		}
	}
}