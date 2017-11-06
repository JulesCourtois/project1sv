from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import matplotlib.pyplot as plt

datas = np.genfromtxt("out.csv", delimiter = ',')

pp = PdfPages('graphics.pdf')


select = np.array([data for data in datas if data[0] <= 30])
select = select.transpose()

plt.figure(1)
plt.scatter(select[1], select[0], alpha=0.8, edgecolors='none')
plt.xlim([0, 1000])
plt.ylim([0, 30])
plt.xlabel('time (ms)', fontsize = 13)
plt.ylabel('spikes', fontsize = 13)
pp.savefig()

plt.figure(2)
select2 = np.array([data for data in datas])
select2 = select2.transpose()
n, bins, patches = plt.hist(select2[1], 50, normed=0, alpha=0.75)
pp.savefig()

pp.close()
