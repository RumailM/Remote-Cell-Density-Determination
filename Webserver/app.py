"""

A small Test application to show how to use Flask-MQTT.

"""
import logging

import eventlet
import json
from flask import Flask, render_template
from flask_mqtt import Mqtt
from flask_socketio import SocketIO
from flask_bootstrap import Bootstrap

eventlet.monkey_patch()

app = Flask(__name__)
app.config['SECRET'] = 'my secret key'
app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['MQTT_BROKER_URL'] = 'localhost'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_CLIENT_ID'] = 'flask_mqtt'
app.config['MQTT_CLEAN_SESSION'] = True
app.config['MQTT_USERNAME'] = 'smart'
app.config['MQTT_PASSWORD'] = 'clamp'
app.config['MQTT_KEEPALIVE'] = 5
app.config['MQTT_TLS_ENABLED'] = False
app.config['MQTT_LAST_WILL_TOPIC'] = 'home/lastwill'
app.config['MQTT_LAST_WILL_MESSAGE'] = 'bye'
app.config['MQTT_LAST_WILL_QOS'] = 2

# Parameters for SSL enabled
# app.config['MQTT_BROKER_PORT'] = 8883
# app.config['MQTT_TLS_ENABLED'] = True
# app.config['MQTT_TLS_INSECURE'] = True
# app.config['MQTT_TLS_CA_CERTS'] = 'ca.crt'

mqtt = Mqtt(app)
socketio = SocketIO(app)
bootstrap = Bootstrap(app)

qos = 2;


#Parameters
num_devices = 8;

mac_addr_list = [None]*num_devices;
experiment_start_time_list = [None]*num_devices;

def push_mac(mac_str):
    #Adds MAC Address and returns device ID, call when device logs in
    id = -99;
    for i in range(num_devices):
        if i is None:
            mac_addr_list[i] = mac_str
            id = i
            break
    if id == -99:
        print("Exceeded max Number of Connected Devices")
    else:
        print("Succesfully cached MAC Address " + mac_str + " at id " + str(id))
    return id

def pop_mac(mac_str):
    # Removes MAC Adress and frees up an ID, call when device logs out
    for i in range(num_devices):
        if mac_addr_list[i] == mac_str:
            mac_addr_list[i] = None
            print("Removed MAC Address at id " + str(i))


@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('publish')
def handle_publish(json_str):
    data = json.loads(json_str)
    mqtt.publish(data['topic'], data['message'], data['qos'])

@socketio.on('experiment_toggle')
def handle_publish(json_str):
    data = json.loads(json_str)
    print("this works??")
    
    temp_dict = {"id":data["id"],"msgType":"control","toggle":"1"}
    json_str_temp = json.dumps(temp_dict)
    mqtt.publish("lab/control/experimentToggle", json_str_temp, qos)

@socketio.on('publish_to_a')
def handle_publish(json_str):
    # data = json.loads(json_str)
    # print("this works??")
    mqtt.publish("a", json_str, qos)


@socketio.on('subscribe')
def handle_subscribe(json_str):
    data = json.loads(json_str)
    mqtt.subscribe(data['topic'], data['qos'])


@socketio.on('unsubscribe_all')
def handle_unsubscribe_all():
    mqtt.unsubscribe_all()


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    print("inside mqtt.on_message")
    print(message.payload.decode())
    data = dict(
        topic=message.topic,
        payload=message.payload.decode(),
        qos=message.qos,
    )
    socketio.emit('mqtt_message', data=data)


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    # print(level, buf)
    pass


@mqtt.on_topic("lab/data")
    def handle_data(client, userdata, message):
        
        print('Received message on topic {}: {}'
          .format(message.topic, message.payload.decode()))

        print(message.payload.decode())
        # print(payload)
        payload_dict = json.loads(message.payload.decode())
        print(payload_dict["msgType"])
        print(type(payload_dict["msgType"]))
        if payload_dict["msgType"] == "data":
            print("inside if")
            file_name = mac_addr_list[payload_dict["id"]]+"_"+"date"+".txt"
            print(file_name)
            file_descriptor = open(file_name,"a")
            file_descriptor.write(message.payload.decode())
            file_descriptor.write("\n")
            file_descriptor.close()

@mqtt.on_topic("lab/control/login")
    def handle_login(client, userdata, message):

        print('Received message on topic {}: {}'
          .format(message.topic, message.payload.decode()))
        
        payload_dict = json.loads(message.payload.decode())
        ret_id = push_mac(payload_dict["MAC"])
        temp_dict = {"MAC":payload_dict,"msgType":"control","id":ret_id}
        json_str = json.dumps(temp_dict)
        mqtt.publish("lab/control/login", json_str, qos)

@mqtt.on_topic("lab/control/logout")
    def handle_login(client, userdata, message):

        print('Received message on topic {}: {}'
          .format(message.topic, message.payload.decode()))
        
        payload_dict = json.loads(message.payload.decode())
        pop_mac(payload_dict["MAC"])
        

@mqtt.on_topic("lab/control/experimentToggle")
def handle_experimentToggle(client, userdata, message):
    # create log file and start recording

    print('Received message on topic {}: {}'
          .format(message.topic, message.payload.decode()))

    received_payload = message.payload.decode()
    print('                        line102')
    print(received_payload)
    received_payload = json.loads(received_payload)
    device_mac = mac_addr_list[received_payload['id']]
    new_file = open(device_mac + "_" + date + ".txt",'x')
    new_file.close()

if __name__ == '__main__':
    mqtt.subscribe("lab/control/login", qos)
    mqtt.subscribe("lab/control/logout", qos)
    mqtt.subscribe("lab/data", qos)
    mqtt.subscribe("lab/control/experimentToggle", qos)
    socketio.run(app, host='192.168.0.110', port=5000, use_reloader=False, debug=True)
