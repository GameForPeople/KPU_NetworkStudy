#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#include <iostream>

int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) { 	//윈속 초기화
		MessageBox(NULL, "윈속 초기화 실패", "알림", MB_OK);
		return 1;
	}

	MessageBox(NULL, "윈속 초기화 성공", "따란~~~~", MB_OK);
	
#pragma region [IPv4 변환연습]

	//원래의 Ipv4 주소 출력
	char *ipv4test = "147.46.114.70";
	printf("IPv4 주소(변환전) = %s\n", ipv4test);

	//inet_addr() 함수연습
	printf("IPv4 주소(변환후) = 0x%x\n", inet_addr(ipv4test));

	//inet_ntoa() 함수연습
	IN_ADDR ipv4num;
	ipv4num.s_addr = inet_addr(ipv4test);
	printf("IPv4 주소(다시 변환 후) = %s\n", inet_ntoa(ipv4num));

	printf("\n");

#pragma endregion

#pragma region [IPv6 변환연습]

#pragma endregion






	WSACleanup();  //윈속 종료 1. 사용중지 2. 리소스 반환

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}