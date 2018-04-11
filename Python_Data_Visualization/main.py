from subprocess import check_output
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
from os import listdir
import json
import sys

def openSingleFile(file):
    output = check_output("grep { " + file + " | sed -e 's/$/,/'",shell=True)

    jsonToPython = json.loads((b'[' + output + b'{"string":"foo"}' + b']').decode('utf-8'))
    json_size = len(jsonToPython)
    gameCheck = False

    for index in range(1,json_size-1):
        if(jsonToPython[index]["move"] == "00"):
            gameCheck = True
        else:
            print(jsonToPython[index]["move"] + " " + jsonToPython[index]["time"])
            plt.plot(jsonToPython[index]["move"],jsonToPython[index]["time"],'ro')

averageTimeToEvolve = [][]
def openMultipleFiles():
    for cur_file in data_files:
        output = check_output("grep { " + "../NN_185gens/gen_185/games/" + cur_file + " | sed -e 's/$/,/'",shell=True)
        jsonToPython = json.loads((b'[' + output + b'{"string":"foo"}' + b']').decode('utf-8'))

        json_size = len(jsonToPython)
        for i in range(1,json_size-1):
            print(jsonToPython[i]["move"] + " " + jsonToPython[i]["time"])

            #plt.plot(jsonToPython[i]["move"],jsonToPython[i]["time"],'ro')

def main():

    data_files = []
    # Open files in directory
    for list_files in listdir('../NN_185gens/gen_185/games'):
        data_files.append(list_files)

    if(len(sys.argv)==2):
        openSingleFile(sys.argv[1])
    else:
        openMultipleFiles()

    plt.title("NN Gen185 Game")
    # plt.xlabel("Generations per evolution")
    # plt.ylabel("Time it took to evolve")
    #plt.savefig("Gen185_00v01.pdf")
    style.use('fivethirtyeight')
    plt.show()

if __name__ == '__main__':
    main()
