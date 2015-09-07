// TestPCString.cpp : 定义控制台应用程序的入口点。
//
#include <Windows.h>
#include <crtdbg.h>
#include "..\src\PCString.h"

void TestPCString()
{
    //PCString str1 = PCStringNewFromPChar("xbc");
    //PCString str1 = PCStringNew(10);
    //PCString str2 = PCStringNewFromPChar("123");
    //PCStringSetAlloc(&str2, 10);
    //PCString str3 = PCStringNewFromPChar("abc");
    //PCStringAppend(&str1, str2);
    //PCStringAppend(&str2, str3);
    //PCString *s = PCStringNewFromPChar("123abc", 0);
    //PCString *sub = PCStringSub(s, 3, 10);
    //PCString *s2 = PCStringNewFromPChar("xbc", 0);
    //PCStringRemove(s, -1, -1);
    //PCStringInsert(s, s2, 3);
    //PCStringCut(s, 4);
    //PCStringFree(&s);
    //PCStringFree(&s2);
    return;
}
void TestString2()
{
    PCString *str = PCStringNewFromPChar("<code>", 0);
    PCString *lt = PCStringNewFromPChar("&lt;", 0);
    PCString *gt = PCStringNewFromPChar("&gt;", 0);
    PCStringReplace(str, '<', lt);
    PCStringReplace(str, '>', gt);

    PCStringFree(&str);
    PCStringFree(&lt);
    PCStringFree(&gt);
    return;
}


int main(int argc, char** argv)
{
    //TestPCString();
    TestString2();

    _CrtDumpMemoryLeaks();
	return 0;
}

