#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <string>

#include <atlsocket.h>

#define DOMAIN_MAX 255

using namespace std;
//get host by nam ,,, by address
#pragma region [Functions]
// ���� �Լ� ���� ���
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

// ������ �̸� -> IPv4 �ּ�
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
		printf(" ������...  #%d: %s\n", ++i, *pAlias);
	}
	
	//cout << ptr->h_addrtype << endl;
	//memcpy(sname, *(ptr->h_aliases), sizeof(ptr->h_aliases));
	//if(!ptr->h_aliases)
		//cout << "��Ī�� " << ptr->h_aliases[0] << endl;
	//cout << "��Ī�� " << **(ptr->h_aliases) << endl;

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
	//cout << "ipv6�� �ּҴ� " << result->ai_addr << endl;
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

			cout << "ipv6�� �ּҴ� " << ipstringbuffer << endl;

			return TRUE;
		}
	}
}

// IPv4 �ּ� -> ������ �̸�
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
	printf("������ �̸��� �Է��ϼ���! :  ", name);
	scanf("%s", name);
	printf("�Է��Ͻ� ������ �̸��� %s �Դϴ�. \n", name);

	IN_ADDR addr;
	IN6_ADDR addr6;

	if (GetIPAddr(name, &addr)) {
		// �����̸� ��� ���
		printf("�ش� �������� IPv4 �ּҴ� = %s �Դϴ�.\n", inet_ntoa(addr));
	}

	GetIP6Addr(name, &addr6);
}

void UseIpv4Addr() {
	char addr[DOMAIN_MAX];
	printf("IPv4�� �Է��ϼ���! :  ", addr);
	scanf("%s", addr);
	printf("�Է��Ͻ� Ipv4�� %s �Դϴ�. \n", addr);

	IN_ADDR insertAddr;
	insertAddr.s_addr = inet_addr(addr);

	// IP �ּ� -> ������ �̸�
	char name[DOMAIN_MAX];
	if (GetDomainName(insertAddr, name, sizeof(name))) {
		// �����̸� ��� ���
		printf("������ �̸��� = %s\n", name);
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
		cout << endl << "� ����� ���Ͻʴϱ�?" << endl;
		cout << "1.������ , 2. ipv4 , 3.ipv6" << endl;
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

	

	//// ������ �̸� -> IP �ּ�
	//IN_ADDR addr;
	//if (GetIPAddr(name, &addr)) {
	//	// �����̸� ��� ���
	//	printf("IP �ּ�(��ȯ ��) = %s\n", inet_ntoa(addr));
	//
	//	// IP �ּ� -> ������ �̸�
	//	char name[256];
	//	if (GetDomainName(addr, name, sizeof(name))) {
	//		// �����̸� ��� ���
	//		printf("������ �̸�(�ٽ� ��ȯ ��) = %s\n", name);
	//	}
	//}

	WSACleanup();

#pragma region [Exit Program]
	char bufferToStop;
	std::cin >> bufferToStop;

	return 0;
#pragma endregion
}