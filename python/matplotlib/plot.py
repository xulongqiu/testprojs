#!/usr/bin/python
#coding=utf-8

import os.path
import sys
import random
import numpy as np  
import pandas as pd
import matplotlib.pyplot as plt  


class SampleChart:
    def __init__(self, outPath):
        self.outPath = outPath
        #图表字体为华文细黑，字号为15
        plt.rc('font', family='STXihei', size=15)        
        pass
    def plot_cycles(self, dict_data, file_name, typeArray): 
        disp_font = matplotlib.font_manager.FontProperties(fname='/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf')
        maxes_val = []
        maxes_cnt = []
        min_val = []
        plt.figure(figsize=(9, 6))
        #main_fig = plt.subplot(211)
        if len(dict_data) > 0:
            #print 'have %d series...' % len(dict_data)
            new_data = []
            # 每组数据
            for key in dict_data:
                temp_temp_data = dict_data[key]
                if 'lineChart' in typeArray:
                    maxes_val.append(max(temp_temp_data))
                    maxes_cnt.append(len(temp_temp_data))
                    min_val.append(min(temp_temp_data))
                    new_data.append(temp_temp_data[len(temp_temp_data) / 2])
                    datas = key.split('_')
                    if datas[0].find('b') == 0 and len(datas) == 3:
                        plt.plot(range(0, len(temp_temp_data), 1), temp_temp_data, datas[2],label=key)
                    else:
                        plt.plot(range(0, len(temp_temp_data), 1), temp_temp_data,label=key)
                        
                if 'matchedCurve' in typeArray:
                    p = np.polyfit(range(0, len(temp_temp_data), 1),temp_temp_data, 4)
                    y_av = np.polyval(p, range(0, len(temp_temp_data), 1))
                    plt.plot(range(0, len(temp_temp_data), 1), y_av, label=key)  
                    datas = key.split('_')
                    if datas[0].find('b') == 0 and len(datas) == 3:
                        plt.plot(range(0, len(temp_temp_data), 1), temp_temp_data, datas[2],label=key)
                    else:
                        plt.plot(range(0, len(temp_temp_data), 1), temp_temp_data,label=key)
                    
                plt.legend()
    
    
        plt.title(file_name, fontproperties=disp_font)
        plt.xlabel(u'time(seconds)', fontproperties=disp_font)
        plt.ylabel(u'bandwidth(bytes)', fontproperties=disp_font)
        if len(maxes_cnt):
            plt.xlim(0.0, max(maxes_cnt) + 40)
        if len(maxes_val):
            plt.ylim(min(min_val) - 20, max(maxes_val) + 10)
    
        plt.grid(False)
        ax = plt.axes()
        ax.yaxis.grid()
        #ax.xaxis.grid()
        #plt.show()  
        curve_name = file_name + '.png'
        plt.savefig(curve_name)
        plt.close()
        pass      
    
    def lineChart(self, dict_data, file_name, title, xlable, ylable): 
        max_val = []
        min_val = []
        max_cnt = []        
        
        if isinstance(dict_data, dict) == False:
            raise TypeError('dict_data need dictionary type!!')
        
        #绘制图形大小
        plt.figure(figsize=(9, 6)) 
        
        for line_key in dict_data:
            line_data = dict_data[line_key]
            if not isinstance(line_data, list) and not isinstance(line_data, tuple):
                raise TypeError("dict_data's elements need to be list or tuple")
            max_val.append(max(line_data))
            min_val.append(min(line_data))
            max_cnt.append(len(line_data))
            plt.plot(range(0, len(line_data), 1),line_data,label = line_key, linewidth=2,markeredgewidth=3,markeredgecolor='#99CC01',alpha=0.8)
            
            
        #x 轴长度
        if len(max_cnt):
            plt.xlim(0.0, max(max_cnt) + 40)
        # y 轴长度
        if len(max_val):
            plt.ylim(min(min_val) - 20, max(max_val) + 10)    
          
        # 图例位置  
        plt.legend(loc='upper right')    
        #添加x轴标签
        plt.xlabel(xlable)
        #添加y周标签
        plt.ylabel(ylable)
        #添加图表标题
        plt.title(title)
        #添加图表网格线，设置网格线颜色，线形，宽度和透明度
        plt.grid( color='#95a5a6',linestyle='--', linewidth=1 ,axis='y',alpha=0.4)
        #设置数据分类名称
        #plt.xticks(a, ('1月','2月','3月','4月','5月','6月','7月','8月','9月','10月','11月','12月') )
        #输出图表
        plt.show()
        pass
    
    def barChart(self, dict_array, file_name, title, xlable, ylable):
        max_val = []
        min_val = []
        max_cnt = []   
        clors = ['r', 'g', 'b', 'c', 'm']
        bar_width = 0.8 
        cnt = 0
        dict_data = {}
        
        if len(dict_array) > 1:
            bar_width = bar_width/len(dict_array)
        
        if isinstance(dict_array, dict) == False:
            raise TypeError('dict_data need dictionary type!!')   

        #绘制图形大小
        plt.figure(figsize=(9, 6))        
        
        for key in dict_array:
            dict_data = dict_array[key]
            if isinstance(dict_data, dict) == False:
                raise TypeError('dict_data[key] need dictionary type!!')   
            
            #创建一个一维数组赋值给
            index =np.arange(len(dict_data))
            
            #创建柱状图，数据源为按用户等级汇总的贷款金额，设置颜色，透明度和外边框颜色
            plt.bar(index + cnt * bar_width, dict_data.values(), bar_width, color=clors[cnt%len(clors)],
                    alpha=0.4, edgecolor='white', label = key)
            cnt = cnt + 1
            
        #设置x轴标签
        plt.xlabel(xlable)
        #设置y周标签
        plt.ylabel(ylable)
        #设置图表标题
        plt.title(title)
        #设置图例的文字和在图表中的位置
        plt.legend(loc='upper right')
        #设置背景网格线的颜色，样式，尺寸和透明度
        plt.grid(color='#95a5a6',linestyle='--', linewidth=1,axis='y',alpha=0.4)
        #设置数据分类名称
        plt.xticks(index + (len(dict_array) - 1) * bar_width/2, dict_data.keys())
        plt.tight_layout()
        #显示图表
        #plt.show()
        plt.savefig(self.outPath + file_name)
        plt.close()

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print 'sys.argc=(%d)' % len(sys.argv)
        sys.exit(-1)
        pass
    print sys.argv[0] 
    print sys.argv[1]
    
    plot = SampleChart(sys.argv[1])
    dict_data = {}
    list_data1= []
    list_data2= []
    for i in range(20000):
        list_data1.append(random.randint(25, 45))
        list_data2.append(random.randint(40, 55))
    dict_data['40M'] = list_data1 
    dict_data['60M'] = list_data2
    
    #plot.lineChart(dict_data, 'LineExample', 'LineExample', 'time(seconds)', 'bandwidth(Mbits)')
    
    dict_data = {}
    
    dict_data[u'B47'] = {'high':44, 'normal': 27}
    dict_data[u'B69'] = {'high':33, 'normal': 22}
    dict_data[u'B52'] = {'high':45, 'normal': 32}
    dict_data[u'B07'] = {'high':30, 'normal': 21}
    dict_data[u'B45'] = {'high':28, 'normal': 22}
    dict_data[u'B46'] = {'high':41, 'normal': 25}
    dict_data[u'B32'] = {'high':33, 'normal': 18}
    dict_data[u'B53'] = {'high':45, 'normal': 30}
    dict_data[u'B33'] = {'high':32, 'normal': 21}
    dict_data[u'B51'] = {'high':41, 'normal': 30}
    
    plot.barChart(dict_data, u'CPU_Temp_Up.png', u'CPU ∆T', u'board num', u'∆T(°C)')
