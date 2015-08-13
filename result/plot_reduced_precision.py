# -*- coding: utf-8 -*-
from __future__ import division
import re
import sys
import numpy as np
import matplotlib.pyplot as plt

class PlotReducedPrecision(object):
    """docstring for PlotReducedPrecision"""
    def __init__(self, filename, cell_width):
        super(PlotReducedPrecision, self).__init__()
        self.cell_width = cell_width
        self.filename = filename
        self.x = list()
        self.y = list()
        content = open(self.filename).readlines()
        for i in content:
            li = i.strip().split(' ')
            self.x.append(float(li[0]))
            self.y.append(float(li[1]))

    def process_vector(self, vec):
        mid_x = 0.0
        mid_y = 0.0
        for i in vec:
            mid_x = mid_x + self.x[i]
            mid_y = mid_y + self.y[i]
        mid_x = mid_x / len(vec)
        mid_y = mid_y / len(vec)

        diff = []
        for i in vec:
            delta = (self.x[i] - mid_x) * (self.x[i] - mid_x)
            delta = delta + (self.y[i] - mid_y) * (self.y[i] - mid_y)
            diff.append((delta, i))
        diff.sort(reverse = True)
        for i in range(0, len(diff)):
            vec[i] = diff[i][1]

    def reduced_precision(self):
        for key in self.grid:
            self.process_vector(self.grid[key])

    def construct(self):
        min_x = min(self.x)
        min_y = min(self.y)
        max_x = max(self.x)
        max_y = max(self.y)
        n_rows = int((max_x - min_x) / self.cell_width) + 1
        n_cols = int((max_y - min_y) / self.cell_width) + 1

        self.grid = dict()
        for i in range(0, len(self.x)):
            key = int((self.x[i] - min_x) / self.cell_width + 1) * (n_cols + 1) + int((self.y[i] - min_y) / self.cell_width + 1)
            if self.grid.has_key(key):
                self.grid[key].append(i)
            else:
                self.grid[key] = list([i])
    
    def show(self, lim_num):
        color = [0.3] * len(self.x)
        size = [30] * len(self.x)
        for key in self.grid:
            length = len(self.grid[key])
            if length <= lim_num:
                continue
            for i in range(lim_num+1, length):
                which = self.grid[key][i]
                size[which] = 10
                color[which] = 0.6
        plt.scatter(self.x, self.y, c=color, s=size)
        axes = plt.gca()
        axes.xaxis.set_major_locator(plt.MultipleLocator(self.cell_width))
        axes.yaxis.set_major_locator(plt.MultipleLocator(self.cell_width))
        axes.grid(which='major', axis='x', linewidth=0.75, linestyle='-', color='0.75')
        axes.grid(which='major', axis='y', linewidth=0.75, linestyle='-', color='0.75')
        plt.show()


def main():
    prp = PlotReducedPrecision("reduced_precision.data", 10)
    prp.construct()
    prp.reduced_precision()
    prp.show(30)

if __name__ == '__main__':
    main()
