#include"tcpserver.h"
#include"script_interpretor.h"
#include<string>


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

int main()
{
	TcpServer my_server(9190);
	auto client = my_server.Accept();
	if (!client)
	{
		cout << "invalid socket connect" << endl;
	}
	char message[20] = { 0 };
	SOCKET s = client->GetSocket();
	int len=recv(s, message, 20, 0);
	cout << message << endl;
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
		string cmdline;
		while ((cmdline = keyboard_input(';')) != "quit")
		{
			si.script(cmdline);
			cout << si.show_html_code();
		}
		//si.script(">>.text = for you");
		//si.script("for i(0,2) in 2html : i.text = hello world");
		
	}
	catch (const char * e) //�׳��ַ�������������ģ�¡�������
	{
		cout << si.show_html_code();
		cout << e;
		return -1;
	}
	
}