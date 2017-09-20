



#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

//1. 오류 처리
//int WSAGetLastError(void); 함수를 사용해 소켓 함수 호출 결과 오류 코드를 얻을 수 있다.
//리턴 값만으로 오류를 처리하는 경우 : WSAStartup()함수를 사용

void err_quit(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	//dwFlags(옵션), 1.오류 메세지를 저장할 공간을 알아서 할당, 2. 운영체제로부터 오류 메세지를 가져와서 할당받은 메모리에 넣어라
		NULL,															//lpsource -> NULL인 이유는 운영체제에게 시켰기 떄문에
		WSAGetLastError(),												//dwMessageId -> 오류 코드를 인자로 전달함
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),						//dwLanguageID -> 언어 -> 다음과 같은 설정으로 설정하면 해당 운영체제 디폴트 언어로 출력됨! -> 공용성!, 코딩은 개발자 를 위한게 아냐
		(LPTSTR)&lpMsgBuf,												//lpBuffer -> 오류 문자열 시작 주소
		0,																// 사이즈 무시!
		NULL															// 아규먼트 무시!
	);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);				//메세지박스 함수로 에러코드 문자열 출력!

	LocalFree(lpMsgBuf);
	exit(1);
}


int main(int argc, char *argv[])
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	//socker()
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP 소캣 생성 성공", "알림", MB_OK);

	//closesocket()
	closesocket(tcp_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}