<<<<<<< HEAD
#include"tcpserver.h"
#include"script_interpretor.h"
=======
#include<iostream>
#include"htmlElement.h"  //html主类
#include"html_doc.h" //html 辅助类
#include"string_processor.h" //文字处理工具， 包括主要为字符串分割保护器
#include"script_interpretor.h"//脚本解释类
>>>>>>> a79583aad532b57d2c5163f548efeef6db657598
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
<<<<<<< HEAD
		
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
=======
		script_interpretor si; //脚本解释器
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
>>>>>>> a79583aad532b57d2c5163f548efeef6db657598
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
	catch (const char * e) //抛出字符串常量，请勿模仿。。。。
	{
		cout << si.show_html_code();
		cout << e;
		return -1;
	}
	
}
