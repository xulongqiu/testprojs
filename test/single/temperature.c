/*************************************************************************
> File: temperature.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2017年04月10日 星期一 10时45分53秒
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define ADC_VOL_REF 1792  //mv
#define ADC_VAL_MAX 1024

struct temp_r_t {
    int temp;
    unsigned int resistor;
};


static const struct temp_r_t temp_table[] = {
    { -40, 195652},
    { -35, 148171},
    { -30, 113347},
    { -25, 87559},
    { -20, 68237},
    { -15, 53650},
    { -10, 42506},
    { -5,  33892},
    {0,   27219},
    {5,   22021},
    {10,  17926},
    {15,  14674},
    {20,  12081},
    {25,  10000},
    {30,  8315},
    {35,  6948},
    {40,  5834},
    {45,  4917},
    {50,  4161},
    {55,  3535},
    {60,  3014},
    {65,  2586},
    {70,  2228},
    {75,  1925},
    {80,  1669},
    {85,  1452},
    {90,  1268},
    {95,  1110},
    {100, 974},
    {105, 858},
    {110, 758},
    {115, 672},
    {120, 596},
    {125, 531}
};

int main(int argc, const char* argv[])
{
    int i = 0, j = 0, cur_temp = 125;
    int temp = 0;
    for (j = 52; j < 1024; j++) {
        uint16_t adc_val = j;
        unsigned int cur_resistor = (unsigned int)((1024.0 / adc_val - 1.0) * 10000.0);

        int cnt = sizeof(temp_table) / sizeof(struct temp_r_t);

        for (i = 0; i < cnt; i ++) {
            if (cur_resistor >= temp_table[i].resistor) {
                if (i == 0) {
                    cur_temp = -40;
                } else {
                    cur_temp = temp_table[i - 1].temp;
                    cur_temp += (int)((5.0 / (temp_table[i - 1].resistor - temp_table[i].resistor))
                                      * (temp_table[i - 1].resistor - cur_resistor));
                }

                break;
            }
        }
        if(cur_temp != temp){
            temp = cur_temp;
            printf("{%d, %d},\n",adc_val, cur_temp);
        }
    }

    return cur_temp;

}

