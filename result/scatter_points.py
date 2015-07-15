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
    
    def plot(self, filename, filetype):
        x, y, color = self.readin(filename, filetype)
        self.show_result(x, y, color)

def main(argv):
    if len(argv) != 3:
        print 'usage: scatter_points.py <filename> <origin/answer>'
        return

    sp = ScatterPoints()
    sp.plot(argv[1], argv[2])

if __name__ == '__main__':
    main(sys.argv)
    