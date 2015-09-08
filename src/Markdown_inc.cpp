#include "PCString.h"
#include <stdio.h>

/*
objective-c 与标准c不兼容的地方
 1. oc不支持&变量引用
 2. oc不支持全局变量用函数的返回值来初始化
 */

/*static char *s_head = "<html> <head> <meta http-equiv=\"content-type\" content=\"text/html\"; charset=\"UTF-8\"/>"
"<style type=\"text/css\">"
"body{ background-color: #c7edcc; color: #444444; line-height: 1; padding: 5px; word-wrap: break-word; } p { font-family: \".HelveticaNeueInterface-Regular\";  font-size: 50px; margin-top: 3px;margin-bottom:3px; }"
"h1, h2, h3, h4 {"
"color: #111111;"
"font-weight:bold;"
"}"
"h1, h2, h3, h4, h5, p {"
"margin-bottom: 16px;"
"padding: 0;"
"}"
"h1 {"
"font-size: 28px;"
"border-bottom: 1px solid;"
"}"
"h2 {"
"font-size: 22px;"
"margin: 20px 0 6px;"
"font-weight: bold;"
"color:#2F517B"
"}"
"h3 {"
"font-size: 21px;"
"text-decoration: underline;"
"}"
"h4 {"
"font-size: 18px;"
"}"
"h5 {"
"font-size: 16px;"
"}"
"</style>"
"</head>"
"<body><p/>"
;*/

extern char *s_head;
static char *s_end = "</body> </html>";


PCString *tag_br;
PCString *tag_p;
PCString *tag_tab;
PCString *tag_ol1;
PCString *tag_ol2;
PCString *tag_ul1;
PCString *tag_ul2;
PCString *tag_code1;
PCString *tag_code2;

void InitTag()
{
    tag_br = PCStringNewFromPChar("<br/>", 0);
    tag_p = PCStringNewFromPChar("<p/>", 0);
    tag_tab = PCStringNewFromPChar("&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;", 0);
    tag_ol1 = PCStringNewFromPChar("<ol>", 0);
    tag_ol2 = PCStringNewFromPChar("</ol><p/>", 0);
    tag_ul1 = PCStringNewFromPChar("<ul>", 0);
    tag_ul2 = PCStringNewFromPChar("</ul><p/>", 0);
    tag_code1 = PCStringNewFromPChar("<pre><code>", 0);
    tag_code2 = PCStringNewFromPChar("</code></pre><p/>", 0);
}

void FreeTag()
{
    PCStringFree(&tag_br);
    PCStringFree(&tag_p);
    PCStringFree(&tag_tab);
    PCStringFree(&tag_ol1);
    PCStringFree(&tag_ol2);
    PCStringFree(&tag_ul1);
    PCStringFree(&tag_ul2);
    PCStringFree(&tag_code1);
    PCStringFree(&tag_code2);
}

//判断是否为标题
// @level 返回标题的层次[1,6]
// @textStart 返回去掉前面的#，后面剩下的文本的起始位置
void GetHead(char *str, int *level, int *textStart)
{
    int len = strlen(str);
    int state = 0;
    for (int i = 0; i < len; ++i) {

        switch (state) {
            case 0:
                if (str[0] == '#') {
                    state = 1;
                } else {
                    *level = 0;
                    *textStart = 0;
                    return;
                }
                break;
            case 1:
                if (str[i] == '#') {

                } else if (str[i] == ' ') {
                    *level = i;
                    state = 2;
                } else {
                    *level = i;
                    *textStart = i;
                    return;

                }
                break;
            case 2:
                if (str[i] == ' ') {

                } else {
                    *textStart = i;
                    return;
                }
                break;
        }
    } // end for
    *textStart = len;
}

void ConvertHtmlTag(PCString *str)
{
    PCStringReplace2(str, '&', "&amp;");
    PCStringReplace2(str, '<', "&lt;");
    PCStringReplace2(str, '>', "&gt;");
    PCStringReplace2(str, '"', "&quot;");
    PCStringReplace2(str, '\'', "&qpos;");
}

PCString* ConvertHead(PCString *line)
{
    int level = 0, textStart = 0;
    GetHead(line->text, &level, &textStart);

    if (level>=0 && level<=6 && textStart<line->len) {
        char s1[10], s2[10];
        sprintf(s1, "<h%d>", level);
        sprintf(s2, "</h%d><p/>", level);

        PCString *result = PCStringNewFromPChar(s1, 0);
        PCString *text = PCStringSub(line, textStart, -1);
        ConvertHtmlTag(text);
        PCString *p2 = PCStringNewFromPChar(s2, 0);
        PCStringAppend(result, text);
        PCStringAppend(result, p2);
        
        PCStringFree(&text);
        PCStringFree(&p2);
        return result;
    }
    return PCStringClone(line);
}

int IsCode(PCString *line)
{
    if (line->len == 3) {
        if (strcmp(line->text, "```") == 0) {
            return 1;
        }
    }
    return 0;
}

int IsNumber(char ch)
{
    if (ch >= '0' && ch <= '9') {
        return 1;
    }
    return 0;
}

//是否有有序列表,前面跟 数字.空格<ol> </ol>
int IsOl(PCString *line)
{
    int black_index = PCStringFindChar(line, ' ', 0);
    if (black_index < 2) {
        return 0;
    }
    if (line->text[black_index-1] != '.') {
        return 0;
    }

    for (int i = 0; i < black_index-1; ++i) {
        if (!IsNumber(line->text[i])) {
            return 0;
        }
    }
    return 1;
}

//无序列表, 前面只有*
int IsUl(PCString *line)
{
    if (line->len < 3) {
        return 0;
    }
    if (line->text[0] == '*' && line->text[1] == ' ') {
        return 1;
    }
    return 0;
}

int IsTab(PCString *line)
{
    if (line->len <= 1) {
        return 0;
    }
    if (line->len >= 5) {
        if (line->text[0] == ' ' && line->text[1] == ' ' && line->text[2] == ' ' && line->text[3] == ' ') {
            return 1;
        }
    }
    if (line->text[0] == '\t') {
        return 1;
    }
    return 0;
}

PCString* ConvertLi(PCString *line)
{
    PCString *tag_li2 = PCStringNewFromPChar("</li>", 0);

    int black_index = PCStringFindChar(line, ' ', 0);
    int start_index = -1;
    for (int i = black_index+1; i < line->len; ++i) {
        if (line->text[i] != ' ') {
            start_index = i;
            break;
        }
    }
    if (start_index == -1) {
        PCString *r = PCStringNewFromPChar("<li></li>", 0);
        return r;
    }
    PCString *text = PCStringSub(line, start_index, -1);
    ConvertHtmlTag(text);

    PCString *result = PCStringNewFromPChar("<li>", 0);
    PCStringAppend(result, text);
    PCStringAppend(result, tag_li2);
    PCStringFree(&tag_li2);
    PCStringFree(&text);

    return result;
}

void ConvertNormal(PCString *line, int *section, PCString *result)
{
    if (line->len == 0) {
        *section = 0;
        PCStringAppend(result, tag_br);
    }
    else if (line->text[0] == '#') {
        *section = 0;

        PCString *r = ConvertHead(line);
        PCStringAppend(result, r);
        PCStringFree(&r);

    } else if (IsCode(line)) {
        *section = 1;

        PCStringAppend(result, tag_code1);

    } else if (IsOl(line)) {
        *section = 2;

        PCStringAppend(result, tag_ol1);

        PCString *r = ConvertLi(line);
        PCStringAppend(result, r);

        PCStringFree(&r);

    } else if (IsUl(line)) {
        *section = 3;

        PCStringAppend(result, tag_ul1);

        PCString *r = ConvertLi(line);
        PCStringAppend(result, r);

        PCStringFree(&r);
    }
    else {
        *section = 0;
        
        PCString *text = PCStringClone(line);
        ConvertHtmlTag(text);
        PCStringAppend(result, text);
        PCStringAppend(result, tag_br);
        
        PCStringFree(&text);
    }
}

char* ConvertMarkdown(char **strs, int count)
{
    InitTag();
    
    PCString *result = PCStringNewFromPChar(s_head, 0);
    PCString *html_end = PCStringNewFromPChar(s_end, 0);

    // 指定是否进入代码块、或者ol、ul块
    // 0 未进入
    // 1 code 块
    // 2 ol 块,有序列表，即前面跟字数
    // 3 ul 块,无序列表，即前面有*
    int section = 0;
    //TODO:sectin得建立一个栈, 因为在ol中还可能有pre_code，pre_code结束后得接着上面的ol

    for (int i = 0; i < count; ++i) {
       
        PCString *line = PCStringNewFromPChar(strs[i], 1);

        if (section == 0) {
            ConvertNormal(line, &section, result);

        } else if (section == 1) {
            
            if (IsCode(line)) {
                section = 0;
                PCStringAppend(result, tag_code2);
            } else {
                PCString *text = PCStringClone(line);
                ConvertHtmlTag(text);
                PCStringAppend(result, text);
                PCStringAppend(result, tag_br);

                PCStringFree(&text);
            }

        } else if (section == 2) { //ol
            if (IsOl(line)) {
                PCString *r = ConvertLi(line);
                PCStringAppend(result, r);
                PCStringFree(&r);
            } else if (IsTab(line)) {
                PCString *r = PCStringSub(line, 1, -1);
                PCStringAppend(result, tag_tab);
                PCStringAppend(result, r);
                PCStringAppend(result, tag_br);
                PCStringFree(&r);
            }
            else {
                PCStringAppend(result, tag_ol2);
                ConvertNormal(line, &section, result);
            }
        } else if (section == 3) { //ul
            if (IsUl(line)) {
                PCString *r = ConvertLi(line);
                PCStringAppend(result, r);
                PCStringFree(&r);
            } else if (IsTab(line)) {
                PCString *r = PCStringSub(line, 1, -1);
                PCStringAppend(result, tag_tab);
                PCStringAppend(result, r);
                PCStringAppend(result, tag_br);
                PCStringFree(&r);
            }
            else {
                PCStringAppend(result, tag_ul2);
                ConvertNormal(line, &section, result);
            }
        }
      
        PCStringFree(&line);
    } // end for

    if (section == 1) {
        PCStringAppend(result, tag_code2);
    } else if (section == 2) {
        PCStringAppend(result, tag_ol2);
    } else if (section == 3) {
        PCStringAppend(result, tag_ul2);
    } 

    
    PCStringAppend(result, html_end);
    char *p = PCStrringToPChar(result);
    
    PCStringFree(&result);
    PCStringFree(&html_end);
    
    FreeTag();
    
    return p;
}
