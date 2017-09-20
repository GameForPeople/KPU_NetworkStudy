#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

// ����Ʈ ����
BOOL IsLittleEndian() {
	u_short inputVal = 0x1234;
	u_short resultVal;

	resultVal = htons(inputVal);
	printf("0x%x -> 0x%x \n", inputVal, resultVal);

	if (inputVal == resultVal) {
			std::cout << "�� ȣ��Ʈ�� ��Ʋ ������� �ƴմϴ�." << std::endl;
			return false;
	}
	else {//if (inputVal == ntohs(resultVal)) {
		std::cout << "�� ȣ��Ʈ�� ��Ʋ ������Դϴ�." << std::endl;
		return true;
	}
}

BOOL IsBigEndian() {
	u_short inputVal = 0x1234;
	u_short resultVal;

	resultVal = htons(inputVal);
	//printf("0x%x -> 0x%x \n", inputVal, resultVal);

	if (inputVal == resultVal) {
		std::cout << "�� ȣ��Ʈ�� �� ������Դϴ�." << std::endl;
		return true;
	}
	else {//if (inputVal == ntohs(resultVal)) {
		std::cout << "�� ȣ��Ʈ�� �� ������� �ƴմϴ�." << std::endl;
		return false;
	}
}


int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0) { 	//���� �ʱ�ȭ
		MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
		return 1;
	}

	MessageBox(NULL, "���� �ʱ�ȭ ����", "����~~~~", MB_OK);

	IsLittleEndian();
	IsBigEndian();

	WSACleanup();  //���� ���� 1. ������� 2. ���ҽ� ��ȯ

	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
}


//u_short x1 = 0x1234;
//u_long y1 = 0x12345678;
//u_short x2;
//u_long y2;
//
//printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
//printf("0x%x -> 0x%x \n", x1, x2 = htons(x1));
//printf("0x%x -> 0x%x \n", y1, y2 = htonl(y1));
//
//printf("[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
//printf("0x%x -> 0x%x \n", x2, ntohs(x2));
//printf("0x%x -> 0x%x \n", y2, ntohl(y2));
//
//// �߸��� ��� ��
//printf("\n AnsWer Point \n");