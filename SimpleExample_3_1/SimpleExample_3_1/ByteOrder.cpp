#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// ����Ʈ ����

int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) { 	//���� �ʱ�ȭ
		MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
		return 1;
	}

	MessageBox(NULL, "���� �ʱ�ȭ ����", "����~~~~", MB_OK);

	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;
	u_short x2;
	u_long y2;

	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
	printf("0x%x -> 0x%x \n", x1, x2 = htons(x1));
	printf("0x%x -> 0x%x \n", y1, y2 = htons(y1));

	printf("[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	printf("0x%x -> 0x%x \n", x1, ntohs(x1));
	printf("0x%x -> 0x%x \n", y1, ntohs(y1));

	// �߸��� ��� ��
	printf("\n [�߸��� ��� ��]\n");
	printf("0x%x -> 0x%x \n", x1, htonl(x1));

	WSACleanup();  //���� ���� 1. ������� 2. ���ҽ� ��ȯ

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}