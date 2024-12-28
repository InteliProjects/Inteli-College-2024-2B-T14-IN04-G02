from flask import Blueprint, request, jsonify, make_response
from extensions import db
from models import User
from flask_cors import cross_origin

user_blueprint = Blueprint('user', __name__)

@user_blueprint.route('', methods=['OPTIONS'])
def handle_options():
    response = make_response()
    response.headers.add('Access-Control-Allow-Origin', 'http://localhost:3000')
    response.headers.add('Access-Control-Allow-Headers', 'Content-Type, Authorization')
    response.headers.add('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS')
    response.headers.add('Access-Control-Allow-Credentials', 'true')
    return response, 204

@user_blueprint.route('', methods=['POST'])
def create_user():
    print("Recebendo requisição POST em /api/users")
    print("Dados recebidos:", request.get_json())
    data = request.get_json()
    name = data.get('name')
    cpf = data.get('cpf')
    
    # Verifica se o CPF já está cadastrado
    existing_user = User.query.filter_by(cpf=cpf).first()
    if existing_user:
        return jsonify({'message': 'CPF já cadastrado'}), 400

    new_user = User(name=name, cpf=cpf)
    db.session.add(new_user)
    db.session.commit()

    return jsonify({
        'message': 'User created successfully', 
        'user': {
            'id': new_user.id, 
            'name': new_user.name, 
            'cpf': new_user.cpf
        }
    }), 201

# Rota para listar todos os usuários
@user_blueprint.route('', methods=['GET'])
def get_users():
    users = User.query.all()
    users_data = [{'id': user.id, 'name': user.name, 'cpf': user.cpf} for user in users]
    return jsonify(users_data), 200

# Rota para obter um usuário específico
@user_blueprint.route('/<int:user_id>', methods=['GET'])
def get_user(user_id):
    user = User.query.get(user_id)
    if user:
        return jsonify({'id': user.id, 'name': user.name, 'cpf': user.cpf}), 200
    return jsonify({'message': 'User not found'}), 404

# Rota para atualizar um usuário
@user_blueprint.route('/<int:user_id>', methods=['PUT'])
def update_user(user_id):
    data = request.get_json()
    user = User.query.get(user_id)
    
    if user:
        # Verifica se o CPF já existe em outro usuário
        new_cpf = data.get('cpf', user.cpf)
        if new_cpf != user.cpf and User.query.filter_by(cpf=new_cpf).first():
            return jsonify({'message': 'CPF já cadastrado'}), 400

        user.name = data.get('name', user.name)
        user.cpf = new_cpf
        db.session.commit()
        
        return jsonify({
            'message': 'User updated successfully', 
            'user': {
                'id': user.id, 
                'name': user.name, 
                'cpf': user.cpf
            }
        }), 200
    return jsonify({'message': 'User not found'}), 404

# Rota para deletar um usuário
@user_blueprint.route('/<int:user_id>', methods=['DELETE'])
def delete_user(user_id):
    user = User.query.get(user_id)
    
    if user:
        db.session.delete(user)
        db.session.commit()
        return jsonify({'message': 'User deleted successfully'}), 200
    return jsonify({'message': 'User not found'}), 404
