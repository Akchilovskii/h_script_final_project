#pragma once

#include<WinSock2.h>
#include<vector>
#include<memory>
#include<string>
#include<iostream>
#pragma comment(lib, "ws2_32.lib")

class TcpServer {
public:
	class Client {
	public:
		Client();
		~Client();
		//Client(const Client&);
		bool operator!();
		std::string GetIpAddress();
		SOCKET& GetSocket();
		sockaddr* GetAddr();
		int& Size();
	private:
		SOCKADDR_IN clientAddr;
		SOCKET client;
		int size;
	};
	

public:
	TcpServer(short port);
	TcpServer(const TcpServer&) = delete;
	TcpServer& operator=(const TcpServer) = delete;
	~TcpServer();

public:
	std::shared_ptr<TcpServer::Client> Accept();



private:
	SOCKET server;
	SOCKADDR_IN saddr;
};