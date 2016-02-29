#include "PCString.h"

PCString* PCStringNew(int size)
{
    PCString *str = (PCString*)malloc(sizeof(PCString));
    str->allocated_len = size+1;
    str->text = (char*)malloc(size+1);
    memset(str->text, 0, str->allocated_len);
    str->len = 0;
    str->is_shared = 0;
    return str;
}

char* PCStrringToPChar(PCString *str)
{
    char *pchar = (char*)malloc(str->len+1);
    memcpy(pchar, str->text, str->len);
    pchar[str->len] = '\0';
    return pchar;
}

PCString* PCStringNewFromPChar(const char *ch, int is_shared)
{
    int len = strlen(ch);
    PCString *str = (PCString*)malloc(sizeof(PCString));
    str->is_shared = is_shared;

    if (!is_shared) {
        str->allocated_len = len+1;
        str->text = (char*)malloc(str->allocated_len);
        memset(str->text, 0, str->allocated_len);
        strcpy(str->text, ch);
        str->len = len;

    } else {
        str->allocated_len = 0;
        str->text = (char*)ch;
        str->len = len;
    }
    return str;
}

void PCStringSetAlloc(PCString *str, int size)
{
    if (size > str->allocated_len) {
        char *new_text = (char*)malloc(size);
        memset(new_text, 0, size);
        strcpy(new_text, str->text);
        free(str->text);
        str->text = new_text;
        str->allocated_len = size;
    }
}

void PCStringCut(PCString *str, int len)
{
    if (str->len > len) {
        char *p = str->text + len;

        memset(p, 0, str->len-len);
        str->len = len;
    }
}

PCString* PCStringClone(PCString *str)
{
    PCString *new_str = (PCString*)malloc(sizeof(PCString));

    if (str->is_shared) {
        new_str->allocated_len = str->len+1;
        
    } else {
        new_str->allocated_len = str->allocated_len;
    }
    new_str->text = (char*)malloc(new_str->allocated_len);
    strcpy(new_str->text, str->text);
    new_str->len = str->len;
    new_str->is_shared = 0;
    return new_str;
}

void PCStringFree(PCString **str)
{
    if (*str) {
//        int is_shard = (*str)->is_shared;
        if (!(*str)->is_shared) {
            if ((*str)->text) {
                free((*str)->text);
                (*str)->text = 0;
                (*str)->len = 0;
                (*str)->allocated_len = 0;
            }
        } else {
            (*str)->text = 0;
            (*str)->len = 0;
            (*str)->allocated_len = 0;
        }
        free(*str);
    }
}

void PCStringClear(PCString *str)
{
    memset(str->text, 0, str->len);
    str->len = 0;
}

void PCStringAppend(PCString *str, const PCString *append)
{
    int new_all = str->len + append->len + 1;
    if (new_all <= str->allocated_len) {
       //直接连接 
        strcat(str->text, append->text);
        str->len += append->len;

    } else {
        //新增alloc
        PCStringSetAlloc(str, new_all*2);
        PCStringAppend(str, append);
    }
}

PCString* PCStringSub(const PCString *str, int start, int len)
{
    if (start < 0) {
        len += start;
        start = 0;
    }
    if (start >= str->len) {
        return 0;
    }
    if (start + len > str->len) {
        len = str->len-start;
    }
    if (len < 0) {
        len = str->len-start;
    }

    char *p = str->text + start;
    PCString *sub = PCStringNew(len);
    memcpy(sub->text, p, len);
    sub->len = len;
    sub->is_shared = 0;
    return sub; 
}

void PCStringRemove(PCString *str, int start, int len)
{
    if (start < 0) {
        len += start;
        start = 0;
    }
    if (start >= str->len) {
        return;
    }

    if (start+len > str->len) {
        len = str->len-start;
    }
    if (len < 0) {
        len = str->len-start;
    }

    PCString *sub = PCStringSub(str, start+len, -1);
    PCStringCut(str, start);

    if (sub) {
        PCStringAppend(str, sub);
        
        PCStringFree(&sub);
    }

    return;
}

void PCStringInsert(PCString *str, const PCString *sub, int start)
{
    //正则的值[0,len-1],就像在vim中如果仅仅用i命令是无法在末尾增加字符
    if (start >= str->len) {
        PCStringAppend(str, sub);
        return;
    }

    if (start < 0) {
        start = 0;
    }
  
    PCString *sub2 = PCStringSub(str, start, -1);
    PCStringCut(str, start);
    
    PCStringAppend(str, sub);
    PCStringAppend(str, sub2);

    PCStringFree(&sub2);
}

int PCStringFindChar(PCString *str, const char ch, int from)
{
    if (from < 0) {
        from = 0;
    }
    for (int i = from; i < str->len; ++i) {
        if (str->text[i] == ch) {
            return i;
        }
    }
    return -1;
}

int PCStringFindLastChar(PCString *str, const char ch, int from)
{
    if (from > str->len-1) {
        from = str->len-1;
    }
    for (int i = from; i >= 0; --i) {
        if (str->text[i] == ch) {
            return i;
        }
    }
    return -1;
}

void PCStringReplace(PCString *str, const char ch, const PCString *sub)
{
    int i = 0;
    while (1) {
        int index = PCStringFindChar(str, ch, i);

        if (index == -1) {
            return;
        }
        PCStringRemove(str, index, 1);
        PCStringInsert(str, sub, index);
        i = index + sub->len;

        if (i >= str->len) {
            return;
        }
    }
}

void PCStringReplace2(PCString *str, const char ch, const char *sub)
{
    PCString *s2 = PCStringNewFromPChar(sub, 0);
    PCStringReplace(str, ch, s2);

    PCStringFree(&s2);
}
