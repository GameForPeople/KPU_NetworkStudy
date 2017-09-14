#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// Page61 1��
// wsa������ �ʵ� �� ó�� �װ��� ȭ�鿡 ����Ͻÿ�.
// ���� 1.1 ������ ����ϵ��� ������ ������ ��� �� ������ ��� �ٲ�°�!

//void err_quit(char *msg) {
//LPVOID lpMsgBuf;
//FormatMessage(
//FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	//dwFlags(�ɼ�), 1.���� �޼����� ������ ������ �˾Ƽ� �Ҵ�, 2. �ü���κ��� ���� �޼����� �����ͼ� �Ҵ���� �޸𸮿� �־��
//NULL,															//lpsource -> NULL�� ������ �ü������ ���ױ� ������
//WSAGetLastError(),												//dwMessageId -> ���� �ڵ带 ���ڷ� ������
//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),						//dwLanguageID -> ��� -> ������ ���� �������� �����ϸ� �ش� �ü�� ����Ʈ ���� ��µ�! -> ���뼺!, �ڵ��� ������ �� ���Ѱ� �Ƴ�
//(LPTSTR)&lpMsgBuf,												//lpBuffer -> ���� ���ڿ� ���� �ּ�
//0,																// ������ ����!
//NULL															// �ƱԸ�Ʈ ����!
//);
//
//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);				//�޼����ڽ� �Լ��� �����ڵ� ���ڿ� ���!
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

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { 	//���� �ʱ�ȭ
		MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
		return 1;
	}

	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	std::cout<< ConvertWORDToVersion(MAKEWORD(3, 2)) <<std::endl;
	std::cout << ConvertWORDToVersion(0x0203) << std::endl;
		
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