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


@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('publish')
def handle_publish(json_str):
    data = json.loads(json_str)
    mqtt.publish(data['topic'], data['message'], data['qos'])

@socketio.on('experiment_toggle')
def handle_publish(json_str):
    # data = json.loads(json_str)
    print("this works??")
    mqtt.publish("lab/control/experimentToggle", json_str, qos)

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
    print(message.payload.decode())
    # print(payload)
    payload_dict = json.loads(message.payload.decode())
    print(payload_dict["msgType"])
    print(type(payload_dict["msgType"]))
    if payload_dict["msgType"] == "data":
        print("inside if")
        file_name = payload_dict["id"]+"_"+"123"+"_"+"date"+".txt"
        print(file_name)
        file_descriptor = open(file_name,"a")
        file_descriptor.write(message.payload.decode())
        file_descriptor.write("\n")
        
        file_descriptor.close()
    socketio.emit('mqtt_message', data=data)


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    # print(level, buf)
    pass

# @mqtt.on_topic("a")
# def handle_a(client, userdata, message):
#     # create log file and start recording
#     print('Received message on topic {}: {}'
#           .format(message.topic, message.payload.decode()))
    

@mqtt.on_topic("lab/control/experimentToggle")
def handle_experimentToggle(client, userdata, message):
    # create log file and start recording
    print('Received message on topic {}: {}'
          .format(message.topic, message.payload.decode()))
    received_payload = message.payload.decode()
    print('                        line102')
    print(received_payload)
    received_payload = json.loads(received_payload)
    date = "date"; # replace with actual time
    device_id = received_payload['id']
    experiment_counter = "123" # replace 
    new_file = open(device_id + "_" + experiment_counter + "_" + date + ".txt",'x')
    new_file.close()

if __name__ == '__main__':
    mqtt.subscribe("lab/control/experimentToggle", qos)
    socketio.run(app, host='192.168.4.1', port=5000, use_reloader=False, debug=True)
