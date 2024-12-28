from routes.screen1 import screen1_blueprint
from routes.user import user_blueprint
from routes.responsavel import responsavel_blueprint
from routes.adm import adm_blueprint  
from routes.catraca import catraca_blueprint
from routes.cadastro import cadastro_blueprint
from routes.registro import registro_blueprint
from routes.registro_biometrico import biometric_request_blueprint
from dotenv import load_dotenv
import os
from flask import Flask, request, jsonify
from extensions import db
import logging
from mqtt_client import init_mqtt, mqtt_client
from flask_cors import CORS

logging.basicConfig(level=logging.DEBUG)

load_dotenv()  # Carrega variáveis do arquivo .env

def create_app():
    app = Flask(__name__)
    
    # Configuração do CORS
    CORS(app, 
         origins=["http://localhost:3000"],
         supports_credentials=True,
         methods=["GET", "POST", "PUT", "DELETE", "OPTIONS"],
         allow_headers=["Content-Type", "Authorization"])
    
    # Configuração do URI do banco de dados usando a variável de ambiente
    app.config['SQLALCHEMY_DATABASE_URI'] = os.getenv('DATABASE_URL')
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

    db.init_app(app)
    
    # inicializa o cliente MQTT
    init_mqtt(app)

    with app.app_context():
        import models  # Importa as definições dos modelos
        db.create_all()  # Cria as tabelas no banco de dados

    # Registro de Blueprints
    app.register_blueprint(screen1_blueprint, url_prefix='/api/screen1')
    app.register_blueprint(user_blueprint, url_prefix='/api/users')
    app.register_blueprint(responsavel_blueprint, url_prefix='/api/responsaveis')
    app.register_blueprint(adm_blueprint, url_prefix='/api/adms')
    app.register_blueprint(biometric_request_blueprint, url_prefix='/api/registro-biometrico')
    app.register_blueprint(catraca_blueprint, url_prefix='/api/catraca')
    app.register_blueprint(cadastro_blueprint, url_prefix='/api/cadastro')
    app.register_blueprint(registro_blueprint, url_prefix='/api/registro')

    return app

if __name__ == '__main__':
    app = create_app()
    app.run(host='0.0.0.0', port=5000, debug=True)
