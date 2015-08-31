#include "PCString.h"
#include <stdio.h>

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

PCString* ConvertHead(PCString *line)
{
    int level, textStart;
    GetHead(line->text, &level, &textStart);

    if (level>=0 && level<=6 && textStart<line->len) {
        char s1[10], s2[10];
        sprintf(s1, "<h%d>", level);
        sprintf(s2, "</h%d>", level);

        PCString *r = PCStringNewFromPChar(s1, 0);
        PCString *text = PCStringSub(line, textStart, -1);
        PCString *p2 = PCStringNewFromPChar(s2, 0);
        PCStringAppend(r, text);
        PCStringAppend(r, p2);
        
        PCStringFree(&text);
        PCStringFree(&p2);
        return r;
    }
    return line;
}

char *ConvertMarkdown(char **strs, int count)
{
    PCString *result = PCStringNew(100);
    PCString *html_p = PCStringNewFromPChar("<p/>", 0);

    for (int i = 0; i < count; ++i) {

        PCString *line = PCStringNewFromPChar(strs[i], 1);

        if (line->len == 0) {
            PCStringAppend(result, html_p);
            PCStringFree(&line);
            continue;
        }
        if (line->text[0] == '#') {
            PCString *r = ConvertHead(line);
            PCStringAppend(result, r);
            PCStringFree(&r);
        } else {
            PCStringAppend(result, line);
        }
        PCStringFree(&line);
    }
    char *p = PCStrringToPChar(result);
    PCStringFree(&html_p);
    PCStringFree(&result);
    return p;
}
