import numpy as np
from sklearn.decomposition import PCA
import os

class PCAData(object):
    """docstring for PCAData"""
    def __init__(self):
        super(PCAData, self).__init__()
        self.attr = 52;
        self.length = 3850505
        self.data = np.empty((self.length, self.attr), np.float32)
        self.index = 0
        self.NaNCounter = 0

    def readfile(self, filename):
        print 'processing %s' % filename
        content = open(filename).readlines()
        for i in content:
            li = i.strip().split(' ')[2:]
            for j in range(0, self.attr):
                item = float(li[j])
                if np.isnan(item):
                    item = 0
                    self.NaNCounter = self.NaNCounter + 1
                self.data[self.index][j] = item
            self.index = self.index + 1
        print self.index
        print self.NaNCounter
    
    def walking(self, dir_name):
        for root, dirs, files in os.walk(dir_name):
            for name in files:
                self.readfile(dir_name + os.sep + name)

    def pca_run(self):
        print 'running PCA'
        pca = PCA(n_components=4, copy=False)
        newData = pca.fit_transform(self.data)
        print 'saving answer'
        np.savetxt("answer.txt", newData, fmt="%f")

    def run(self):
        self.walking('Optional')
        self.walking('Protocol')
        self.pca_run()

def main():
    pcad = PCAData()
    pcad.run()

if __name__ == '__main__':
    main()

