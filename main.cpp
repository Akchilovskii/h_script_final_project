#include<iostream>
#include"htmlElement.h"  //html����
#include"html_doc.h" //html ������
#include"string_processor.h" //���ִ����ߣ� ������ҪΪ�ַ����ָ����
#include"script_interpretor.h"//�ű�������
#include<string>
#include<boost/tokenizer.hpp>
#include<boost/beast.hpp>
using namespace std;

int main()
{
	script_interpretor si;
	try
	{
		script_interpretor si; //�ű�������
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
		si.script("html.body.div[0,1,2].form.action=\"http://192.168.0.1:9190/index.htm\".input.single.placeholder=\"do me a favor.\".type=text.<.input[1].single.type=submit");
		si.script("body.div[0].form.p[3].<");
		si.script("p[0].text=i love you");
		si.script(">>.text= i hate you");
		si.script(">>.text= i lied");
		si.script(">>.text=but all those i do,they're for you");
		//si.script(">>.text = for you");
		//si.script("for i(0,2) in 2html : i.text = hello world");
		cout << si.show_html_code();
	}
	catch (const char * e) //�׳��ַ�������������ģ�¡�������
	{
		//cout << si.show_html_code();
		cout << e;
		return -1;
	}
	
}