#include<iostream>
#include"htmlElement.h"  //html主类
#include"html_doc.h" //html 辅助类
#include"string_processor.h" //文字处理工具， 包括主要为字符串分割保护器
#include"script_interpretor.h"//脚本解释类
#include<string>
#include<boost/tokenizer.hpp>
#include<boost/beast.hpp>
using namespace std;

int main()
{
	script_interpretor si;
	try
	{
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
	catch (const char * e) //抛出字符串常量，请勿模仿。。。。
	{
		//cout << si.show_html_code();
		cout << e;
		return -1;
	}
	
}
