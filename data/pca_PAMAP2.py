import numpy as np

class PCAData(object):
    """docstring for PCAData"""
    def __init__(self):
        super(PCAData, self).__init__()
        self.attr = 52;
        self.length = 100000
        self.data = np.empty((self.length, self.attr), np.float32)
        self.index = 0

    def readfile(self, filename):
        content = open(filename).readlines()
        for i in content:
            li = i.strip().split(' ')[2:]
            for j in range(0, self.attr):
                self.data[self.index][j] = float(li[j])
            self.index = self.index + 1
        
def main():
    pcad = PCAData()
    pcad.

if __name__ == '__main__':
    main()