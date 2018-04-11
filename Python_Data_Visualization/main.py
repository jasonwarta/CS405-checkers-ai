from subprocess import check_output
import matplotlib.pyplot as plt
from os import listdir
import json
import sys

countFiles = 1;
files = []

# Open files in directory
for list_files in listdir('../NN_185gens/gen_185/games'):
    files[countFiles] = list_files
    countFiles = countFiles + 1

file = sys.argv[1]
output = check_output("grep { " + file + " | sed -e 's/$/,/'",shell=True)
# json_data = (open('00v01').read())

jsonToPython = json.loads(b'[' + output + b'{"string":"foo"}' + b']')
json_size = len(jsonToPython)

for i in range(1,json_size-1):
    print(jsonToPython[i]["move"] + " " + jsonToPython[i]["time"])
    plt.plot(jsonToPython[i]["move"],jsonToPython[i]["time"],'ro')

# plt.title("NN Gen185 Game 00v01")
# plt.xlabel("Generations per evolution")
# plt.ylabel("Time it took to evolve")
#plt.savefig("Gen185_00v01.pdf")
plt.show()
