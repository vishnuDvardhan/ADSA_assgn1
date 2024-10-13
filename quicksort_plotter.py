import numpy as np
import matplotlib.pyplot as plt

def read_data(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    l=[]
    k = -1
    i = 0
    while i < len(lines):
        line = int(lines[i].split(":")[1].strip())
        i=i+1
        l.append(line)
        i=i+1
    l=np.array(l)
    l=l.reshape(4,-1)
    return l


   



filename = '/home/vishnu/assignments/quick-sort-output.txt'
k=read_data(filename)
print(k)
avgs=np.int64(np.mean(k,axis=1))
standard_deviations = np.int64(np.std(k,axis=1))

x=["10^4","10^5","10^6","10^7"]
print("averages are ",avgs)
print("standard deviations are",standard_deviations)

plt.bar(x,avgs)
plt.xlabel("arrays sizes")
plt.ylabel("comparisions")
plt.savefig("quick_sort.png")