import matplotlib.pyplot as plt
import csv

x = []
y = []

with open('normal.csv','r') as csvfile:
   plots = csv.reader(csvfile, delimiter=',')
   for row in plots:
      #print (float(row[0]))
      x.append(float(row[0]))
      y.append(float(row[1]))

plt.plot(x,y, label='Loaded from file!')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Interesting Graph\nCheck it out')
plt.legend()
plt.show()
