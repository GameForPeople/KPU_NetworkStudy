#pragma comment(lib, "ws2_32")

//. 주석달기 = CTRL + K + C 주석해지 = CTRL + K + U

#include "MainClient.h"
#include "resource.h"

//사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(101), NULL, DlgProc);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hDlg, IDOK);
			return true;
		case IDCANCEL:
			EndDialog(hDlg, IDOK);
			return true;
		}
		return false;
	}
	return false;
}
//
//int main(int argc, char *argv[]) {
//	int retVal;
//
//#pragma region [// 윈속 초기화]
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		return 1;
//#pragma endregion
//
//#pragma region [ socket() ]
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET) err_quit("socket()");
//#pragma endregion
//
//#pragma region [ connect() ]
//	SOCKADDR_IN serverAddr;
//	ZeroMemory(&serverAddr, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
//	serverAddr.sin_port = htons(SERVERPORT);
//	retVal = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
//	if (retVal == SOCKET_ERROR) err_quit("bind()");
//#pragma endregion
//
//	FileListStruct fileList;
//	retVal = recv(sock, (char*)&fileList, sizeof(fileList), 0);
//
//	std::cout << "받고 싶은 파일을 선택하세요! " << std::endl;
//	std::cout << "1. " << fileList.fileName[0] << std::endl;
//	std::cout << "2. " << fileList.fileName[1] << std::endl;
//	std::cout << "3. " << fileList.fileName[2] << std::endl;
//	std::cout << "4. " << fileList.fileName[3] << std::endl;
//	std::cout << "5. " << fileList.fileName[4] << std::endl;
//
//	int inputFileNumber{};
//
//	rewind(stdin);
//	std::cin >> inputFileNumber;
//	rewind(stdin);
//
//	NeedDataStruct needData;
//	needData.type = inputFileNumber - 1;
//
//	retVal = send(sock, (char *)&needData, sizeof(needData), 0);
//
//	FixDataStruct fixData;
//	retVal = recv(sock, (char *)&(fixData), sizeof(fixData), 0);
//
//	long long int bufSize = fixData.size / BIG_DATA_COUNT;	
//	char* buf = new char[bufSize];
//	int count = fixData.size / bufSize;
//
//	FILE *fp = NULL;
//	fp = fopen(fileList.fileName[needData.type], "wb");
//
//	while (count) {
//		retVal = recvn(sock, buf, bufSize, 0);
//		//if (retVal == SOCKET_ERROR) {err_display("recv()");	break;}
//		fwrite(buf, 1, bufSize, fp);
//
//		count--;
//	}
//
//	count = fixData.size - (fixData.size / bufSize)* bufSize;
//
//	if (count) {
//		retVal = recvn(sock, buf, count, 0);
//		fwrite(buf, 1, count, fp);
//	}
//
//	fclose(fp);
//
//	std::cout << "전송을 끝냈습니다..." << std::endl;
//
//}