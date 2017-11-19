#pragma comment(lib, "ws2_32")

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

//. 주석달기 = CTRL + K + C 주석해지 = CTRL + K + U

#include "MainClient.h"
#include "resource.h"

int selectFile{0};
//사용자 정의 데이터 수신 함수

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(101), NULL, DlgProc);
	return 0;
}

bool isOnCreateThread{ false };
static SOCKET sock;
static FileListStruct fileList;
static FILE *fp = NULL;
static HWND progressBar;

//std::atomic<int> fileSize{};
//std::atomic<int> recvDataSize{};

static int fileSize{};
static int recvDataSize{};
static bool isFlag{ true };

CRITICAL_SECTION PERCENT_SECTION;

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

DWORD WINAPI ThreadFunction(LPVOID arg){
	ThreadStruct *threadStruct = (ThreadStruct *)arg;
	std::cout << threadStruct->fileName << "  " << sizeof(threadStruct->fileName) << std::endl;
	//char* fileName{};
	//memcpy(fileName, threadStruct->fileName, sizeof(threadStruct->fileName));
	static FixDataStruct fixData;
	int	retVal = recv(sock, (char *)&(fixData), sizeof(fixData), 0);

	fileSize = fixData.size;
	std::cout << "받을 파일의 크기는 : " << fixData.size << " 입니다." << std::endl;

	long long int bufSize = fixData.size / BIG_DATA_COUNT;
	char* buf = new char[bufSize];
	int count = fixData.size / bufSize;

	fp = fopen(threadStruct->fileName, "wb");
	
	while (count) {
		//if (isFlag) {
			retVal = recvn(sock, buf, bufSize, 0);
			EnterCriticalSection(&PERCENT_SECTION);
			recvDataSize += retVal;
			//isFlag = false;
			LeaveCriticalSection(&PERCENT_SECTION);

			std::cout << "받았어! " << std::endl;
			//if (retVal == SOCKET_ERROR) {err_display("recv()");	break;}
			fwrite(buf, 1, bufSize, fp);
			count--;
		//}
	}
	
	count = fixData.size - (fixData.size / bufSize)* bufSize;
	
	if (count) {
		retVal = recvn(sock, buf, count, 0);

		EnterCriticalSection(&PERCENT_SECTION);
		recvDataSize += retVal;
		LeaveCriticalSection(&PERCENT_SECTION);

		fwrite(buf, 1, count, fp);
	}
	
	fclose(fp);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO5, IDC_RADIO1); // hdlg안에 있는 1~4까지에서 체크
		SetDlgItemText(hDlg, IDC_IPADDRESS1, "127.000.000.001");
		//InitCommonControls();
		progressBar = GetDlgItem(hDlg, IDC_PROGRESS1);
		SendMessage(progressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendMessage(progressBar, PBM_SETPOS, 0, 0);
		InitializeCriticalSection(&PERCENT_SECTION);
		
#pragma region [// 윈속 초기화]
		static WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;
#pragma endregion

#pragma region [ socket() ]
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
#pragma endregion

		return true;
	case WM_TIMER:
		if (recvDataSize) {
			EnterCriticalSection(&PERCENT_SECTION);
			SendMessage(progressBar, PBM_SETPOS, (float)recvDataSize / (float)fileSize * 100, 0);
			//isFlag = true;
			LeaveCriticalSection(&PERCENT_SECTION);
			if(recvDataSize == fileSize)
				KillTimer(hDlg, 0);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hDlg, IDOK);
			DeleteCriticalSection(&PERCENT_SECTION);
			return true;
		case IDCANCEL:
			EndDialog(hDlg, IDOK);
			DeleteCriticalSection(&PERCENT_SECTION);
			return true;
		case IDC_BUTTON1:	//ip주소 입력후 누르는 버튼
			//HANDLE IPADDRESS = GetDlgItem(hDlg, IDC_IPADDRESS1);         // 핸들을 얻고자 하는 컨트롤의 ID
			static DWORD address;
			SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_GETADDRESS, 0, (LPARAM)&address);

			#pragma region [ connect() ]
			static SOCKADDR_IN serverAddr;
			ZeroMemory(&serverAddr, sizeof(serverAddr));
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_addr.s_addr = htonl(address);
			serverAddr.sin_port = htons(SERVERPORT);
			static int retVal = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
			if (retVal == SOCKET_ERROR) err_quit("bind()");
			#pragma endregion
			
			retVal = recv(sock, (char*)&fileList, sizeof(fileList), 0);

			//static HWND radio1 = GetDlgItem(hDlg, IDC_RADIO1);
			//EnableWindow(radio1, true);
			SetDlgItemText(hDlg, IDC_EDIT1, fileList.fileName[0]);
			SetDlgItemText(hDlg, 1017, fileList.fileName[1]);
			SetDlgItemText(hDlg, 1018, fileList.fileName[2]);
			SetDlgItemText(hDlg, 1019, fileList.fileName[3]);
			SetDlgItemText(hDlg, 1020, fileList.fileName[4]);

			break;
			//return true;
		case IDC_BUTTON2: 	//파일 선택후 누르는 버튼
			//flag = (IsDlgButtonChecked(hDlg, IDC_MAN) == BST_CHECKED);
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)  selectFile = 0;
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED) selectFile = 1;
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)  selectFile = 2;
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED) selectFile = 3;
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO5) == BST_CHECKED) selectFile = 4;

			NeedDataStruct needData;
			needData.type = selectFile;

			retVal = send(sock, (char *)&needData, sizeof(needData), 0);

			if (!isOnCreateThread) {
				isOnCreateThread = true;
				static ThreadStruct threadArgument;
				memcpy(threadArgument.fileName, fileList.fileName[selectFile], sizeof(fileList.fileName[selectFile]));
				
				std::cout << fileList.fileName[selectFile] << std::endl;
				std::cout << threadArgument.fileName << std::endl;

				HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, &threadArgument, 0, NULL);
				if (hThread == NULL) { closesocket(sock); }
				else { CloseHandle(hThread); }

				SetTimer(hDlg, 0, 100, NULL);
			}

			break;
			//return true;
		}
		return false;
	}
	return false;
}



