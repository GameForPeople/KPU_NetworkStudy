#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#include <iostream>

int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) { 	//���� �ʱ�ȭ
		MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
		return 1;
	}

	MessageBox(NULL, "���� �ʱ�ȭ ����", "����~~~~", MB_OK);
	
#pragma region [IPv4 ��ȯ����]

	//������ Ipv4 �ּ� ���
	char *ipv4test = "147.46.114.70";
	printf("IPv4 �ּ�(��ȯ��) = %s\n", ipv4test);

	//inet_addr() �Լ�����
	printf("IPv4 �ּ�(��ȯ��) = 0x%x\n", inet_addr(ipv4test));

	//inet_ntoa() �Լ�����
	IN_ADDR ipv4num;
	ipv4num.s_addr = inet_addr(ipv4test);
	printf("IPv4 �ּ�(�ٽ� ��ȯ ��) = %s\n", inet_ntoa(ipv4num));

	printf("\n");

#pragma endregion

#pragma region [IPv6 ��ȯ����]

#pragma endregion






	WSACleanup();  //���� ���� 1. ������� 2. ���ҽ� ��ȯ

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}