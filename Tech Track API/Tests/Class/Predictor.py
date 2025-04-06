#see how much error is in the prediction from the predictor class
from math import cos, sin
import requests
import json

coeff = [1, 2, 3, 4]
times = [11, 12, 13, 14]
data = []
for time in times:
    data.append([time,  coeff[0] * sin( time * coeff[1]) + coeff[2] * cos(time * coeff[3])])

res = requests.get('http://localhost:8008/data/technology/Python')

if res.status_code != 200:
    print("Error: get_data_technology()")
    exit(1)

res_data = json.loads(res.text)
res_data["predicted values"]
diff = []
for i in range(len(data)):
    diff.append(abs(data[i][1] - res_data["predicted values"][i]))
print("Diff: ", diff)
print("Diff sum: ", sum(diff))
print("Diff avg: ", sum(diff) / len(diff))

