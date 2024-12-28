import json
import paho.mqtt.client as mqtt
from threading import Thread
from flask_sqlalchemy import SQLAlchemy
from extensions import db
from models import BiometricRequest

# Configurações do MQTT
broker = "broker.emqx.io"
port = 1883
command_topic = "a-p0rt4l/command"
response_topic = "a-p0rt4l/callback"

class MQTTClient:
    def __init__(self, app):
        self.app = app
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def start(self):
        try:
            self.client.connect(broker, port)
            self.client.loop_start()
            print("Cliente MQTT iniciado e conectado ao broker.")
        except Exception as e:
            print(f"Erro ao iniciar o cliente MQTT: {e}")

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("Conectado ao broker MQTT com sucesso!")
            client.subscribe(response_topic)
            print(f"Inscrito no tópico: {response_topic}")
        else:
            print(f"Erro ao conectar ao broker MQTT. Código: {rc}")

    def on_message(self, client, userdata, msg):
        print(f"Mensagem recebida no tópico {msg.topic}: {msg.payload.decode('utf-8')}")
        with self.app.app_context():
            try:
                # Decodifica e processa a mensagem recebida
                dados = json.loads(msg.payload.decode('utf-8'))
                request_id = dados.get("request_id")
                fingerprint_id = dados.get("fingerprint_id")
                status = dados.get("status")

                # Atualiza o status no banco de dados
                biometric_request = BiometricRequest.query.filter_by(request_id=request_id).first()
                if biometric_request:
                    biometric_request.status = status
                    db.session.commit()
                    print(f"Status atualizado para a requisição {request_id}: {status}")
                else:
                    print(f"Requisição {request_id} não encontrada no banco de dados.")
            except Exception as e:
                print(f"Erro ao processar mensagem: {e}")

    def publish(self, topic, message):
        try:
            self.client.publish(topic, json.dumps(message))
            print(f"Mensagem publicada no tópico {topic}: {message}")
        except Exception as e:
            print(f"Erro ao publicar mensagem no tópico {topic}: {e}")

# Inicializa o MQTTClient
mqtt_client = None

def init_mqtt(app):
    global mqtt_client
    mqtt_client = MQTTClient(app)
    mqtt_thread = Thread(target=mqtt_client.start)
    mqtt_thread.daemon = True
    mqtt_thread.start()
    print("Cliente MQTT inicializado e rodando em um thread separado.")
