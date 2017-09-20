



#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>

//1. ���� ó��
//int WSAGetLastError(void); �Լ��� ����� ���� �Լ� ȣ�� ��� ���� �ڵ带 ���� �� �ִ�.
//���� �������� ������ ó���ϴ� ��� : WSAStartup()�Լ��� ���

void err_quit(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	//dwFlags(�ɼ�), 1.���� �޼����� ������ ������ �˾Ƽ� �Ҵ�, 2. �ü���κ��� ���� �޼����� �����ͼ� �Ҵ���� �޸𸮿� �־��
		NULL,															//lpsource -> NULL�� ������ �ü������ ���ױ� ������
		WSAGetLastError(),												//dwMessageId -> ���� �ڵ带 ���ڷ� ������
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),						//dwLanguageID -> ��� -> ������ ���� �������� �����ϸ� �ش� �ü�� ����Ʈ ���� ��µ�! -> ���뼺!, �ڵ��� ������ �� ���Ѱ� �Ƴ�
		(LPTSTR)&lpMsgBuf,												//lpBuffer -> ���� ���ڿ� ���� �ּ�
		0,																// ������ ����!
		NULL															// �ƱԸ�Ʈ ����!
	);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);				//�޼����ڽ� �Լ��� �����ڵ� ���ڿ� ���!

	LocalFree(lpMsgBuf);
	exit(1);
}


int main(int argc, char *argv[])
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	//socker()
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP ��Ĺ ���� ����", "�˸�", MB_OK);

	//closesocket()
	closesocket(tcp_sock);

	//���� ����
	WSACleanup();
	return 0;
}