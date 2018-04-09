/*************************************************************************
> File: list_me.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn 
> Time:   2018-03-06 16:16:55
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#define LIST_DEBUG 1

typedef int Elem_t;

typedef struct Node{
    Elem_t ele;
    struct Node* next;
}Node_t;

typedef Node_t* List;

typedef enum{
    ListRetOk,
    ListRetErr
}ListRet_t;

ListRet_t listInit(List* L)
{
    *L = (List)malloc(sizeof(Node_t));
    if(!(*L)){
        return ListRetErr;
    }
    (*L)->next = NULL;

    return ListRetOk;
}

ListRet_t listDump(List L)
{
    List pn = NULL;
    int i = 0;

    if(!L){
        return ListRetErr;
    }
    pn = L->next;
    while(pn){
        printf("Element[%d]=%d\n", i, pn->ele);
        pn = pn->next;
        i++;
    }

    return ListRetOk;
}

ListRet_t listInsert(List L, int i, Elem_t ele)
{
    int j = 0;
    List p = L;
    List inser = NULL;

    if(!L){
        return ListRetErr;
    }
    while(p && j < i){
        p = p->next;
        j++;
    }
    inser = (Node_t*)malloc(sizeof(Node_t));
    if(!inser){
        return ListRetErr;
    }
    inser->next = p->next;
    inser->ele = ele;
    p->next = inser;
#if LIST_DEBUG
    listDump(L);
#endif
    return ListRetOk;
}

ListRet_t listAddHead(List L, Elem_t ele)
{
    return listInsert(L, 0, ele);
}

ListRet_t listAppend(List L, Elem_t ele)
{
    List tail = L;

    if(!L){
        return ListRetErr;
    }

    while(tail->next){
        tail = tail->next;
    }

    if((tail->next = (Node_t*)malloc(sizeof(Node_t))) == NULL){
        return ListRetErr;
    }
    tail->next->next = NULL;
    tail->next->ele = ele;
#if LIST_DEBUG
    listDump(L);
#endif
   
    return ListRetOk;
}

ListRet_t listReverse(List L)
{
    List cur, pn, pre;

    if(!L || !L->next){
        return ListRetErr;
    }

    cur = L->next;
    pn = cur->next;
    cur->next = NULL;

    while(pn){
        pre = pn->next;
        pn->next = cur;
        cur = pn;
        pn = pre;
        L->next = cur;
    }
    L->next = cur;

#if LIST_DEBUG
    listDump(L);
#endif
    return ListRetOk;
}

int main(int argc, const char* argv[])
{
    List L = NULL;
    ListRet_t ret = ListRetOk;

    if(argc == 0 || argv[0] == NULL){
    
    }
    ret = listInit(&L);
    printf("listAddHead: 0\n");
    ret = listAddHead(L, 0);
    printf("listInsert: i=1, ele=1\n");
    ret = listInsert(L, 1, 1);
    printf("listInsert: i=2, ele=2\n");
    ret = listInsert(L, 2, 2);
    printf("listAppend: ele=3\n");
    ret = listAppend(L, 3);
    printf("listReverse\n");
    ret = listReverse(L);

    return 0;
}
