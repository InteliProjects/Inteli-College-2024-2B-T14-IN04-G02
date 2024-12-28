from flask import Blueprint, request, jsonify
from extensions import db
from models import BiometricRequest
import paho.mqtt.client as mqtt
from mqtt_client import mqtt_client, command_topic

biometric_request_blueprint = Blueprint('biometric-request', __name__)

@biometric_request_blueprint.route("/start-biometric-registration", methods=["POST"])

def start_biometric_registration():
    try:
        data = request.json
        user_id = data.get("user_id")
        turnstile_id = data.get("turnstile_id")

        if not user_id or not turnstile_id:
            return jsonify({"error": "user_id e turnstile_id são obrigatórios"}), 400

        # Gera um request_id único (UUID)
        import uuid
        request_id = str(uuid.uuid4())

        # Cria uma nova entrada na tabela BiometricRequest
        new_request = BiometricRequest(
            request_id=request_id,
            user_id=user_id,
            turnstile_id=turnstile_id,
            status='pending'
        )
        db.session.add(new_request)
        db.session.commit()

        # Publica comando para iniciar o cadastro da digital
        command = {
            "request_id": request_id,
            "turnstile_id": turnstile_id,
            "mode": "register"
        }
        mqtt_client.publish(command_topic, command)

        return jsonify({"message": "Comando enviado para iniciar cadastro de digital", "request_id": request_id}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500
