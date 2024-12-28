const express = require('express');
const http = require('http');
const app = express();
const server = http.createServer(app);
const axios = require("axios");

// access cross origin
app.use((req, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
  res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
  res.setHeader('Access-Control-Allow-Credentials', true);
  next();
});

app.use(express.static('public'));

app.set('view engine', 'ejs');
app.set('views', './views'); // Diretório onde estarão os arquivos EJS

app.get('/', (req, res) => {
  // AQUI VEM A LÓGICA
  let users = [
    {nome: "Vinicius", idade: 21},
    {nome: "Bretas", idade: 21},
    {nome: "Versan", idade: 21},
    {nome: "Ibiapina", idade: 20}
  ]

  // users = users.filter((user) => user.nome === req.query.nome);
  // FIM DA LÓGICA

  // AQUI VEM A RENDERIZAÇÃO
  res.render('index', {...req.query, users}); // Renderiza o arquivo index.ejs com os dados
});

app.get('/info-usuarios', (req, res) => {
  res.render('info-usuarios');
});


app.get('/logs', async (req, res) => {
  try {
    
    // const response = await axios.get('http://127.0.0.1:5000/registros');
    // console.log('Logs:', response.data);
    
    // Resposta mockada do backend, enquanto a rota não está funcionando
    const logs = [
      {name: "Calebe Matias", timestamp: Date.now() -100000000}, 
      {name: "Lucas Scrum", timestamp: Date.now() - 2000000000},
      {name: "Vinicius Bretas", timestamp: Date.now() - 3000000000},
      {name: "Vinicius Ibiapina", timestamp: Date.now() - 4000000000},
      {name: "Davi Versan", timestamp: Date.now() - 5000000000},
      {name: "Lucas Scrum", timestamp: Date.now() - 6000000000},
      {name: "Vinicius Bretas", timestamp: Date.now() - 7000000000},
      {name: "Vinicius Ibiapina", timestamp: Date.now() - 8000000000},
      {name: "Davi Versan", timestamp: Date.now() - 9000000000},
      {name: "Lucas Scrum", timestamp: Date.now() - 10000000000},
      {name: "Vinicius Bretas", timestamp: Date.now() - 11000000000},
      {name: "Vinicius Ibiapina", timestamp: Date.now() - 12000000000},
      {name: "Davi Versan", timestamp: Date.now() - 13000000000},
      {name: "Lucas Scrum", timestamp: Date.now() - 14000000000},
      {name: "Vinicius Bretas", timestamp: Date.now() - 15000000000},
      {name: "Vinicius Ibiapina", timestamp: Date.now() - 16000000000},
      {name: "Davi Versan", timestamp: Date.now() - 17000000000},
      {name: "Lucas Scrum", timestamp: Date.now() - 18000000000},
      {name: "Vinicius Bretas", timestamp: Date.now() - 19000000000},
      {name: "Vinicius Ibiapina", timestamp: Date.now() - 20000000000},
      {name: "Davi Versan", timestamp: Date.now() - 21000000000},
      {name: "Lucas Scrum", timestamp: Date.now() - 22000000000},
      {name: "Vinicius Bretas", timestamp: Date.now() - 23000000000},
      {name: "Vinicius Ibiapina", timestamp: Date.now() - 24000000000},
      {name: "Davi Versan", timestamp: Date.now() - 25000000000},
      {name: "Arthur Bretas", timestamp: Date.now() - 26000000000},
      {name: "Arthur Ibiapina", timestamp: Date.now() - 27000000000},
      {name: "Arthur Versan", timestamp: Date.now() - 28000000000},
      {name: "Arthur Scrum", timestamp: Date.now() - 29000000000},
      {name: "Arthur Bretas", timestamp: Date.now() - 30000000000},
      {name: "Arthur Ibiapina", timestamp: Date.now() - 31000000000},
    ]

    res.render('logs', {logs});
  } catch (error) {
    console.error('Error fetching logs:', error);
    res.render('logs', { logs: [] }); 
  }
});

server.listen(3000, () => {
  console.log('Servidor Express ouvindo na porta 3000');
});
