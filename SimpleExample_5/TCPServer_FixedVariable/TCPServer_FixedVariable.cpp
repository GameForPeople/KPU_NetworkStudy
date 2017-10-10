
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

#pragma comment(lib, "ws2_32")

#include "main.h"

#pragma region [CreateStatic]
HINSTANCE hInst;
HWND	  Hwnd;
LPCTSTR lpszClass = "Network GoFile  [ 2013192027 : ������ ]";

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//char *buf = new char[MAX_POWER_MAN];
#pragma endregion

#pragma region [Define WinMain]
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hwnd;

	hInst = hInstance;

	DWORD dwStyle =
		WS_OVERLAPPED 	// ����Ʈ ������. Ÿ��Ʋ �ٿ� ũ�� ������ �ȵǴ� ��輱�� ������. �ƹ��� ��Ÿ�ϵ� ���� ������ �� ��Ÿ���� ����ȴ�.
		| WS_CAPTION 		// Ÿ��Ʋ �ٸ� ���� �����츦 ����� WS_BORDER ��Ÿ���� �����Ѵ�.
		| WS_SYSMENU		// �ý��� �޴��� ���� �����츦 �����.
		| WS_MINIMIZEBOX	// �ּ�ȭ ��ư�� �����.
		| WS_BORDER			// �ܼ����� �� ��輱�� ����� ũ�� ������ �� �� ����.
							//		| WS_THICKFRAME		// ũ�� ������ ������ �β��� ��輱�� ������. WS_BORDER�� ���� ����� �� ����.
		;					// �߰��� �ʿ��� ������ ��Ÿ���� http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm ����.

	RECT getWinSize;	// �������� ũ�⸦ �޾ƿ�!
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	RECT rc;
	rc.left = rc.top = 0;
	rc.right = SCREEN_WIDTH;
	rc.bottom = SCREEN_HEIGHT;

	AdjustWindowRect(&rc, dwStyle, FALSE);	//�������� �����Ǵ� ���� ����

											//	Ŭ���̾�Ʈ ������ǥ(left, top)
											//	����ũ���� �߾ӿ� Ŭ���̾�Ʈ�� ��ġ�ϵ��� ����
	POINT ptClientWorld;
	ptClientWorld.x = (getWinSize.right - SCREEN_WIDTH) / 2;
	ptClientWorld.y = (getWinSize.bottom - SCREEN_HEIGHT) / 2;

	MSG Message;
	WNDCLASSEX WndClass;

#pragma region [Hello! My Name is Dirty Code!]
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hwnd = CreateWindow(
		lpszClass
		, lpszClass
		, dwStyle
		, ptClientWorld.x
		, ptClientWorld.y
		, SCREEN_WIDTH
		, SCREEN_HEIGHT
		, NULL
		, (HMENU)NULL
		, hInstance
		, NULL
	);

#pragma endregion
	memset(&Message, 0, sizeof(Message));

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (true)
	{
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)	break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		//else myFramework.Timer();
	}

	return Message.wParam;
}
#pragma endregion

#pragma region [WndProc]
LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC							hdc, Memdc;
	PAINTSTRUCT					ps;
	//HBITMAP hBitmap, OldBitmap;
	//HFONT hFont, saveFont;
	//HBRUSH Brush, oldBrush;
	//HPEN MyPen, OldPen, RedPen;

	static RECT					rect;

	static int					retval;
	static WSADATA				wsa;
	static SOCKET				listen_sock; // = socket(AF_INET, SOCK_STREAM, 0);

	static SOCKADDR_IN			serveraddr;

	static bool					isListen = false;

	static SOCKET				client_sock;
	static SOCKADDR_IN			clientaddr;
	static int					addrlen;
	static int					len = 0; //�Ⱦ�.

	static CImage				Button;
	static CImage				Grid;
	static CImage				ColorUI[2];
	static CImage				BarUI;
	static CImage				InBarUI;
	static CImage				CarUI;
	static CImage				NumberUI;
	static CImage				PerUI;
	static CImage				TypeUI;
	static CImage				CheckUI;

	//static BOOL					isOnFunction[3];
	static int					isOnFunction[4];	// LastFunction is ����ȭ����.. isOnFunction[3] == SyncError;
	int							Dis;
	int							barDis;
	int							numDis;
	int							numberPer[3];

	static std::thread			th;				// MainTh --> WinProc, SubTh --> TCP 

	switch (iMessage) {
	case WM_CREATE:
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;
		
		Button.Load("Button_1.png");
		Grid.Load("grid.png");
		ColorUI[0].Load("ColorUI_Red.png");
		ColorUI[1].Load("ColorUI_Blue.png");

		BarUI.Load("BarUI.png");
		InBarUI.Load("InBarUI.png");
		CarUI.Load("CarUI.png");

		NumberUI.Load("NumberUI.png");

		PerUI.Load("PerImg.png");

		TypeUI.Load("TypeUI.png");
		CheckUI.Load("CheckUI.png");

		for (int i = 0; i < 4; i++)
			isOnFunction[i] = FALSE;
		
		//
		GetClientRect(hwnd, &rect);
		SetTimer(hwnd, 1, 600, NULL);
		//

		break;
	}
	case WM_PAINT:
	{
		HDC mainHDC = BeginPaint(hwnd, &ps);
#pragma region [Doubble buffer]
		HBITMAP GLay = CreateCompatibleBitmap(mainHDC, SCREEN_WIDTH, SCREEN_HEIGHT);
		HDC hdc = CreateCompatibleDC(mainHDC);
		SelectObject(hdc, GLay);
		Rectangle(hdc, -10, -10, SCREEN_WIDTH + 20, SCREEN_HEIGHT + 20);
#pragma endregion
		SetStretchBltMode(mainHDC, COLORONCOLOR);
		SetStretchBltMode(hdc, COLORONCOLOR);

		Button.BitBlt(hdc, 20, 10, SRCCOPY);

		ColorUI[isOnFunction[0]].StretchBlt(hdc, 20, 10 + Button.GetHeight() + 10, 220, Button.GetHeight(), SRCCOPY);
		ColorUI[isOnFunction[1]].StretchBlt(hdc, 20 + 230, 10 + Button.GetHeight() + 10, 220, Button.GetHeight(), SRCCOPY);
		ColorUI[isOnFunction[2]].StretchBlt(hdc, 20 + 460, 10 + Button.GetHeight() + 10, 220, Button.GetHeight(), SRCCOPY);

		if (typeUI) {
			BarUI.BitBlt(hdc, 200, 550, SRCCOPY);

			mylock.lock();
			CarUI.BitBlt(hdc, 200 - CarUI.GetWidth() / 2 + distanceUI, (550 - CarUI.GetHeight()), SRCCOPY);
			Dis = distanceUI;
			mylock.unlock();

			numDis = 100 * Dis / OVERALL_LENGTH;
			numberPer[0] = numDis / 100;
			numberPer[1] = numDis / 10;
			numberPer[2] = numDis % 10;

			if (Dis == OVERALL_LENGTH)
				numberPer[1] = 0;

			if(numberPer[0])
				NumberUI.StretchBlt(hdc, 440, 350, 80, 80, numberPer[0] * NumberUI.GetWidth() / 10, 0, NumberUI.GetWidth() / 10, NumberUI.GetHeight(), SRCCOPY);
			
			NumberUI.StretchBlt(hdc, 520, 350, 80, 80, numberPer[1] * NumberUI.GetWidth() / 10, 0, NumberUI.GetWidth() / 10, NumberUI.GetHeight(), SRCCOPY);
			NumberUI.StretchBlt(hdc, 600, 350, 80, 80, numberPer[2] * NumberUI.GetWidth() / 10, 0, NumberUI.GetWidth() / 10, NumberUI.GetHeight(), SRCCOPY);
			PerUI.StretchBlt(hdc, 680, 350, 80, 80, SRCCOPY);

			barDis = Dis - 20;
			if (barDis < 0) barDis = 0;
			InBarUI.StretchBlt(hdc, 215, 565, barDis, InBarUI.GetHeight() - 10, SRCCOPY);

			TypeUI.BitBlt(hdc, 950, 20, SRCCOPY);


			if (typeUI) {
				if (typeUI == 1)
					CheckUI.BitBlt(hdc, 952, 20 + TypeUI.GetHeight(), SRCCOPY);
				else if (typeUI == 2)
					CheckUI.BitBlt(hdc, 1057, 20 + TypeUI.GetHeight(), SRCCOPY);
				else if (typeUI == 3)
					CheckUI.BitBlt(hdc, 1160, 20 + TypeUI.GetHeight(), SRCCOPY);
			}
		}

		SetStretchBltMode(mainHDC, COLORONCOLOR);
		SetStretchBltMode(hdc, COLORONCOLOR);
#pragma region [Doubble buffer]
		BitBlt(mainHDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

		DeleteDC(hdc);
		DeleteObject(GLay);
		EndPaint(hwnd, &ps);
#pragma endregion

		if (isOnFunction[2] && !isOnFunction[3]) {
			isOnFunction[3] = true;
			th = std::thread{ []() {
				Listen(retval, listen_sock);
				Recv(retval, listen_sock, client_sock, clientaddr, len, addrlen);
				}
			};
		}
		break;
	}
	case WM_TIMER:
	{
		InvalidateRgn(hwnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 'g' || wParam == 'G') {
		}
		break;
	}
	case WM_KEYUP:
	{
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		if (y > 10 && y < 10 + Button.GetHeight()) {
			if (x > 20 && x < 20 + Button.GetWidth() / 3) {
				// socket()
				Socket(listen_sock);
				isOnFunction[0] = TRUE;
				std::cout << "Socket is On" << std::endl;
			}
			else if (x > 20 + Button.GetWidth() / 3 && x < 20 + 2 * Button.GetWidth() / 3) {
				//bind()
				Bind(serveraddr, listen_sock, retval);
				isOnFunction[1] = TRUE;
				std::cout << "Bind is On" << std::endl;
			}
			else if (x > 20 + 2 * Button.GetWidth() / 3 && x < 20 + Button.GetWidth()) {
				std::cout << "Listen is On" << std::endl;
				isOnFunction[2] = TRUE;
			}
		}

		InvalidateRgn(hwnd, NULL, false);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		break;
	}
	case WM_DESTROY:
	{
		th.join();

		// closesocket()
		closesocket(listen_sock);

		// ���� ����
		WSACleanup();
		return 0;

		PostQuitMessage(0);
		return 0;
	}
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}
#pragma endregion

#pragma region [OldVer Main]
/*
int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char *buf = new char[999999999];
	int len;
	//std::vector<char> buf2;

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while(1){
			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if(retval == SOCKET_ERROR){
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}

			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, buf, len, 0);
			if(retval == SOCKET_ERROR){
				err_display("recv()");
				break;
			}
			else if(retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %02x\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			std::ofstream outFile("new2.mp4", std::ofstream::binary);
			outFile.write(buf, len);
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;

}
*/
#pragma endregion