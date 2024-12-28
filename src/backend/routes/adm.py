from flask import Blueprint, request, jsonify
from extensions import db
from models import Admin

adm_blueprint = Blueprint('adm', __name__)

# Rota para criar um novo administrador
@adm_blueprint.route('/adms', methods=['POST'])
def create_adm():
    data = request.get_json()
    email = data.get('email')
    senha = data.get('senha')
    
    # Verifica se o email já está cadastrado
    existing_adm = Admin.query.filter_by(email=email).first()
    if existing_adm:
        return jsonify({'message': 'Email já cadastrado'}), 400

    new_adm = Admin(email=email, senha=senha)
    db.session.add(new_adm)
    db.session.commit()

    return jsonify({
        'message': 'Admin created successfully', 
        'adm': {
            'id': new_adm.id, 
            'email': new_adm.email
        }
    }), 201

# Rota para listar todos os administradores
@adm_blueprint.route('/adms', methods=['GET'])
def get_adms():
    adms = Admin.query.all()
    adms_data = [{'id': adm.id, 'email': adm.email} for adm in adms]
    return jsonify(adms_data), 200

# Rota para obter um administrador específico
@adm_blueprint.route('/adms/<int:adm_id>', methods=['GET'])
def get_adm(adm_id):
    adm = Admin.query.get(adm_id)
    if adm:
        return jsonify({'id': adm.id, 'email': adm.email}), 200
    return jsonify({'message': 'Admin not found'}), 404

# Rota para atualizar um administrador
@adm_blueprint.route('/adms/<int:adm_id>', methods=['PUT'])
def update_adm(adm_id):
    data = request.get_json()
    adm = Admin.query.get(adm_id)
    
    if adm:
        # Verifica se o email já existe em outro administrador
        new_email = data.get('email', adm.email)
        if new_email != adm.email and Admin.query.filter_by(email=new_email).first():
            return jsonify({'message': 'Email já cadastrado'}), 400

        adm.email = new_email
        adm.senha = data.get('senha', adm.senha)
        db.session.commit()
        
        return jsonify({
            'message': 'Admin updated successfully', 
            'adm': {
                'id': adm.id, 
                'email': adm.email
            }
        }), 200
    return jsonify({'message': 'Admin not found'}), 404

# Rota para deletar um administrador
@adm_blueprint.route('/adms/<int:adm_id>', methods=['DELETE'])
def delete_adm(adm_id):
    adm = Admin.query.get(adm_id)
    
    if adm:
        db.session.delete(adm)
        db.session.commit()
        return jsonify({'message': 'Admin deleted successfully'}), 200
    return jsonify({'message': 'Admin not found'}), 404
