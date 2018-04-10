import json
import matplotlib.pyplot as plt
from pylab import figure, axes, pie, title, show

json_data = open('00v01').read()
jsonToPython = json.loads(json_data)

for i in range(1,22):
    print jsonToPython[i]["move"] + " " + jsonToPython[i]["time"]
    plt.plot(jsonToPython[i]["move"],jsonToPython[i]["time"],'ro')
plt.title("NN Gen185 Game 00v01")
plt.xlabel("Generations per evolution")
plt.ylabel("Time it took to evolve")
#plt.savefig("Gen185_00v01.pdf")
plt.show()
