#pragma comment(lib, "ws2_32")

#include "MainServer.h"

CRITICAL_SECTION ACCEPT_SECTION;

#pragma region [THREAD FUNCTION]
DWORD WINAPI ProcessClient(LPVOID arg) {
	ThreadStruct *threadStruct = (ThreadStruct *)arg;
	SOCKET clientSock = threadStruct->sock;
	
	int retVal;
	SOCKADDR_IN clientAddr;
	int addrlen;

	// 인자로 전달받은 정보로 구조체 생성
	FileListStruct fileList;
	for (int i = 0; i < 5; i++) {
	//memcpy(fileList.fileName1, threadStruct->fileName1, sizeof(threadStruct->fileName1));
	//memcpy(fileList.fileName2, threadStruct->fileName2, sizeof(threadStruct->fileName2));
	//memcpy(fileList.fileName3, threadStruct->fileName3, sizeof(threadStruct->fileName3));
	//memcpy(fileList.fileName4, threadStruct->fileName4, sizeof(threadStruct->fileName4));
	//memcpy(fileList.fileName5, threadStruct->fileName5, sizeof(threadStruct->fileName5));
	memcpy(fileList.fileName[i], threadStruct->fileName[i], sizeof(threadStruct->fileName[i]));
	}

	NeedDataStruct needData;

	//클라이언트 정보 획득!
	addrlen = sizeof(clientAddr);
	getpeername(clientSock, (SOCKADDR *)&clientAddr, &addrlen);

	retVal = send(clientSock, (char *)&fileList, sizeof(fileList), 0);
	if (retVal == SOCKET_ERROR) { err_display("send()"); }

	retVal = recv(clientSock, (char *)&needData, sizeof(needData), 0);
	if (retVal == SOCKET_ERROR) { err_display("recv()"); }
	else if (!retVal) return 0;

	// 보낼 데이터 만들기
	FILE *in;
	int ch;
	std::vector<int> cont;
	unsigned long long fileSize;
	int len;

	//error 확인.. 이름 제대로..?
	std::cout << "전송되는 파일의 이름은 : " << fileList.fileName[needData.type] << "  입니다. " << std::endl;

	if ((in = fopen(fileList.fileName[needData.type], "rb")) == NULL) { fputs("파일 열기 에러!", stderr); exit(1); }
	fseek(in, 0, SEEK_END);
	fileSize = _ftelli64(in);
	fseek(in, 0, SEEK_SET);

	FixDataStruct fixData;
	fixData.size = fileSize;

	retVal = send(clientSock, (char *)&(fixData), sizeof(fixData), 0);

	//std::cout << "전송되는 파일 (" << fileList.fileName[needData.type] << ") 의 사이즈 크기 값( "<< fixData.size << ")을 알려주었습니다. " << std::endl;

	len = fileSize / BIG_DATA_COUNT;
	char *buf = new char[len];

	for (int i = 0; i < BIG_DATA_COUNT; i++) {
		fread(buf, sizeof(char), len, in);
		retVal = send(clientSock, buf, len, 0);
		if (retVal == SOCKET_ERROR) { err_display("send()"); }
	}

	if (fixData.size - fixData.size / BIG_DATA_COUNT * BIG_DATA_COUNT) {
		retVal = send(clientSock, buf, fixData.size - fixData.size / BIG_DATA_COUNT * BIG_DATA_COUNT, 0);
		if (retVal == SOCKET_ERROR) { err_display("send()"); }
	}
	std::cout << "전송되는 파일 (" << fileList.fileName[needData.type] << ") 의 사이즈( " << fixData.size << ")만큼 전송하였습니다. " << std::endl;

	closesocket(clientSock);
	std::cout << "[ 클라이언트 접속 종료 : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port)  << std::endl;

	fclose(in); // 파일 닫기

	return 0;
}
#pragma endregion


int main(int argc, char *argv[])
{
	InitializeCriticalSection(&ACCEPT_SECTION);
	int retVal;

#pragma region [// 윈속 초기화]
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
#pragma endregion

#pragma region [ socket() ]
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) err_quit("socket()");
#pragma endregion

#pragma region [ Bind() ]
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retVal = bind(listenSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit("bind()");
#pragma endregion

#pragma region [ Listen() ]
	retVal = listen(listenSock, SOMAXCONN);
	if (retVal == SOCKET_ERROR) err_quit("listen()");

	std::cout << "-------- TCP 파일 전송 프로그램 서버 -----------" << std::endl;
	std::cout << "I  P : " << inet_ntoa(serverAddr.sin_addr) << std::endl;
	std::cout << "PORT : " << ntohs(serverAddr.sin_port) << std::endl;
	std::cout << "----- On Listen! ----" << std::endl;

#pragma endregion

#pragma region [Thread Acept]
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	HANDLE hThread;

	char fileStr[256];

	ThreadStruct threadArgument;
	memcpy(threadArgument.fileName[0], "MovieFile_1.mp4", 16);
	memcpy(threadArgument.fileName[1], "MovieFile_2.mp4", 16);
	memcpy(threadArgument.fileName[2], "MovieFile_3.mp4", 16);
	memcpy(threadArgument.fileName[3], "MovieFile_4.mp4", 16);
	memcpy(threadArgument.fileName[4], "MovieFile_5.mp4", 16);


	while (7) {
		//accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR *)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET) { err_display("accept()"); break; }

		std::cout << "[ 새로운 클라이언트 접속 IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << std::endl;
		threadArgument.sock = clientSock;

		hThread = CreateThread(NULL, 0, ProcessClient, &threadArgument, 0, NULL);
		if (hThread == NULL) { closesocket(clientSock); }
		else { CloseHandle(hThread); }
	}

#pragma endregion

#pragma region [ GoodBye Server! ]
	DeleteCriticalSection(&ACCEPT_SECTION);

	// closesocket()
	closesocket(listenSock);

	// 윈속 종료
	WSACleanup();

	return 0;
#pragma endregion
}