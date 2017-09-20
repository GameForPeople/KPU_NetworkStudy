#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// 바이트 정렬

int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) { 	//윈속 초기화
		MessageBox(NULL, "윈속 초기화 실패", "알림", MB_OK);
		return 1;
	}

	MessageBox(NULL, "윈속 초기화 성공", "따란~~~~", MB_OK);

	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;
	u_short x2;
	u_long y2;

	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	printf("0x%x -> 0x%x \n", x1, x2 = htons(x1));
	printf("0x%x -> 0x%x \n", y1, y2 = htons(y1));

	printf("[네트워크 바이트 -> 호스트 바이트]\n");
	printf("0x%x -> 0x%x \n", x1, ntohs(x1));
	printf("0x%x -> 0x%x \n", y1, ntohs(y1));

	// 잘못된 사용 예
	printf("\n [잘못된 사용 예]\n");
	printf("0x%x -> 0x%x \n", x1, htonl(x1));

	WSACleanup();  //윈속 종료 1. 사용중지 2. 리소스 반환

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}