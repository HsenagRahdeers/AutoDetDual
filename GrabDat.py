import serial
import csv
from serial.tools import list_ports

def find_arduino_port():
    arduino_ports = [
        p.device
        for p in list_ports.comports()
        if 'Arduino' in p.description 
    ]
    return arduino_ports[0] if arduino_ports else None

port = find_arduino_port()

if port is None:
    print("Arduino not found on any COM ports.")
else:
    print(f"Using port: {port}")
    ser = serial.Serial(port, 115200)

    conc = input("Enter Concentration: ")
    name = conc + '_Percent_Solution.csv'

    ser.write(b'2')
    csv_name = name
    csv_file = open(csv_name, 'w', newline='')
    csv_writer = csv.writer(csv_file)

    try:
        for x in range(2654):  
            data = ser.readline().decode().strip()
            print(data)
            csv_writer.writerow([data])
        else:
            print("Done Recording!")
            print("Data Written!")
            ser.close()
            csv_file.close()
    except KeyboardInterrupt:
        ser.close()
        csv_file.close()
