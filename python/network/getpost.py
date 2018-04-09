#!/usr/bin/python3.5
#coding=utf-8

import os.path
import sys
import urllib
import urllib3
import json

class GetPostClass:
    def __init__(self, url, ifile, ofile):
        self.url = url
        self.outfile = ofile
        self.infile = ifile
        pass
    
    def get(self):
        seek_byte = {
                    'User-Agent': 'Lavf/55.19.104',
                    'Accept': '*/*',
                    'Range': 'bytes=0-',
                    'Connection': 'close',
                    'Host': '192.168.31.178:61679',
                    'transferMode.dlna.org': 'Streaming'                               
                    }
        seek_time = {'User-Agent': 'Lavf/55.19.104',
                     'Accept': '*/*',
                     #'TimeSeekRange.dlna.org': 'npt=00:00:14.582-',
                     'TimeSeekRange.dlna.org': 'npt=00:00:00.000-',
                     'Connection': 'close',
                     'Host': '192.168.31.178:61679',
                     'transferMode.dlna.org': 'Streaming'
                     }
        http = urllib3.PoolManager()
        f = http.request('GET', 
                           self.url)
                           #headers = seek_time)
        data = f.read()
        headers = f.getheaders()
        
        print('---------------GET REQUEST------------------')
        for k in seek_time:
            print('%s: %s' % (k, seek_time[k]))  
            
        print('---------------GET RESPONSE------------------')
        print('Status:', f.status, f.reason)
        #TimeSeekRange.dlna.org: npt=00:00:14.582-/* bytes=604467-2428626/2428627
        pos = 0
        for k in headers:
            print('%s: %s' % (k, headers[k]))
            if k == 'TimeSeekRange.dlna.org':
                pos = int(headers[k].split('=')[2].split('-')[0])
                
        
        file_out = open(self.outfile, 'wb')
        if pos > 0:
            file_in = open(self.infile, 'rb')
            posBytes = file_in.read(pos)
            file_out.write(posBytes)
        
        file_out.write(f.data)
        file_out.close()
    

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Usage %s url infile outfile" % (sys.argv[0]))
        sys.exit(-1)
        pass
    
    print(sys.argv[0])
    print(sys.argv[1])
    print(sys.argv[2])
    print(sys.argv[3])
    getpost = GetPostClass(sys.argv[1], sys.argv[2], sys.argv[3])
    getpost.get()
        