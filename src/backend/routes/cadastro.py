from flask import Blueprint, request, jsonify
from extensions import db
from models import Cadastro

cadastro_blueprint = Blueprint('cadastros', __name__)

# Rota para adicionar um novo cadastro

@cadastro_blueprint.route('/registrar-cadastro', methods=['POST'])
def create_casdastro():
    data = request.get_json()
    id_users = data.get('id_users')
    id_catracas = data.get('id_catracas')
    
    new_cadastro = Cadastro(id_users=id_users, id_catracas=id_catracas)
    db.session.add(new_cadastro)
    db.session.commit()
    
    return jsonify({
        'message': 'Cadastro criado com sucesso',
        'cadastro': {
            'id': new_cadastro.id,
            'id_users': new_cadastro.id_users,
            'id_catracas': new_cadastro.id_catracas
        }
    }), 201

@cadastro_blueprint.route('/cadastros/<int:cadastro_id>', methods=['DELETE'])
def delete_cadastro(cadastro_id):
    cadastro = Cadastro.query.get(cadastro_id)    
    if cadastro:
        db.session.delete(cadastro)
        db.session.commit()
        return jsonify({'message': 'Cadastro deletado com sucesso'}), 200
    return jsonify({'message': 'Cadastro não encontrado'}), 404