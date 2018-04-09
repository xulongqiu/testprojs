#include <stdio.h>

static void insertion_sort(int* arry,int Length)
{
    for (int i = 1; i < Length; i++) {
        if (arry[i - 1] > arry[i]) {
            int temp = arry[i];
            int j = i;

            while (j > 0 && arry[j - 1] > temp) {
                arry[j] = arry[j - 1];
                j--;
            }

            arry[j] = temp;
        }
    }
}

#define LOG_LVL 1

#define debugPrint(fmt, args...) do{ \
    if(LOG_LVL <= 0) \
        break; \
    printf("[fusb302-%s]: "fmt, __FUNCTION__, ## args); \
}while(0);

int main(void)
{
    int i = 1;
    debugPrint("i=%d\n", 1);
    return 0;
}
