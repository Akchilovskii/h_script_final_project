#include<iostream>
#include"htmlElement.h"
#include"html_doc.h"
#include"string_processor.h"
#include"script_interpretor.h"
#include<string>
#include<boost/tokenizer.hpp>
#include<boost/beast.hpp>
using namespace std;

int main()
{
	try
	{
		script_interpretor si;
		si.script("html.body.div[0,1,2].form.input.single");
		si.script(".is.miserable");
		//si.script("for i(0,2) in 2html : i.text = hello world");
		cout << si.show_html_code();
	}
	catch (const char * e)
	{
		cout << e;
		return -1;
	}
	
}