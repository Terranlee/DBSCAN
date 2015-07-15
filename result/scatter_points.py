# -*- coding: utf-8 -*-
from __future__ import division
import re
import sys
import numpy as np
import matplotlib.pyplot as plt

class ScatterPoints(object):
    """docstring for ScatterPoints"""
    def __init__(self):
        super(ScatterPoints, self).__init__()

    def readin_origin(self, filename):
        '''
        return (x, y, color)
        list color are all 1, refers to black
        '''
        x = list()
        y = list()
        color = list()
        content = open(filename).readlines()
        for i in content:
            li = re.split(r'\s*', i.strip())
            if len(li) != 2:
                print 'origin datatype do not match: %s' % (i)
                assert False
            x.append(float(li[0]))
            y.append(float(li[1]))
            color.append(1)
        return x, y, color

    def readin_answer(self, filename):
        '''
        return (x, y, color)
        list color refers to different cluster
        '''
        x = list()
        y = list()
        color = list()
        content = open(filename).readlines()
        for i in content:
            li = re.split(r'\s*', i.strip())
            if len(li) != 3:
                print 'answer datatype do not match: %s' % (i)
                assert False
            x.append(float(li[0]))
            y.append(float(li[1]))
            color.append( int(li[2])+2)
        return x, y, color

    def readin(self, filename, filetype):
        if filetype == 'origin':
            return self.readin_origin(filename)
        elif filetype == 'answer':
            return self.readin_answer(filename)
        else:
            print 'unknown readin type: %s' % (filetype)
            assert False

    def show_result(self, x, y, color):
        size = [10] * len(x)
        max_col = max(color)
        for i in color:
            i = i / max_col
        plt.scatter(x,y, c=color, s=size)
        plt.show()
    
    def show_result_diff(self, x, y, color):
        size = [10] * len(x)
        max_col = max(color)
        for i in color:
            i = i / max_col
        for i in range(0, len(x)):
            if color[i] == 0:
                size[i] = 2
        plt.scatter(x,y, c=color, s=size)
        plt.show()

    def plot(self, filename, filetype):
        x, y, color = self.readin(filename, filetype)
        self.show_result(x, y, color)

    def readin_diff(self, filename1, filename2):
        x = list()
        y = list()
        color = list()
        content1 = open(filename1).readlines()
        content2 = open(filename2).readlines()
        if len(content1) != len(content2):
            print 'warning: file length is not the same'
        length = min(len(content1), len(content2))
        for i in range(0, length):
            li1 = re.split(r'\s*', content1[i].strip())
            li2 = re.split(r'\s*', content2[i].strip())
            if len(li1) != 3 or len(li2) != 3:
                print 'answer datatype do not match: (%s, %s)' % (li1, li2)
                assert False
            if li1[0] != li2[0] or li1[1] != li2[1]:
                print 'warning: (x,y) not match (%s, %s)' % (li1, li2)

            x.append(float(li1[0]))
            y.append(float(li1[1]))
            if li1[2] == li2[2]:
                color.append(0)
            else:
                color.append(1);
        return x, y, color

    def plot_diff(self, filename1, filename2, filetype):
        if filetype != 'diff':
            self.help()
            return
        x, y, color = self.readin_diff(filename1, filename2)
        self.show_result_diff(x, y, color)

    def help(self):
        print 'usage1: scatter_points.py <filename> origin/answer'
        print 'usage2: scatter_points.py <filename1> <filename2> diff'
        

def main(argv):
    sp = ScatterPoints()
    if len(argv) == 3:
        sp.plot(argv[1], argv[2])
    elif len(argv) == 4:
        sp.plot_diff(argv[1], argv[2], argv[3])
    else:
        sp.help()

if __name__ == '__main__':
    main(sys.argv)
    