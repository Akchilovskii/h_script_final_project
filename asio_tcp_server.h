#pragma once
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio.hpp>
#include<iostream>
#include<string>
#include<sstream>
using namespace std;
using namespace boost;

using address_type = asio::ip::address;
using socket_type = asio::ip::tcp::socket;
using acceptor_type = asio::ip::tcp::acceptor;
using endpoint_type = asio::ip::tcp::endpoint;

void try_func(string& out_str)
{
	boost::asio::io_service server_stream;
	acceptor_type acceptor(server_stream, endpoint_type(asio::ip::tcp::v4(), 9190));
	cout << acceptor.local_endpoint() << endl;
	for(;;)
	{
		ostringstream oss;
		socket_type socket(server_stream);
		acceptor.accept(socket);
		cout << socket.remote_endpoint().address() << endl;
		vector<char> str(socket.available() + 1, 0);
		socket.receive(asio::buffer(str));
		//cout<<&str[0];
		oss << &str[0];
		out_str=oss.str();
		//cout << out_str;
		if (out_str.length() > 400)
			break;
	}
}
class my_asio_server
{
public:
	my_asio_server(short port)
		:
		m_sock(m_io),
		port(port)
	{}
	void accept_new_sock()
	{
		acceptor_type acceptor(m_io, endpoint_type(asio::ip::tcp::v4(), port));
		acceptor.accept(m_sock);
		
	}
	void close_sock()
	{
		m_sock.close();
	}
	string server_receive()
	{
		ostringstream oss;
		vector<char> str_vec(m_sock.available() + 1);
		m_sock.receive(asio::buffer(str_vec));
		oss << &str_vec[0];
		return oss.str();
	}
	int send_by_server(string str)
	{
		auto bf = asio::buffer(str);
		return m_sock.send(asio::buffer(str));
	}
private:
	unsigned short port;
	boost::asio::io_service m_io;
	socket_type m_sock;
};

void string_replacer(string& str, string ori, string rep)
{
	int pos = -1;
	while ((pos = str.find(ori)) != string::npos)
	{
		str.replace(pos, ori.length(), rep);
	}
}

string post_request_context_reader(string post_req_line)
{
	int pos = post_req_line.find("\r\n\r\n");
	if (pos != string::npos)
	{
		return post_req_line.substr(pos + 4);
	}
	else
		return "";
}

void html_doc_processor(string& html_str)
{
	string_replacer(html_str, "%3D", "=");
	string_replacer(html_str, "+", " ");
	string_replacer(html_str, "%28", "(");
	string_replacer(html_str, "%29", ")");
	string_replacer(html_str, "%2C", ",");
	string_replacer(html_str, "%22", "\"");
	string_replacer(html_str, "%5B", "[");
	string_replacer(html_str, "%5D", "]");
	string_replacer(html_str, "%2B", "+");
	string_replacer(html_str, "%3C", "<");
	string_replacer(html_str, "%25", "%");
	string_replacer(html_str, "%3F", "?");
}

vector<string> post_request_devider(string req_context)
{
	vector<string> res;
	int pos = -1;
	while ((pos=req_context.find('&'))!=string::npos)
	{
		res.push_back(req_context.substr(0, pos));
		req_context = req_context.substr(pos + 1);
	}
	return res;
}