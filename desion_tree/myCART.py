#!/usr/bin/python
#coding = utf-8


def loadData(path):
    fr = open(path)
    dataSet = []
    for line in fr.readlines():
        lineData = []
        label = line[0]
        lineStr = line[2:].split(',')
        for k in lineStr:
            lineData.append(float(k))
        lineData.append(label)
        dataSet.append(lineData)
    return dataSet

def impurity(dataSet):
    setLen = len(dataSet)
    if setLen <= 1:
	return 0
    IndexSet = set(dataSet[:][-1])
    dataLen = len(dataSet) - 1
    errSum = 0
    for index in IndexSet:
        err = 0
        for i in xrange(dataLen):
            if dataSet[i][-1] == index:
                err += 1
        err /= float(dataLen)
        err **= 2
        errSum += err
    return (1 - errSum)

def binSplitDataSet(dataSet, feature, value):
    left = []
    right = []
    for data in dataSet:
        if data[feature] <= value:
            data_ = data[:feature]
            data_.extend(data[feature + 1:])
            left.append(data_)
        else:
            data_ = data[:feature]
            data_.extend(data[feature + 1:])
            right.append(data_)
    return left, right



def chooseBestFeature(dataSet):
    SetLen = len(dataSet)
    dataLen = len(dataSet[0]) - 1
    if SetLen == 1:
        return None, None
    if impurity(dataSet) == 0:
        return None, None
    bestFeature = 0
    bestValue = dataSet[0][0]
    bestImpurity = 3
    bestleft = []
    bestright = []
    for i in xrange(dataLen):
        for j in xrange(SetLen):
            print 'cbf: i = ', i, 'j = ', j
            left, right = binSplitDataSet(dataSet, i, dataSet[j][i])
            cutImpurity = impurity(left) + impurity(right)
            if cutImpurity <= bestImpurity:
                bestFeature = i
                bestValue = dataSet[j][i]
                bestleft = left
                bestright = right
    print 'cbf:', bestFeature, bestValue
    return bestFeature, bestValue, bestleft, bestright


def createTree(dataSet, height = 0, maxHeight = 5):
    tree = {}
    print 'height = ', height + 1
    feature, value, leftdata, rightdata = chooseBestFeature(dataSet)
    if feature == None:
        return None
    if height >= maxHeight:
        return feature
    left = createTree(leftdata, height + 1, maxHeight / 2)
    right = createTree(rightdata, height + 1, maxHeight / 2)
    llen = len(leftdata)
    rlen = len(rightdata)
    if type(left).__name__ != 'dict':
        if llen == 1:
            left = dataSet[0][-1]
    if type(right).__name__ != 'dict':
        if rlen == 1:
            right = dataSet[0][-1]
    subtree = [value, left, right]
    tree[feature] = subtree
    return tree



if __name__ == '__main__':
    dataSet = loadData('./letter-recognition.data')
    print createTree(dataSet)
