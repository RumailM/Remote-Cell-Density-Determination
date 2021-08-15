
import paho.mqtt.client as paho
import time

filename = "sim_data.txt"


broker = "127.0.0.1"
port = 1883
client = paho.Client("playback")
client.username_pw_set(username="smart", password="clamp")
client.connect(broker,port)
client.loop_start()


payload_login = "{\"MAC\": \"AA:AA:AA:AA:AA\"}"

#payload_data = "{\"ID\":0,\"TIME\":3249971,\"GAIN\":8,\"ATIME\":59,\"ASTEP\":599,\"DATA\":[268,2408,14168,1731,6182,657]}"

payload_start = "{\"CLR\":\"1\",\"MODE\":\"2\",\"ID\":\"0\",\"WKE\":\"10\",\"SLP\":\"50\",\"experimentName\":\"sim\"}"



topic_login = "lab/control/login"
topic_data = "lab/data"
topic_start = "lab/control/experimentStart"

client.publish(topic_login, payload_login)
time.sleep(1)
#client.publish(topic_start, payload_start)
#time.sleep(1)
input("press enter once start/stop button has been pressed")

    
f = open(filename, "r")
while True:
    time.sleep(0.25)
    line = f.readline()[:-1]
    if (line == ""):
        break
    else:
        client.publish(topic_data, line)
        print("Published!")
f.close()

    
   
