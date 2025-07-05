# -*- coding: utf-8 -*-
"""
Created on Thu Oct 31 14:26:21 2024

@author: bwehlin
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import collections  as mc

def plotDomain(xPath, yPath, ax=None, pointsize=0.1, linewidth=0.4):
    
    if ax is None:
        fig, ax = plt.subplots(figsize=(10, 3))
        
    X = np.loadtxt(xPath, delimiter=' ')
    Y = np.loadtxt(yPath, delimiter=' ')
    
    nI = X.shape[0] - 2
    nJ = X.shape[1] - 2
    
    hlines = [[(X[i, j], Y[i, j]), (X[i+1, j], Y[i+1, j])] for i in range(nI+1) for j in range(nJ+2)]
    vlines = [[(X[i, j], Y[i, j]), (X[i, j+1], Y[i, j+1])] for i in range(nI+2) for j in range(nJ+1)]
    
    X = X.reshape(-1, 1)
    Y = Y.reshape(-1, 1)
    
    ax.scatter(X, Y, s=pointsize)

    hlc = mc.LineCollection(hlines, linewidths=linewidth)
    vlc = mc.LineCollection(vlines, linewidths=linewidth)
    ax.add_collection(hlc)
    ax.add_collection(vlc)

    ax.axis('equal')
    
    return ax

xPath = 'x1.txt'
yPath = 'y1.txt'

ax = plotDomain(xPath, yPath)

plt.show()
