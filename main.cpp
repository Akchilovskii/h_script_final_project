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
	
	//脚本解释器
	script_interpretor si;
	try
	{
		
		/*                                  ***********简单的使用方法**************
		    “.” 是字符分割符号，但放在双引号中无效（不起分割作用），同时该符号能定位前一个元素下的名字为当前元素的html标签，如果不存在该标签，
			就会自动创建该标签
			“[]” 是元素选择器， 可以以逗号分隔，在其中填入整数， 会确切选择前一个元素下的第几个，或某几个名字为当前元素的html标签，与“.”结合时，
			如果不存在该标签，则会创建符合要求个数的标签，例如i[1],会创建两个i标签，为了满足能访问i[1]的需求， 该符号可以选择多个标签， 会为每个标签创建
			一个脚本历程，需要注意的是每次都会定位到最后一个选择的标签
			“=” 表示为前一个标签的某项属性赋值，与“.”结合，不存在该属性时，会自动添加该属性，可以使用或者不使用双引号，当属性值包含“.”时，请使用双引号，
			双引号不完整将引发异常
			“text”关键字，表示为标签内的innerhtml赋值，“single”关键字表示该元素为单标签元素
			“>>”符号滚动至同目录下，下一个同名标签
			“<”符号返回上一级目录
		*/
		ifstream inf("main.hScript");
		si.debug(false);
		if (!inf)
		{
			throw "file failed";
		}
		string cmd;
		cout << "hScript源码:" << endl;
		while (getline(inf,cmd))
		{
			//cout << cmd << endl;
			cout << cmd << endl;
			si.script(cmd);
			//cout << "\r\n current at: "<<si.print_current_position() << endl;
		}
		inf.close();
		cout << "\nhtml代码如下\n" << endl;
		cout<<si.show_html_code();
		ofstream outf("out.html");
		outf << si.show_html_code();
		outf.close();
		cout << "已保存至文件" << endl;
	}
	catch (const char * e) //抛出字符串常量，请勿模仿。。。。
	{
		cout << si.show_html_code();
		cout << e;
		cout << endl;
		return -1;
	}
	
}