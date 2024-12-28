# Inteli - Instituto de Tecnologia e Liderança 

<p align="center">
<a href= "https://www.inteli.edu.br/"><img src="assets/inteli.png" alt="Inteli - Instituto de Tecnologia e Liderança" border="0" width=40% height=40%></a>
</p>

<br>

# Projeto A-Portal

## Grupo Hydra

## 👨‍🎓 Integrantes: 
- <a href="https://www.linkedin.com/in/arthur-bretas-b2a6a82b9/">Arthur Bretas</a>
- <a href="https://www.linkedin.com/in/vinicius-testa-passos/">Vinicius Testa</a>
- <a href="https://www.linkedin.com/in/calebe-matias/">Calebe Matias</a> 
- <a href="https://www.linkedin.com/in/daviversan/">Davi Versan</a> 
- <a href="https://www.linkedin.com/in/vinicius-ibiapina/">Vinicius Ibiapina</a>
- <a href="https://www.linkedin.com/in/lucas-nunes-matheus/">Lucas Matheus Nunes</a>

## 👩‍🏫 Professores:
### Orientador(a) 
- <a href="https://www.linkedin.com/in/fabiana-martins-de-oliveira-8993b0b2/">Fabiana Martins</a>
### Instrutores
- <a href="https://www.linkedin.com/in/vthayashi/">Victor Hayashi</a>
- <a href="https://www.linkedin.com/in/fernando-pizzo-208b526a/">Fernando Pizzo</a> 
- <a href="https://www.linkedin.com/in/renato-penha/">Renato Penha</a> 
- <a href="https://www.linkedin.com/in/fillipe-resina-b2211a22/">Fillipe Resina</a>
- <a href="https://www.linkedin.com/in/bruna-mayer/">Bruna Mayer</a> 
- <a href="https://www.linkedin.com/in/filipe-gon%C3%A7alves-08a55015b/">Filipe Gonçalves</a>

## 📜 Descrição

&nbsp;&nbsp;&nbsp;&nbsp; O projeto A-Portal é um sistema de controle de acesso que utiliza captura biométrica para autenticação de usuários. O sistema é composto por um dispositivo IoT (Microcontrolador ESP32, leitor biométrico DY50, display LCD, dentre outros componentes) e uma aplicação web, que permite o cadastro de usuários e a visualização de logs de acesso. O dispositivo IoT é responsável por capturar a biometria do usuário e enviar para um servidor MQTT, que por sua vez realiza o salvamento no banco de dados e envio dos dados para a aplicação web. Esta é responsável por exibir os logs de acesso e permitir o cadastro de novos usuários (Alunos, Professores e Funcionários). O projeto foi desenvolvido para o Instituto Apontar, uma instituição educacional sem fins lucrativos que atualmente atende cerca de 600 alunos do Rio de Janeiro. O A-Portal foi desenvolvido para atender as necessidades de controle de acesso da instituição, garantindo segurança e praticidade no dia a dia dos usuários.


Portanto, segue um vídeo demonstrativo do protótipo final do projeto:
<a href="https://youtube.com/shorts/rd58tc8TJSY?feature=share">Para assistir o vídeo demonstrativo da versão final do projeto, clique aqui.</a>


## 📁 Estrutura de pastas

Dentre os arquivos e pastas presentes na raiz do projeto, definem-se:

- `assets/`: nesta pasta estão guardados os arquivos de imagem utilizados no projeto, como logos, imagens de componentes eletrônicos e ilustrações apresentadas na documentação.

- `document/`: aqui estão os documentos do projeto: o [Manual de Instrução](document/manual-de-instrucao-a-portal.pdf) e a [Documentação](document/documentacao.md).

- `src/`: contém o código fonte do projeto, dividido em três pastas: `frontend`, `backend` e `firmware`.

    - `frontend/`: contém o código do Frontend da aplicação, separado em `public/` e `views/`.

        - `public/`: contém os arquivos estáticos da aplicação, como imagens, estilos e scripts.

        - `views/`: contém o arquivso `.ejs`, que renderizam as páginas da aplicação.
        - `main.js`: arquivo que contém o código JavaScript responsável por realizar as requisições ao Backend e atualizar a interface da aplicação.

    - `backend/`: contém o código do Backend da aplicação, cujas principais partes são `/routes` e `app.py`.

        - `routes/`: pasta que contém os arquivos `.py` que definem as rotas da aplicação.

        - `app.py`: arquivo principal do Backend, que inicializa o servidor e define as configurações da aplicação.

    - `firmware/`: contém o Código Embarcado construído em `C++`, que será carregado pelo microcontrolador. Suas principais pastas são `biometric_capture/src` e `biometric_capture/include`.


        - `biometric_capture/src`: contém o arquivo principal do código `C++`(`main.cpp`), que executa um loop das funcionalidades do dispositivo.

        - `biometric_capture/include`: contém os arquivos de cabeçalho do projeto, que são arquivos `.hpp`.

- `README.md`: arquivo que serve como guia introdutório e explicação geral sobre o projeto e a aplicação.

## 🔧 Instalação

Para realizar a instalação completa do projeto localmente, é necessário clonar o repositório do projeto em sua máquina. Para isso, execute o comando abaixo no terminal:
```bash
git clone https://github.com/Inteli-College/2024-2B-T14-IN04-G02.git
```
É importante entender que três partes do projeto devem ser executadas separadamente, são elas: o Frontend (construído com `Express.js`e `Bootstrap.js`), o Backend (`Python` e `Flask`) e o Código Embarcado (`C++`).
A seguir, são apresentadas as instruções para execução de cada uma dessas partes localmente:

### Frontend
Para executar o Frontend, é necessário ter o `Node.js` instalado na máquina (Clique [aqui](https://nodejs.org/en/download/) para baixar) e o gerenciador de pacotes `npm` (já vem instalado com o Node.js). Com essas ferramentas instaladas, vá até a pasta do Frontend e execute os comandos abaixo para instalar as dependências do projeto e, em seguida, executar o Frontend:
```bash
cd 2024-2B-T14-IN04-G02/src/frontend
npm install
npm run dev
```

### Backend
Para executar o Backend, é necessário ter o `Python3` instalado na máquina (Clique [aqui](https://www.python.org/downloads/) para baixar) e o gerenciador de pacotes `pip` (Clique [aqui](https://pip.pypa.io/en/stable/installation/) para instalar). Com essas ferramentas instaladas, execute os comandos abaixo para instalar as dependências do projeto e, em seguida, executar o Backend:
```bash
cd 2024-2B-T14-IN04-G02/src/backend
pip install -r requirements.txt
python app.py
```

### Código Embarcado
Para executar o Código Embarcado, é necessário ter o `PlatformIO` instalado na máquina (Clique [aqui](https://platformio.org/install) para baixar, ou utilize a extensão do `PlatformIO` no `VSCode`). Com essa ferramenta instalada, você deverá conectar o dispositivo IoT ao computador (por meio do cabo USB-B) e abrir o projeto usando o `PlatformIO`. Em seguida, compile e faça o upload do código para o dispositivo pressionando o botão de upload no canto superior direito da tela.

### Manual de Instruções
Para mais informações sobre o projeto, é possível acessar o manual de instruções do projeto, que contém informações detalhadas sobre o projeto, como Componentes/Recursos, Guia de Montagem, Guia de Instalação, Guia de Configuração, Guia de Operação, Descarte e Troubleshooting. (Clique [aqui](document/manual-de-instrucao-a-portal.pdf) para acessar o manual de instruções.
[Manual de Instruções](document/manual-de-instrucao-a-portal.pdf))



## 🗃 Histórico de lançamentos

* 0.5.0 - 19/12/2024
    * Protótipo final montado
    * Apresentação do projeto
    * Manual de instruções refinado
    * Conclusão da documentação

* 0.4.0 - 06/12/2024
    * Manual de instruções
    * Possibilidades de descarte
    * Diagrama UML
    * Desenvolvimento do Back end e Front end

* 0.3.0 - 22/11/2024
    * Arquitetura da solução
    * Implementação do protocolo MQTT e I2C à solução
    * Início da plataforma web

* 0.2.0 - 08/11/2024
    * Requisitos não funcionais
    * Início da montagem do protótipo físico (circuito)

* 0.1.0 - 25/10/2024
    *Análise de negócios
    *Definição de escopo do projeto
    *Workshop com o parceiro

## 📋 Licença/License
[Hydra](https://github.com/Inteli-College/2024-2B-T14-IN04-G02), by [Inteli](https://www.inteli.edu.br/), [Davi Versan](https://www.linkedin.com/in/daviversan/), [Calebe Matias](https://www.linkedin.com/in/calebe-matias/), [Lucas Nunes](https://www.linkedin.com/in/lucas-nunes-matheus/), [Arthur Bretas](https://www.linkedin.com/in/arthur-bretas/), [Vinicius Ibiapina](https://www.linkedin.com/in/vinicius-ibiapina/), [Vinicius Testa](https://www.linkedin.com/in/vinicius-testa-passos/), under license [Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0/?ref=chooser-v1)
