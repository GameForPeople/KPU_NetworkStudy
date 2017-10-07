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

#define SCREEN_WIDTH		1280
#define SCREEN_HEIGHT		720

#define SERVERPORT			9000

#define MAX_POWER_MAN		999999999
#define BUF_SIZE			100000000

//std::atomic<int>			distanceUI = 0;
volatile int				distanceUI = 0;

std::mutex					mylock;

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

// 사용자 정의 데이터 수신 함수
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

		std::cout << "전송받은 데이터의 길이는 : " << len << std::endl;
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

void Recv(int &retval, SOCKET& listen_sock, SOCKET& client_sock, SOCKADDR_IN& clientaddr, char *buf, int& len, int& addrlen) {
	while (1) {
		int count;
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기(고정된! 즉 약속된 길이!)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			count = len / BUF_SIZE;

			int partDistance = 860 / count;

			FILE *fp = NULL;

			fp = fopen("new3.mp4", "wb");

			// 데이터 받기(가변 길이)
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
				std::cout << distanceUI << " " << partDistance << std::endl;
				mylock.unlock();

				count--;
			}

			count = len - (len / BUF_SIZE)* BUF_SIZE;

			retval = recvn(client_sock, buf, BUF_SIZE, 0, 1);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}

			mylock.lock();
			distanceUI = 860;
			mylock.unlock();

			fwrite(buf, 1, count, fp);
			fclose(fp);
			// 받은 데이터 출력
			buf[retval] = '\0';
			//printf("[TCP/%s:%d] %02x\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			//ofstream으로 한번에 많은 파일 바이너리 복호화?? 힘듬!!! 하아...
			//std::ofstream outFile("new2.mp4", std::ofstream::binary);
			//outFile.write(buf, len);
		}
	}
}
