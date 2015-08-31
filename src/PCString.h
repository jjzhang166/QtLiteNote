#include <stdlib.h>
#include <string.h>
#include <tchar.h>

typedef struct _PCString PCString;

struct _PCString {
    char *text;
    int len;
    int allocated_len;

    int is_shared; //�Ƿ��������PCString����ԭʼ��char*���͹���text������ǹ�����ô��PCStringFree�н������ͷ�text
};

/* 
* 
* ����PCStringFree()�ͷ��ڴ�
*/
PCString* PCStringNew(int size);

/* 
* ��ȡc���͵��ַ��� 
* @return ����ֵ�õ�free()���ͷ��ڴ�
*/
char* PCStrringToPChar(PCString *str);


/* 
* @ch ԭʼ��char*�ַ���  
* @is_shared ���Ϊtrue,��ô��PCStringFree()ʱ�������ͷ�text�е��ڴ�
*            ���Ϊfalse,
*/
PCString* PCStringNewFromPChar(char *ch, int is_shared);

/* 
* ��������alloc�����������@size�����е�С����ʲôҲ������ 
*
*/
void PCStringSetAlloc(PCString *str, int size);

/* 
* ��strǿ�Ʋü�Ϊ����Ϊlen�����str->len��@lenС����ʲôҲ������ 
*�� 
*/
void PCStringCut(PCString *str, int len);

/* 
*  
* ����PCStringFree()�ͷ��ڴ�
*/
PCString* PCStringClone(PCString *str);

/* 
* ���str�е��ı��������ͷ��ڴ� 
*
*/
void PCStringClear(PCString *str);

/* 
* �ͷ��ڴ� 
*/
void PCStringFree(PCString **str);

void PCStringAppend(PCString *str, const PCString *append);

/* 
* ��start��ʼ,���ɳ���Ϊlen��str���ַ���
* @start ���С��0����ô���Զ�ָ��0�����һ���Ӧ�޸�len
*        ���start������str,��ô�����ؿ�ֵ
* @len   ���start+len ����str, ��ô���Զ���Сlen����Ӧstr
*        len���Ϊ��������ô���ǽ�ȡ��start��ʼ�������е��ַ���
* @return ����PCStringFree()�ͷ��ڴ�
*/
PCString* PCStringSub(const PCString *str, int start, int len);

/* 
* ��start��ʼ,�Ƴ�����Ϊlen�Ĳ���
* @start ���С��0����ô���Զ�ָ��0�����һ���Ӧ�޸�len
*        ���start������str,��ô�����ؿ�ֵ
* @len   ���start+len ����str, ��ô���Զ���Сlen����Ӧstr
*        len���Ϊ��������ô��ɾ����start��ʼ�������е��ַ���
*/
void PCStringRemove(PCString *str, int start, int len);

/* 
* ��sub���뵽str�е�startλ�� 
* @start ��startλ��֮ǰ����,������vim�а�iһ��
*        ���startС��0���Զ�����0,���start����str��������β�����ӣ��൱��PCStringAppend
*/
void PCStringInsert(PCString *str, const PCString *sub, int start);