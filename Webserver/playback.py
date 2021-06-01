"""
Python Script to publish saved JSON text file dumps as MQTT messages
"""
import paho.mqtt.client as paho
import time

broker = "192.168.0.114"
port = 1883
topic = "lab/data"
filename = "MF_1_30C_0.5_300.txt"
client = paho.Client("playback")

if __name__ == "__main__":
    client.username_pw_set(username="smart", password="clamp")
    client.connect(broker,port)
    client.loop_start()
    f = open(filename, "r")
    while True:
        time.sleep(0.1)
        line = f.readline()[:-1]
        if (line == ""):
            break
        else:
            client.publish(topic, line)
            print("Published!")
    f.close()