import json
import matplotlib.pyplot as plt

json_data = open('00v01').read()
jsonToPython = json.loads(json_data)

for i in range(1,22):
    print jsonToPython[i]["move"] + " " + jsonToPython[i]["time"]
    plt.plot(jsonToPython[i]["move"],jsonToPython[i]["time"],'ro')
plt.show()

# jsonData = jsonToPython["data"]
# for item in jsonData:
#     name = item.get("Name")
#     campaignID = item.get("CampaignID")
