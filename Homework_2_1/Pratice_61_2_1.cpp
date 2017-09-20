#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// Page61 1번
// wsa변수이 필드 중 처음 네개를 화면에 출력하시오.
// 윈속 1.1 버전을 사용하도록 예제를 수정할 경우 이 값들이 어떻게 바뀌는가!

double ConvertWORDToVersion(WORD WORDVal) {
	double hiVer = 0, lowVer = 0, lowValPos = 10;

	for (int i = 7; i >= 0; --i)
	{
		if ((WORDVal >> i) & 1) {
			hiVer += pow(2.0, (double)i);
		}
	}

	for (int i = 15; i >= 8; --i) {
		if ((WORDVal >> i) & 1 ) {
			lowVer += pow(2.0, (double)(i - 8));
		}
	}

	if (lowVer) {
		for (double posBuffer = lowVer; posBuffer >= 10; posBuffer /= 10) {
			lowValPos *= 10;
		}
	}
	
	std::cout << hiVer << "   " << lowVer << std::endl;
	return hiVer + (lowVer / lowValPos);
}


int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { 	//윈속 초기화
		MessageBox(NULL, "윈속 초기화 실패", "알림", MB_OK);
		return 1;
	}
	
	MessageBox(NULL, "윈속 초기화 성공", "따란~~~~", MB_OK);

	//to Debug
	//std::cout<< ConvertWORDToVersion(MAKEWORD(3, 2)) <<std::endl;
	//std::cout << ConvertWORDToVersion(0x0203) << std::endl;
		
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