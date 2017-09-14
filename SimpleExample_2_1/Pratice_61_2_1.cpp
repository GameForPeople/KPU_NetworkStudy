#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// Page61 1번
// wsa변수이 필드 중 처음 네개를 화면에 출력하시오.
// 윈속 1.1 버전을 사용하도록 예제를 수정할 경우 이 값들이 어떻게 바뀌는가!

//void err_quit(char *msg) {
//LPVOID lpMsgBuf;
//FormatMessage(
//FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	//dwFlags(옵션), 1.오류 메세지를 저장할 공간을 알아서 할당, 2. 운영체제로부터 오류 메세지를 가져와서 할당받은 메모리에 넣어라
//NULL,															//lpsource -> NULL인 이유는 운영체제에게 시켰기 떄문에
//WSAGetLastError(),												//dwMessageId -> 오류 코드를 인자로 전달함
//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),						//dwLanguageID -> 언어 -> 다음과 같은 설정으로 설정하면 해당 운영체제 디폴트 언어로 출력됨! -> 공용성!, 코딩은 개발자 를 위한게 아냐
//(LPTSTR)&lpMsgBuf,												//lpBuffer -> 오류 문자열 시작 주소
//0,																// 사이즈 무시!
//NULL															// 아규먼트 무시!
//);
//
//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);				//메세지박스 함수로 에러코드 문자열 출력!
//
//LocalFree(lpMsgBuf);
//exit(1);
//}


double ConvertWORDToVersion(WORD WORDVal) {
	double hiVal = 0, lowVal = 0, lowValPos = 10;

	for (int i = 7; i >= 0; --i)
	{
		if ((WORDVal >> i) & 1) {
			hiVal += pow(2.0, (double)i);
		}
	}

	for (int i = 15; i >= 8; --i) {
		if ((WORDVal >> i) & 1) {
			lowVal += pow(2.0, (double)(i - 8));
		}
	}

	if (lowVal) {
		for (double posBuffer = lowVal; posBuffer >= 10; posBuffer /= 10) {
			lowValPos *= 10;
		}
	}
	//std::cout << hiVal << "   " << lowVal << std::endl;
	return hiVal + (lowVal / lowValPos);
}


int main(int argc, char *argv[])
{

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { 	//윈속 초기화
		MessageBox(NULL, "윈속 초기화 실패", "알림", MB_OK);
		return 1;
	}

	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	std::cout<< ConvertWORDToVersion(MAKEWORD(3, 2)) <<std::endl;
	std::cout << ConvertWORDToVersion(0x0203) << std::endl;
		
	//--
	std::cout << "   현재버전(wsa.wVersion) : " << ConvertWORDToVersion(wsa.wVersion) << std::endl <<
		"   가용가능최고버전(wsa.wHighVersion) : " << ConvertWORDToVersion(wsa.wHighVersion) << std::endl <<
		"   wsa.szDescription : " << wsa.szDescription << std::endl <<
		"   wsa.szSystemStatus : " << wsa.szSystemStatus << std::endl;
	
	//szDescription
	//Windows 소켓 DLL이 공급 업체 ID를 포함하여 Windows 소켓 구현에 대한 설명을 복사하는 Null 종료 ASCII 문자열입니다. 
	//텍스트 (최대 256 자)는 모든 문자를 포함 할 수 있지만 공급 업체는 제어 문자 및 서식 지정 문자에주의해야합니다.
	
	//szSystemStatus
	//Windows 소켓 DLL이 관련 상태 또는 구성 정보를 복사하는 Null 종료 ASCII 문자열입니다. 
	//Windows 소켓 DLL은 정보가 사용자 또는 지원 담당자에게 유용 할 경우에만이 필드를 사용해야합니다. 
	//szDescription 필드의 확장으로 간주해서는 안됩니다.
	
	
	WSACleanup();  //윈속 종료 1. 사용중지 2. 리소스 반환

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}