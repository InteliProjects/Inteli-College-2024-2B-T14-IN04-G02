from flask import Blueprint, jsonify, request

screen1_blueprint = Blueprint('screen1', __name__)

@screen1_blueprint.route('/', methods=['GET'])
def get_data():
    # Lógica para recuperar dados da Tela 1
    return jsonify({"message": "Dados da Tela 1"})

@screen1_blueprint.route('/', methods=['POST'])
def post_data():
    # Lógica para processar dados recebidos da Tela 1
    data = request.json
    return jsonify({"message": "Dados recebidos da Tela 1", "data": data})
