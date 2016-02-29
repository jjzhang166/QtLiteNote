#include <stdlib.h>
#include <string.h>
//#include <tchar.h>

typedef struct _PCString PCString;

struct _PCString {
    char *text;
    int len;
    int allocated_len;

    int is_shared; //是否和其它的PCString或者原始的char*类型共享text，如果是共享那么在PCStringFree中将不会释放text
};

/* 
* 
* 得用PCStringFree()释放内存
*/
PCString* PCStringNew(int size);

/* 
* 获取c类型的字符串 
* @return 返回值得到free()来释放内存
*/
char* PCStrringToPChar(PCString *str);


/* 
* @ch 原始的char*字符串  
* @is_shared 如果为true,那么在PCStringFree()时将不会释放text中的内存
*            如果为false,
*/
PCString* PCStringNewFromPChar(const char *ch, int is_shared);

/* 
* 尝试增大alloc的容量，如果@size比现有的小，将什么也不会做 
*
*/
void PCStringSetAlloc(PCString *str, int size);

/* 
* 将str强制裁剪为长度为len，如果str->len比@len小，将什么也不会做 
*： 
*/
void PCStringCut(PCString *str, int len);

/* 
*  
* 得用PCStringFree()释放内存
*/
PCString* PCStringClone(PCString *str);

/* 
* 清除str中的文本，但不释放内存 
*
*/
void PCStringClear(PCString *str);

/* 
* 释放内存 
*/
void PCStringFree(PCString **str);

void PCStringAppend(PCString *str, const PCString *append);

/* 
* 从start开始,生成长度为len的str子字符串
* @start 如果小于0，那么将自动指向0，并且会相应修改len
*        如果start超过了str,那么将返回空值
* @len   如果start+len 超过str, 那么将自动减小len以适应str
*        len如果为负数，那么就是截取从start开始往后所有的字符串
* @return 得有PCStringFree()释放内存
*/
PCString* PCStringSub(const PCString *str, int start, int len);

/* 
* 从start开始,移除长度为len的部分
* @start 如果小于0，那么将自动指向0，并且会相应修改len
*        如果start超过了str,那么将返回空值
* @len   如果start+len 超过str, 那么将自动减小len以适应str
*        len如果为负数，那么将删除从start开始往后所有的字符串
*/
void PCStringRemove(PCString *str, int start, int len);

/* 
* 将sub插入到str中的start位置 
* @start 在start位置之前插入,就像在vim中按i一样
*        如果start小于0则自动正则到0,如果start超过str，将会在尾部增加，相当于PCStringAppend
*/
void PCStringInsert(PCString *str, const PCString *sub, int start);

/* 
* 在PCString中从头开始找char, 如果找到则返回其对应的index,否则返回-1
*
*/
int PCStringFindChar(PCString *str, const char ch, int from);

/* 
* 在 PCString中从尾开始找char, 如果找到则返回其对应的index,否则返回-1
*
*/
int PCStringFindLastChar(PCString *str, const char ch, int from);

/* 
* 将str中所有的ch字符都替换为sub 
*
*/
void PCStringReplace(PCString *str, const char ch, const PCString *sub);

void PCStringReplace2(PCString *str, const char ch, const char *sub);
