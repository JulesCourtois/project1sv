import numpy as np
import matplotlib.pyplot as pl

data = np.genfromtxt('out.csv', delimiter='\t')

select= np.array([d for d in data if d[0] < 30])
data1= select.transpose()
pl.scatter(0.1*data1[1],data1[0],alpha=0.8, edgecolors='none');
pl.show();

n, bins, patches = pl.hist(0.1*data1[1], 50, normed=0, alpha=0.75)
pl.show();
