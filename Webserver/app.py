"""
Flask-MQTT Webserver for Raspberry Pi remote processing unit
"""
import logging
import eventlet
import json
from flask import Flask, render_template
from flask_mqtt import Mqtt
from flask_socketio import SocketIO
from flask_bootstrap import Bootstrap
from datetime import datetime
from dataclasses import dataclass

@dataclass
class Clamp():
    mac_addr: str = None
    experiment_name: str = None
    experiment_start_time: str = None
    experiment_mode: str = None

eventlet.monkey_patch()

app = Flask(__name__)
app.config["SECRET"] = "my secret key"
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.config["MQTT_BROKER_URL"] = "localhost"
app.config["MQTT_BROKER_PORT"] = 1883
app.config["MQTT_CLIENT_ID"] = "flask_mqtt"
app.config["MQTT_CLEAN_SESSION"] = True
app.config["MQTT_USERNAME"] = "smart"
app.config["MQTT_PASSWORD"] = "clamp"
app.config["MQTT_KEEPALIVE"] = 5
app.config["MQTT_TLS_ENABLED"] = False
app.config["MQTT_LAST_WILL_TOPIC"] = "home/lastwill"
app.config["MQTT_LAST_WILL_MESSAGE"] = "bye"
app.config["MQTT_LAST_WILL_QOS"] = 2

mqtt = Mqtt(app)
socketio = SocketIO(app)
bootstrap = Bootstrap(app)
qos = 0

#Parameters
clamp_list = []

def push_mac(mac_str):
    #Adds MAC Address and returns device ID, call when device logs in
    id = 0
    isPushed = False
    for i in range(len(clamp_list)):
        if clamp_list[i].mac_addr==mac_str:
            isPushed = True
            id = i
            print("Existing cached MAC Address " + mac_str + " found at id " + str(id))
            break
    if not isPushed:
        clamp_list.append(Clamp(mac_addr=mac_str))
        id = len(clamp_list) - 1
        print("Succesfully cached MAC Address " + mac_str + " at id " + str(id))
    
    return id

def pop_mac(mac_str):
    # Removes MAC Adress and frees up an ID, call when device logs out
    for i in range(len(clamp_list)):
        if clamp_list[i].mac_addr == mac_str:
            clamp_list.pop(i)
            print("Removed MAC Address at id " + str(i))

@app.route("/")
def index():
    return render_template("index.html")

@socketio.on("publish")
def handle_publish(json_str):
    data = json.loads(json_str)
    mqtt.publish(data["topic"], data["message"], data["qos"])

@socketio.on("experimentStart")
def handle_experimentStart(json_str):   
    mqtt.publish("lab/control/experimentStart", json_str, qos)

@socketio.on("experimentStop")
def handle_experimentStop(json_str):
    mqtt.publish("lab/control/experimentStop", json_str, qos)

@socketio.on("subscribe")
def handle_subscribe(json_str):
    data = json.loads(json_str)
    mqtt.subscribe(data["topic"], data["qos"])

@socketio.on("unsubscribe_all")
def handle_unsubscribe_all():
    mqtt.unsubscribe_all()

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    print("Published message on topic {}: {}"
        .format(message.topic, message.payload.decode()))
    data = dict(
        topic=message.topic,
        payload=message.payload.decode(),
        qos=message.qos,
    )
    socketio.emit("mqtt_message", data=data)

@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    # print(level, buf)
    pass

@mqtt.on_topic("lab/data")
def handle_data(client, userdata, message):
    
    print("Received message on topic {}: {}"
        .format(message.topic, message.payload.decode()))

    payload_dict = json.loads(message.payload.decode())
    date = clamp_list[int(payload_dict["ID"])].experiment_start_time
    
    file_name = (clamp_list[int(payload_dict["ID"])].experiment_name + "_" +
                clamp_list[int(payload_dict["ID"])].experiment_agc_freq + "_" +
                clamp_list[int(payload_dict["ID"])].experiment_mode + "_" +
                clamp_list[int(payload_dict["ID"])].mac_addr + "_" +
                date + ".txt")

    file_descriptor = open(file_name,"a")
    file_descriptor.write(message.payload.decode())
    file_descriptor.write("\n")
    file_descriptor.close()

    data = dict(
        topic=message.topic,
        payload=message.payload.decode(),
        qos=message.qos,
    )
    socketio.emit("mqtt_message", data=data)

@mqtt.on_topic("lab/control/login")
def handle_login(client, userdata, message):

    print("Received message on topic {}: {}"
        .format(message.topic, message.payload.decode()))

    payload_dict = json.loads(message.payload.decode())
    ret_id = push_mac(payload_dict["MAC"])
    temp_dict = {"MAC":payload_dict["MAC"],"ID":ret_id}
    json_str = json.dumps(temp_dict)
    mqtt.publish("lab/control/loginResponse", json_str, qos)

@mqtt.on_topic("lab/control/logout")
def _login(client, userdata, message):

    print("Received message on topic {}: {}"
        .format(message.topic, message.payload.decode()))
    
    payload_dict = json.loads(message.payload.decode())
    pop_mac(payload_dict["MAC"])
        

@mqtt.on_topic("lab/control/experimentStart")
def handle_experimentStart(client, userdata, message):
    # create log file and start recording

    print("Received message on topic {}: {}"
          .format(message.topic, message.payload.decode()))

    received_payload = message.payload.decode()

    received_payload = json.loads(received_payload)
    now = datetime.now()
    date = now.strftime("%d_%m_%Y_%H_%M_%S")
    clamp_list[int(received_payload["ID"])].experiment_start_time = date
    device_mac = clamp_list[int(received_payload["ID"])].mac_addr
    clamp_list[int(received_payload["ID"])].experiment_mode = str(received_payload["MODE"])
    clamp_list[int(received_payload["ID"])].experiment_name = str(received_payload["experimentName"])
    experiment_name = clamp_list[int(received_payload["ID"])].experiment_name
    new_file = open(
                clamp_list[int(received_payload["ID"])].experiment_name + "_" +
                clamp_list[int(received_payload["ID"])].experiment_mode + "_" +
                date + ".txt","x")
    new_file.close()

if __name__ == "__main__":
    # Clearing Retained Messages
    mqtt.publish("lab/control/login", payload=None, qos=qos, retain=True)
    mqtt.publish("lab/control/loginResponse", payload=None, qos=qos, retain=True)
    mqtt.publish("lab/control/logout", payload=None, qos=qos, retain=True)
    mqtt.publish("lab/data", payload=None, qos=qos, retain=True)
    mqtt.publish("lab/control/experimentStart", payload=None, qos=qos, retain=True)
    mqtt.publish("lab/control/experimentStop", payload=None, qos=qos, retain=True)

    # Subscribing to relevant MQTT Topics
    mqtt.subscribe("lab/control/login", qos)
    mqtt.subscribe("lab/control/logout", qos)
    mqtt.subscribe("lab/data", qos)
    mqtt.subscribe("lab/control/experimentStart", qos)
    mqtt.subscribe("lab/control/experimentStop", qos)

    socketio.run(app, host="0.0.0.0", port=5000, use_reloader=False, debug=True)