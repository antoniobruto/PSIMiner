import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl

### Set your path to the folder containing the .csv files
PATH = './' # Use your path

### Fetch all files in path
fileNames = os.listdir(PATH)

### Filter file name list for files ending with .csv
fileNames = [file for file in fileNames if '.csv' in file]
name=['x','y']
### Loop over all files
for file in fileNames:
    #mpl.style.use(sty)
    ### Read .csv file and append to list
    df = pd.read_csv(PATH + file)
    x=df.x
    y=df.y
    ### Create line for every file
    plt.plot(x,y)

### Generate the plot
plt.show()
