#pragma once
#include "标头.h"
#include <sstream>



#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost;
using namespace boost::gregorian;
using namespace std;

class FourteenDaysAgo {
public:
	FourteenDaysAgo(date mday = day_clock::local_day()) {
		phtml = new htmlE("html");
		ele head = phtml->addElement("head");
		ele meta = head->addElement("meta");
		meta->single = true;
		meta->attrib("charset", "gb2313");
		ele body = phtml->addElement("body");
		ele style = body->addElement("style");
		ele css = style->addElement("CSSStyle");
		css->attrib("name", ".spanstyle");
		css->attrib("left", "40%");
		css->attrib("top", "40%");
		css->attrib("width", "100px");
		css->attrib("height", "30px");
		css->attrib("postion", "fixed");
		css->attrib("color", "red");
		ele div = body->addElement("div");
		//div->attrib("class", "spanstyle");
		ele span = div->addElement("span");
		span->attrib("class", "spanstyle");
		ele text=span->addElement("textScript");
		date fourteen_days_ago = mday - days(14);
		oss << "今天是 " << day_clock::local_day()<<endl;
		oss << "您输入的日期的14天前是 ：" << fourteen_days_ago;
		text->innerText = oss.str();		
	}
	string GetString() {
		return phtml->getFinalString(phtml);
	}
private:
	ele phtml;
	ostringstream oss;
};