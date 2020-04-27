#include"tcpserver.h"
#include"script_interpretor.h"
#include<string>
#include<fstream>
#include"asio_tcp_server.h"
#include"string_processor.h"


using namespace std;







string keyboard_input(const char end_char)
{
	char i = 0;
	string result;
	cin >> noskipws;
	while (i != end_char)
	{
		cin >> i;
		if (i == '\n')
			continue;
		result += i;
	}
	cin >> skipws;
	result.pop_back();
	return result;
}

class server_processing
{
private:
	script_interpretor m_si;
	my_asio_server m_asio_serv;

public:
	server_processing(short port)
		:
		m_asio_serv(port)
	{}

	vector<string> read_file(string file_name)
	{
		vector<string> res;
		ifstream inf(file_name);
		if (!inf)
		{
			throw "file not exist error";
		}
		string str;
		while (getline(inf, str))
		{
			res.push_back(str);
		}
		inf.close();
		return res;
	}
	void write_file(string file_name, vector<string>& cmds)
	{
		if (file_name == "default new user.hScript")
		{
			return;
		}
		ofstream outf(file_name);
		for (auto& x : cmds)
		{
			outf << x << "\n";
		}
		outf.close();
	}
	void run()
	{
		string protocal = "HTTP 1.1 200 OK\r\n";
		string c_type = "Content-type:text/html\r\n";
		string c_length = "2048\r\n";
		string delim = "\r\n\r\n";

		try
		{
			string http_message;
			while (http_message.length() < 300)
			{
				http_message = m_asio_serv.server_receive();
			}
			auto post_req_line = post_request_context_reader(http_message);

			auto post_context_vec = post_request_devider(post_req_line);

			string username;
			string code;

			if (post_context_vec.size() < 2)
			{
				cout<< "warning! server processing: didn't read user name or code"<< endl;
				username = "code";
				code = "html.body";
			}
			else
			{
				code = post_context_vec.at(0);
				username = post_context_vec.at(1);
				html_doc_processor(username);
				html_doc_processor(code);
			}
			string file_name = username + ".hScript";

			auto cmds_list = read_file(file_name);
			cmds_list.push_back(code);

			script_interpretor si;
			for (auto& i : cmds_list)
			{
				cout << i << endl;
				si.script(i);
				cout << si.show_html_code() << endl;
			}
			
			write_file(file_name, cmds_list);
			auto html_source = si.show_html_code();

			m_asio_serv.send_by_server(protocal);
			m_asio_serv.send_by_server(c_type);
			m_asio_serv.send_by_server(c_length);
			m_asio_serv.send_by_server(delim);
			m_asio_serv.send_by_server(html_source);
		}
		catch (const char* e)
		{
			cout << e << endl;
		}

	
	}
};



int main()
{

	/*
	server_processing sp(9190);
	for (int i = 0; i < 10; i++)
	{
		sp.run();
	}
	*/
	//return 1;
	/*
	string protocal = "HTTP 1.1 200 OK\r\n";
	string c_type = "Content-type:text/html\r\n";
	string c_length = "2048\r\n";
	string delim = "\r\n\r\n";
	my_asio_server server(9190);
	string str;
	
	while (1)
	{
		str = server.server_receive();
		if (str.length() > 300)
			break;
	}
	//str = server.server_receive();
	//cout << str;
	//return 2;
	//try_func(str);
	str = post_request_context_reader(str);
	auto h_code = str.substr(0,str.find('&'));
	h_code = h_code.substr(5);
	string_replacer(h_code, "%3D", "=");
	cout << "code:" << h_code << endl;
	str=str.substr(str.find('&') + 1);
	auto user_name= str.substr(0, str.find('&'));
	cout << "user:" << user_name.substr(10) << endl;
	str = str.substr(str.find('&') + 1);
	script_interpretor sii;
	sii.script(h_code);
	cout<<sii.show_html_code();
	server.send_by_server(protocal);
	server.send_by_server(c_type);
	server.send_by_server(c_length);
	server.send_by_server(delim);
	server.send_by_server(sii.show_html_code());

	//cout << str;
	return 1;*/
	
	//�ű�������
	script_interpretor si;
	try
	{
		
		/*                                  ***********�򵥵�ʹ�÷���**************
		    ��.�� ���ַ��ָ���ţ�������˫��������Ч������ָ����ã���ͬʱ�÷����ܶ�λǰһ��Ԫ���µ�����Ϊ��ǰԪ�ص�html��ǩ����������ڸñ�ǩ��
			�ͻ��Զ������ñ�ǩ
			��[]�� ��Ԫ��ѡ������ �����Զ��ŷָ������������������� ��ȷ��ѡ��ǰһ��Ԫ���µĵڼ�������ĳ��������Ϊ��ǰԪ�ص�html��ǩ���롰.�����ʱ��
			��������ڸñ�ǩ����ᴴ������Ҫ������ı�ǩ������i[1],�ᴴ������i��ǩ��Ϊ�������ܷ���i[1]������ �÷��ſ���ѡ������ǩ�� ��Ϊÿ����ǩ����
			һ���ű����̣���Ҫע�����ÿ�ζ��ᶨλ�����һ��ѡ��ı�ǩ
			��=�� ��ʾΪǰһ����ǩ��ĳ�����Ը�ֵ���롰.����ϣ������ڸ�����ʱ�����Զ���Ӹ����ԣ�����ʹ�û��߲�ʹ��˫���ţ�������ֵ������.��ʱ����ʹ��˫���ţ�
			˫���Ų������������쳣
			��text���ؼ��֣���ʾΪ��ǩ�ڵ�innerhtml��ֵ����single���ؼ��ֱ�ʾ��Ԫ��Ϊ����ǩԪ��
			��>>�����Ź�����ͬĿ¼�£���һ��ͬ����ǩ
			��<�����ŷ�����һ��Ŀ¼
		*/
		ifstream inf("main.hScript");
		si.debug(false);
		if (!inf)
		{
			throw "file failed";
		}
		string cmd;
		cout << "hScriptԴ��:" << endl;
		while (getline(inf,cmd))
		{
			//cout << cmd << endl;
			cout << cmd << endl;
			si.script(cmd);
			//cout << "\r\n current at: "<<si.print_current_position() << endl;
		}
		inf.close();
		cout << "\nhtml��������\n" << endl;
		cout<<si.show_html_code();
		ofstream outf("out.html");
		outf << si.show_html_code();
		outf.close();
		cout << "�ѱ������ļ�" << endl;
	}
	catch (const char * e) //�׳��ַ�������������ģ�¡�������
	{
		cout << si.show_html_code();
		cout << e;
		cout << endl;
		return -1;
	}
	
}