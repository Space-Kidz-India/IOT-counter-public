
import csv
import serial
ser = serial.Serial("COM11", 9600)

def parse_data(raw_data):
    reader = csv.reader(raw_data.split('\n'), delimiter=',')
    data = []
    for row in reader:
        for values in row:
            data.append(values)
    return data

while True:
     cc=str(ser.readline())
     raw_data = cc[2:][:-5]
     
#raw_data = "34,245,345,36,34,34"
final_data = parse_data(raw_data)
cnt = 0
for values in final_data:
    cnt += 1
    print(f"channel {cnt}: {values}")
print(final_data)