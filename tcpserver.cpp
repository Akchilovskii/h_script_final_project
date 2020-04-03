#include "tcpserver.h"
using std::cout;

TcpServer::Client::Client()
	:
	client(INVALID_SOCKET)
{
	memset(&clientAddr, 0, sizeof(clientAddr));
	size = sizeof(clientAddr);
}

TcpServer::Client::~Client()
{
	closesocket(client);
	cout << "client: ip:" << GetIpAddress() << " has disconnected" << std::endl;
}



bool TcpServer::Client::operator!()
{
	return (client == INVALID_SOCKET);
}

std::string TcpServer::Client::GetIpAddress()
{
	std::string ip;
	unsigned int uip = clientAddr.sin_addr.S_un.S_addr;
	ip += std::to_string(uip & 0x000000ff) + ".";
	ip += std::to_string((uip & 0x0000ff00) >> 8) + ".";
	ip += std::to_string((uip & 0x00ff0000) >> 16) + ".";
	ip += std::to_string((uip & 0xff000000) >> 24);
	return ip;
}

SOCKET& TcpServer::Client::GetSocket()
{
	// TODO: 在此处插入 return 语句
	return client;
}

sockaddr* TcpServer::Client::GetAddr()
{
	return (sockaddr*)(&clientAddr);
}

int& TcpServer::Client::Size()
{
	// TODO: 在此处插入 return 语句
	return size;
}

TcpServer::TcpServer(short port)
{
	WSAData wsaData;
	int iret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iret != NO_ERROR) throw - 1;


	server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET) throw - 1;


	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(server, (sockaddr*)&saddr, sizeof(saddr)) != 0) throw - 2;


	if (listen(server, 5) == SOCKET_ERROR) throw - 3;
}

TcpServer::~TcpServer()
{
	if (server != INVALID_SOCKET) {
		closesocket(server);
		OutputDebugString("Server destructed\n");
	}
	WSACleanup();
}

std::shared_ptr<TcpServer::Client> TcpServer::Accept()
{
	std::shared_ptr<TcpServer::Client> client = std::make_shared<TcpServer::Client>();
	if (client->GetSocket()=accept(server, client->GetAddr(), &(client->Size())) == INVALID_SOCKET) {
		throw 100;
	}
	return client;
}


