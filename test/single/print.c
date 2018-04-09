/*************************************************************************
> File: Documents/print.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2016年11月29日 星期二 09时44分09秒
*************************************************************************/

#include<stdio.h>
#include <stdio.h>
#include <stdarg.h>

#define console_print(ch) putchar(ch)

static void printch(char ch)
{
    console_print(ch);
}

static void printdec(int dec)
{
    if (dec == 0) {
        return;
    }

    printdec(dec / 10);
    printch((char)(dec % 10 + '0'));
}

static void printflt(double flt)
{
    int tmpint = 0;

    tmpint = (int)flt;
    printdec(tmpint);
    printch('.');
    flt = flt - tmpint;
    tmpint = (int)(flt * 1000000);
    printdec(tmpint);
}

static void printstr(char* str)
{
    while (*str) {
        printch(*str++);
    }
}

static void printbin(int bin)
{
    if (bin == 0) {
        printstr("0b");
        return;
    }

    printbin(bin / 2);
    printch((char)(bin % 2 + '0'));
}

static void printhex(int hex)
{
    if (hex == 0) {
        printstr("0x");
        return;
    }

    printhex(hex / 16);

    if (hex < 10) {
        printch((char)(hex % 16 + '0'));
    } else {
        printch((char)(hex % 16 - 10 + 'a'));
    }
}

void print(char* fmt, ...)
{
    double vargflt = 0;
    int  vargint = 0;
    char* vargpch = NULL;
    char vargch = 0;
    char* pfmt = NULL;
    va_list vp;

    va_start(vp, fmt);
    pfmt = fmt;

    while (*pfmt) {
        if (*pfmt == '%') {
            switch (*(++pfmt)) {

            case 'c':
                vargch = va_arg(vp, int);
                /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                 *                            mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                printch(vargch);
                break;

            case 'd':
            case 'i':
                vargint = va_arg(vp, int);
                printdec(vargint);
                break;

            case 'f':
                vargflt = va_arg(vp, double);
                /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                 *                            mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                printflt(vargflt);
                break;

            case 's':
                vargpch = va_arg(vp, char*);
                printstr(vargpch);
                break;

            case 'b':
            case 'B':
                vargint = va_arg(vp, int);
                printbin(vargint);
                break;

            case 'x':
            case 'X':
                vargint = va_arg(vp, int);
                printhex(vargint);
                break;

            case '%':
                printch('%');
                break;

            default:
                break;
            }

            pfmt++;
        } else {
            printch(*pfmt++);
        }
    }

    va_end(vp);
}

int main(void)
{
    print("print: %c\n", 'c');
    print("print %d\n", 1234567);
    print("print: %f\n", 1234567.1234567);
    print("print: %s\n", "string test");
    print("print: %b\n", 0x12345ff);
    print("print: %x\n", 0xabcdef);
    print("print: %%\n");

    return 0;
}


