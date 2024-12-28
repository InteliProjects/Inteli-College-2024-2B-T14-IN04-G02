from flask import Blueprint, request, jsonify
from extensions import db
from models import Responsavel

responsavel_blueprint = Blueprint('responsavel', __name__)

# Rota para criar um novo responsável
@responsavel_blueprint.route('/registrar-responsavel', methods=['POST'])
def create_responsavel():
    data = request.get_json()
    name = data.get('name')
    cpf = data.get('cpf')
    telefone = data.get('telefone')
    id_users = data.get('user_id')
    
    # Verifica se o CPF já está cadastrado
    existing_responsavel = Responsavel.query.filter_by(cpf=cpf).first()
    if existing_responsavel:
        return jsonify({'message': 'CPF já cadastrado'}), 400

    new_responsavel = Responsavel(name=name, cpf=cpf, telefone=telefone, id_users=id_users)
    db.session.add(new_responsavel)
    db.session.commit()

    return jsonify({
        'message': 'User created successfully', 
        'user': {
            'id': new_responsavel.id, 
            'name': new_responsavel.name, 
            'cpf': new_responsavel.cpf,
            'telefone': new_responsavel.telefone,
            'user_id': new_responsavel.id_users
        }
    }), 201

# Rota para listar todos os responsáveis
@responsavel_blueprint.route('/responsaveis', methods=['GET'])
def get_responsaveis():
    responsaveis = Responsavel.query.all()
    responsaveis_data = [{'id': responsavel.id, 'name': responsavel.name, 'cpf': responsavel.cpf, 'telefone': responsavel.telefone, 'user_id': responsavel.id_users} for responsavel in responsaveis]
    return jsonify(responsaveis_data), 200

# Rota para obter um responsável específico
@responsavel_blueprint.route('/responsaveis/<int:responsavel_id>', methods=['GET'])
def get_responsavel(responsavel_id):
    responsavel = Responsavel.query.get(responsavel_id)
    if responsavel:
        return jsonify({'id': responsavel.id, 'name': responsavel.name, 'cpf': responsavel.cpf, 'telefone': responsavel.telefone, 'user_id': responsavel.id_users}), 200
    return jsonify({'message': 'User not found'}), 404

# Rota para atualizar um responsável
@responsavel_blueprint.route('/responsaveis/<int:responsavel_id>', methods=['PUT'])
def update_user(responsavel_id):
    data = request.get_json()
    responsavel = Responsavel.query.get(responsavel_id)
    
    if responsavel:
        # Verifica se o CPF já existe em outro usuário
        new_cpf = data.get('cpf', responsavel.cpf)
        if new_cpf != responsavel.cpf and Responsavel.query.filter_by(cpf=new_cpf).first():
            return jsonify({'message': 'CPF já cadastrado'}), 400

        responsavel.name = data.get('name', responsavel.name)
        responsavel.cpf = new_cpf
        responsavel.telefone = data.get('telefone', responsavel.telefone)
        db.session.commit()
        
        return jsonify({
            'message': 'User updated successfully', 
            'user': {
                'id': responsavel.id, 
                'name': responsavel.name, 
                'cpf': responsavel.cpf,
                'telefone': responsavel.telefone,
                'user_id': responsavel.id_users
            }
        }), 200
    return jsonify({'message': 'User not found'}), 404

# Rota para deletar um responsável
@responsavel_blueprint.route('/responsaveis/<int:responsavel_id>', methods=['DELETE'])
def delete_user(responsavel_id):
    responsavel = Responsavel.query.get(responsavel_id)
    
    if responsavel:
        db.session.delete(responsavel)
        db.session.commit()
        return jsonify({'message': 'User deleted successfully'}), 200
    return jsonify({'message': 'User not found'}), 404
