#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <string>

#include <atlsocket.h>

#define DOMAIN_MAX 255

using namespace std;
//get host by nam ,,, by address
#pragma region [Functions]
// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 도메인 이름 -> IPv4 주소
BOOL GetIPAddr(char *name, IN_ADDR *addr)
{
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
		return FALSE;

	memcpy(addr, ptr->h_addr, ptr->h_length);

	char **pAlias;
	int i = 0;

	for (pAlias = ptr->h_aliases; *pAlias != 0; pAlias++) {
		printf(" 별명은...  #%d: %s\n", ++i, *pAlias);
	}
	
	//cout << ptr->h_addrtype << endl;
	//memcpy(sname, *(ptr->h_aliases), sizeof(ptr->h_aliases));
	//if(!ptr->h_aliases)
		//cout << "별칭은 " << ptr->h_aliases[0] << endl;
	//cout << "별칭은 " << **(ptr->h_aliases) << endl;

	return TRUE;
}
BOOL GetIP6Addr(char *name, IN6_ADDR *addr)
{	
	struct addrinfo hints;
	struct addrinfo *result;
	LPSOCKADDR sockaddr_ip;

	SOCKADDR_IN6 *pSockAddr = nullptr;
	char addrBuffer[50];

	ZeroMemory(&hints, sizeof(hints));
	//hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_UNSPEC; //AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	//getaddrinfo((char *)&name, "0", &hints, &result);
	getaddrinfo(name, "0", &hints, &result);
	//cout << "ipv6의 주소는 " << result->ai_addr << endl;
	//pSockAddr = (SOCKADDR_IN6 *)result->ai_addr;
	
	//printf("IP6 address is %s\n", pSockAddr->sin6_addr);
	struct addrinfo *ptr = NULL;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		if (ptr->ai_family == AF_INET6) {
			sockaddr_ip = (LPSOCKADDR)result->ai_addr;
			int ipbufferlength = 46;
			char ipstringbuffer[46];
			INT iRetval;

			iRetval = WSAAddressToString(sockaddr_ip, (DWORD)result->ai_addrlen, NULL,
				ipstringbuffer, (LPDWORD)&ipbufferlength);
			//WSAAddressToStringA( (pSockAddr->sin6_addr) , sizeof(pSockAddr), NULL, addrBuffer, (LPDWORD)sizeof(addrBuffer));

			cout << "ipv6의 주소는 " << ipstringbuffer << endl;

			return TRUE;
		}
	}
}

// IPv4 주소 -> 도메인 이름
BOOL GetDomainName(IN_ADDR addr, char *name, int namelen)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
	
	struct addrinfo hints;
	struct addrinfo *result;
	
	getaddrinfo((char *)&addr, "0", &hints, &result);

	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET && ptr->h_addrtype != AF_INET6)
		return FALSE;

	//std::cout << ptr->h_name << std::endl;
	strncpy(name, ptr->h_name, namelen);
	return TRUE;
}

void UseDomainName() {
	char name[DOMAIN_MAX];
	printf("도메인 이름을 입력하세요! :  ", name);
	scanf("%s", name);
	printf("입력하신 도메인 이름은 %s 입니다. \n", name);

	IN_ADDR addr;
	IN6_ADDR addr6;

	if (GetIPAddr(name, &addr)) {
		// 성공이면 결과 출력
		printf("해당 도메인의 IPv4 주소는 = %s 입니다.\n", inet_ntoa(addr));
	}

	GetIP6Addr(name, &addr6);
}

void UseIpv4Addr() {
	char addr[DOMAIN_MAX];
	printf("IPv4를 입력하세요! :  ", addr);
	scanf("%s", addr);
	printf("입력하신 Ipv4는 %s 입니다. \n", addr);

	IN_ADDR insertAddr;
	insertAddr.s_addr = inet_addr(addr);

	// IP 주소 -> 도메인 이름
	char name[DOMAIN_MAX];
	if (GetDomainName(insertAddr, name, sizeof(name))) {
		// 성공이면 결과 출력
		printf("도메인 이름은 = %s\n", name);
	}
}
#pragma endregion

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	int insertFunction = 0;

	while (7) {
		cout << endl << "어떤 기능을 원하십니까?" << endl;
		cout << "1.도메인 , 2. ipv4 , 3.ipv6" << endl;
		rewind(stdin);
		cin >> insertFunction;

		switch (insertFunction) {
		case 1:
			UseDomainName();
			break;
		case 2:
			UseIpv4Addr();
			break;

		case 3:
			break;
		}
	}

	

	//// 도메인 이름 -> IP 주소
	//IN_ADDR addr;
	//if (GetIPAddr(name, &addr)) {
	//	// 성공이면 결과 출력
	//	printf("IP 주소(변환 후) = %s\n", inet_ntoa(addr));
	//
	//	// IP 주소 -> 도메인 이름
	//	char name[256];
	//	if (GetDomainName(addr, name, sizeof(name))) {
	//		// 성공이면 결과 출력
	//		printf("도메인 이름(다시 변환 후) = %s\n", name);
	//	}
	//}

	WSACleanup();

#pragma region [Exit Program]
	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
#pragma endregion
}