#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// 바이트 정렬
BOOL IsLittleEndian() {
	u_short inputVal = 0x1234;
	u_short resultVal;

	resultVal = htons(inputVal);
	printf("0x%x -> 0x%x \n", inputVal, resultVal);

	if (inputVal == resultVal) {
			std::cout << "이 호스트는 리틀 엔디안이 아닙니다." << std::endl;
			return false;
	}
	else {//if (inputVal == ntohs(resultVal)) {
		std::cout << "이 호스트는 리틀 엔디안입니다." << std::endl;
		return true;
	}
}

BOOL IsBigEndian() {
	u_short inputVal = 0x1234;
	u_short resultVal;

	resultVal = htons(inputVal);
	//printf("0x%x -> 0x%x \n", inputVal, resultVal);

	if (inputVal == resultVal) {
		std::cout << "이 호스트는 빅 엔디안입니다." << std::endl;
		return true;
	}
	else {//if (inputVal == ntohs(resultVal)) {
		std::cout << "이 호스트는 빅 엔디안이 아닙니다." << std::endl;
		return false;
	}
}


int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) { 	//윈속 초기화
		MessageBox(NULL, "윈속 초기화 실패", "알림", MB_OK);
		return 1;
	}

	MessageBox(NULL, "윈속 초기화 성공", "따란~~~~", MB_OK);

	IsLittleEndian();
	IsBigEndian();

	WSACleanup();  //윈속 종료 1. 사용중지 2. 리소스 반환

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}


//u_short x1 = 0x1234;
//u_long y1 = 0x12345678;
//u_short x2;
//u_long y2;
//
//printf("[호스트 바이트 -> 네트워크 바이트]\n");
//printf("0x%x -> 0x%x \n", x1, x2 = htons(x1));
//printf("0x%x -> 0x%x \n", y1, y2 = htonl(y1));
//
//printf("[네트워크 바이트 -> 호스트 바이트]\n");
//printf("0x%x -> 0x%x \n", x2, ntohs(x2));
//printf("0x%x -> 0x%x \n", y2, ntohl(y2));
//
//// 잘못된 사용 예
//printf("\n AnsWer Point \n");