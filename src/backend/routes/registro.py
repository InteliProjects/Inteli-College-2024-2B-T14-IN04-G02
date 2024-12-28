from flask import Blueprint, request, jsonify
from models import Registro
from extensions import db

registro_blueprint = Blueprint('registro', __name__)

@registro_blueprint.route('/registros', methods=['POST'])
def create_registro():
    data = request.get_json()
    novo_registro = Registro(
        id_users=data.get('id_users'),
        acesso=data.get('acesso'),
        saida=data.get('saida'),
        id_catracas=data.get('id_catracas')
    )
    db.session.add(novo_registro)
    db.session.commit()
    return jsonify({'message': 'Registro criado com sucesso'}), 201

@registro_blueprint.route('/registros', methods=['GET'])
def get_registros():
    registros = Registro.query.all()
    registros_list = []
    for registro in registros:
        registros_list.append({
            'id': registro.id,
            'id_users': registro.id_users,
            'acesso': registro.acesso,
            'saida': registro.saida,
            'id_catracas': registro.id_catracas
        })
    return jsonify(registros_list), 200

@registro_blueprint.route('/registros/<int:id>', methods=['GET'])
def get_registro(id):
    registro = Registro.query.get_or_404(id)
    return jsonify({
        'id': registro.id,
        'id_users': registro.id_users,
        'acesso': registro.acesso,
        'saida': registro.saida,
        'id_catracas': registro.id_catracas
    }), 200

@registro_blueprint.route('/registros/<int:id>', methods=['PUT'])
def update_registro(id):
    registro = Registro.query.get_or_404(id)
    data = request.get_json()
    registro.id_users = data.get('id_users', registro.id_users)
    registro.acesso = data.get('acesso', registro.acesso)
    registro.saida = data.get('saida', registro.saida)
    registro.id_catracas = data.get('id_catracas', registro.id_catracas)
    db.session.commit()
    return jsonify({'message': 'Registro atualizado com sucesso'}), 200

@registro_blueprint.route('/registros/<int:id>', methods=['DELETE'])
def delete_registro(id):
    registro = Registro.query.get_or_404(id)
    db.session.delete(registro)
    db.session.commit()
    return jsonify({'message': 'Registro excluído com sucesso'}), 200