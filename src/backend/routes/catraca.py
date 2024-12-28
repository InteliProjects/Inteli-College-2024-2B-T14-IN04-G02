from flask import Blueprint, request, jsonify
from extensions import db
from models import Catraca

catraca_blueprint = Blueprint('catraca', __name__)

catraca_blueprint.route('/registrar-catraca', methods=['POST'])
def create_catraca():
    data = request.get_json()
    chipID = data.get('chipID')
    status = data.get('status')

    new_catraca = Catraca(chipID=chipID, status=status)
    db.session.add(new_catraca)
    db.session.commit()

    return jsonify({
        'message': 'Catraca criada com sucesso',
        'catraca': {
            'id': new_catraca.id,
            'chipID': new_catraca.chipID,
            'status': new_catraca.status
        }
    }), 201
    
catraca_blueprint.route('/catracas', methods=['GET'])
def get_catracas():
    catracas = Catraca.query.all()
    catracas_data = [{'id': catraca.id, 'chipID': catraca.chipID, 'status': catraca.status} for catraca in catracas]
    return jsonify(catracas_data), 200


catraca_blueprint.route('/catracas/<int:catraca_id>', methods=['GET'])
def get_catraca(catraca_id):
    catraca = Catraca.query.get(catraca_id)
    if catraca:
        return jsonify({'id': catraca.id, 'chipID': catraca.chipID, 'status': catraca.status}), 200
    return jsonify({'message': 'Catraca não encontrada'}), 404


catraca_blueprint.route('/catracas/<int:catraca_id>', methods=['PUT'])
def update_catraca(catraca_id):
    data = request.get_json()
    catraca = Catraca.query.get(catraca_id)

    if catraca:
        new_chipID = data.get('chipID', catraca.chipID)
        new_status = data.get('status', catraca.status)

        catraca.chipID = new_chipID
        catraca.status = new_status
        db.session.commit()

        return jsonify({
            'message': 'Catraca atualizada com sucesso',
            'catraca': {
                'id': catraca.id,
                'chipID': catraca.chipID,
                'status': catraca.status
            }
        }), 200
    return jsonify({'message': 'Catraca não encontrada'}), 404





    