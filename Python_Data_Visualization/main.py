import json
import matplotlib.pyplot as plt

json_data = open('../NN_185gens/gen_185/games/00v01').read()
jsonToPython = json.loads(json_data)

print jsonToPython["move"]

# jsonData = jsonToPython["data"]
# for item in jsonData:
#     name = item.get("Name")
#     campaignID = item.get("CampaignID")
