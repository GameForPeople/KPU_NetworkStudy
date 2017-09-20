#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// Page61 1��
// wsa������ �ʵ� �� ó�� �װ��� ȭ�鿡 ����Ͻÿ�.
// ���� 1.1 ������ ����ϵ��� ������ ������ ��� �� ������ ��� �ٲ�°�!

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

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { 	//���� �ʱ�ȭ
		MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
		return 1;
	}
	
	MessageBox(NULL, "���� �ʱ�ȭ ����", "����~~~~", MB_OK);

	//to Debug
	//std::cout<< ConvertWORDToVersion(MAKEWORD(3, 2)) <<std::endl;
	//std::cout << ConvertWORDToVersion(0x0203) << std::endl;
		
	//--
	std::cout << "   �������(wsa.wVersion) : " << ConvertWORDToVersion(wsa.wVersion) << std::endl <<
		"   ���밡���ְ����(wsa.wHighVersion) : " << ConvertWORDToVersion(wsa.wHighVersion) << std::endl <<
		"   wsa.szDescription : " << wsa.szDescription << std::endl <<
		"   wsa.szSystemStatus : " << wsa.szSystemStatus << std::endl;
	
	//szDescription
	//Windows ���� DLL�� ���� ��ü ID�� �����Ͽ� Windows ���� ������ ���� ������ �����ϴ� Null ���� ASCII ���ڿ��Դϴ�. 
	//�ؽ�Ʈ (�ִ� 256 ��)�� ��� ���ڸ� ���� �� �� ������ ���� ��ü�� ���� ���� �� ���� ���� ���ڿ������ؾ��մϴ�.
	
	//szSystemStatus
	//Windows ���� DLL�� ���� ���� �Ǵ� ���� ������ �����ϴ� Null ���� ASCII ���ڿ��Դϴ�. 
	//Windows ���� DLL�� ������ ����� �Ǵ� ���� ����ڿ��� ���� �� ��쿡���� �ʵ带 ����ؾ��մϴ�. 
	//szDescription �ʵ��� Ȯ������ �����ؼ��� �ȵ˴ϴ�.
	
	
	WSACleanup();  //���� ���� 1. ������� 2. ���ҽ� ��ȯ

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}