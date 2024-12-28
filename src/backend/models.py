from extensions import db

class User(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    name = db.Column(db.String(255), nullable=False)
    cpf = db.Column(db.String(255), unique=True, nullable=True)

    registros = db.relationship('Registro', backref='usuario', lazy=True)
    # digitals = db.relationship('Digital', backref='user', lazy=True)
    responsaveis = db.relationship('Responsavel', back_populates='user', lazy=True)

class Registro(db.Model):
    __tablename__ = 'registers'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    id_users = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=True)
    acesso = db.Column(db.DateTime, nullable=True)
    saida = db.Column(db.DateTime, nullable=True)
    id_catracas = db.Column(db.Integer, db.ForeignKey('catracas.id'), nullable=True)

    catraca = db.relationship('Catraca', backref=db.backref('registers', lazy=True))

class Responsavel(db.Model):
    __tablename__ = 'responsavel'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    name = db.Column(db.String(255), nullable=False)
    cpf = db.Column(db.String(11), nullable=False)
    telefone = db.Column(db.String(15), nullable=False)
    id_users = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=True)

    user = db.relationship('User', back_populates='responsaveis', lazy=True)

class Admin(db.Model):
    __tablename__ = 'adm'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    email = db.Column(db.String(255), nullable=False)
    senha = db.Column(db.String(255), nullable=False)

class Catraca(db.Model):
    __tablename__ = 'catracas'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    chipID = db.Column(db.Integer, nullable=False)
    status = db.Column(db.Boolean, nullable=False) 

class Cadastro(db.Model):
    __tablename__ = 'cadastros'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    id_users = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=True)
    id_catracas = db.Column(db.Integer, db.ForeignKey('catracas.id'), nullable=True)

    user = db.relationship('User', backref=db.backref('cadastros', lazy=True))
    catraca = db.relationship('Catraca', backref=db.backref('cadastros', lazy=True))

class BiometricRequest(db.Model):
    __tablename__ = 'biometric_requests'
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    request_id = db.Column(db.String(36), nullable=False, unique=True)  # UUID gerado no front-end
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    catraca_id = db.Column(db.Integer, db.ForeignKey('catracas.id'), nullable=False)
    status = db.Column(db.String(20), nullable=True)  # 'approved', 'failed'
    timestamp = db.Column(db.DateTime, default=db.func.now())

    user = db.relationship('User', backref=db.backref('biometric_requests', lazy=True))
    catraca = db.relationship('Catraca', backref=db.backref('biometric_requests', lazy=True))

