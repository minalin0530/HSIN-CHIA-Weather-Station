import csv
import pandas as pd
import numpy as np

file_txt = "serial.txt"
file_csv = "test.csv"
with open(file_csv, 'w', newline='') as csv_file:
  location = 0
  writer = csv.writer(csv_file)
  writer.writerow(['Hour', 'Minute','Longitude','Latitude','UV index','Light','Temperature','Humidity'])
  with open(file_txt, "r") as f:
    for line in f:
      targ = line[9:11]
      if(targ == 'UV'):
        if(line[20] == ' '):
          uv = int(line[19])
        else:
          uv = int(line[19:21])
      elif(targ == 'Li'):
        light = float(line[16:-3])
      elif(targ == 'Hu'):
        humidity = float(line[19:24])  
      elif(targ == 'Te'):
        temp = float(line[22:27])
      elif(targ == 'Lo'):
        longitude = float(line[20:28])
      elif(targ == 'La'):
        latitude = float(line[19:26])
      elif(targ == 'Ti'):
        if(line[15:17] != "No"):
          hour = float(line[15:17])
          minute = float(line[18:20])
        writer.writerow([hour, minute, longitude, latitude, uv, light, temp, humidity])
      
