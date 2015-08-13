# -*- coding: utf-8 -*-
from __future__ import division
import re
import sys
import numpy as np
import matplotlib.pyplot as plt

class PlotDim(object):
    """docstring for PlotDim"""
    def __init__(self):
        super(PlotDim, self).__init__()
    
    def readin_dim(self, filename):
        x1 = list()
        x2 = list()
        # get 1 point from 10 input points
        # only plot the id1, id2 coordinates of the data
        get = 50
        id1 = 0
        id2 = 1
        counter = 0

        content = open(filename).readlines()
        for i in content:
            counter = counter + 1
            if counter % get == 0:
                li = re.split(r'\s*', i.strip())
                x1.append(float(li[id1]))
                x2.append(float(li[id2]))
        print "input size : %d" % len(x1)
        return x1, x2

    def show_result(self, x, y):
        size = [10] * len(x)
        plt.scatter(x, y, s=size)
        plt.show()

    def show(self, filename):
        x, y = self.readin_dim(filename)
        self.show_result(x, y)

def main():
    pld = PlotDim()
    pld.show("../data/clustered_5M_5D")

if __name__ == '__main__':
    main()
