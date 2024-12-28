let currentUserId = null; 

function setCurrentUserId(userId) {
    currentUserId = userId;
}

async function iniciarCadastroDigital() {
    const statusCard = document.getElementById('statusCard');
    const statusTable = document.getElementById('statusTable');

    if (!currentUserId) {
        alert('Por favor, primeiro salve os dados do usuário antes de cadastrar a digital.');
        return;
    }

    try {
        const requestId = crypto.randomUUID(); 
        
        console.log('Iniciando cadastro biométrico:', {
            request_id: requestId,
            user_id: currentUserId
        });
        
        const response = await axios.post('http://localhost:5000/api/registro-biometrico/iniciar', {
            request_id: requestId,
            user_id: currentUserId
        }, {
            headers: {
                'Content-Type': 'application/json'
            },
            withCredentials: true
        });

        console.log('Resposta do servidor:', response.data);

        // Mostra o card de status
        statusCard.style.display = 'block';

        // Adiciona a linha na tabela
        const row = document.createElement('tr');
        row.id = `request-${requestId}`;
        row.innerHTML = `
            <td>${requestId}</td>
            <td><span class="badge bg-warning">Pendente</span></td>
            <td>${new Date().toLocaleString()}</td>
        `;
        statusTable.appendChild(row);

        // Inicia o polling para verificar o status
        verificarStatus(requestId);

    } catch (error) {
        console.error('Erro ao iniciar cadastro:', error);
        
        let errorMessage = 'Erro ao iniciar o cadastro da digital.';
        
        if (error.response) {
            console.log('Detalhes do erro:', error.response.data);
            
            // Se o erro for relacionado ao MQTT
            if (error.response.status === 503) {
                if (error.response.data.error === 'mqtt_not_initialized') {
                    errorMessage = 'O serviço de biometria não está inicializado. Por favor, tente novamente em alguns instantes.';
                } else if (error.response.data.error === 'mqtt_unavailable') {
                    errorMessage = 'O serviço de biometria está temporariamente indisponível. Por favor, tente novamente em alguns instantes.';
                } else if (error.response.data.error === 'mqtt_publish_failed') {
                    errorMessage = 'Não foi possível enviar o comando para o leitor biométrico. Por favor, verifique se o dispositivo está conectado e tente novamente.';
                }
            }
            
            errorMessage = error.response.data.message || errorMessage;
        }
        
        alert(`Erro: ${errorMessage}`);
    }
}

async function verificarStatus(requestId) {
    try {
        const response = await axios.get(`http://localhost:5000/api/registro-biometrico/status/${requestId}`, {
            withCredentials: true
        });

        console.log('Status da requisição:', response.data);

        const row = document.getElementById(`request-${requestId}`);
        if (row) {
            const statusCell = row.querySelector('td:nth-child(2)');
            const status = response.data.status;
            const mqtt_status = response.data.mqtt_status;

            let badgeClass = 'bg-warning';
            let statusText = 'Pendente';

            if (status === 'approved') {
                badgeClass = 'bg-success';
                statusText = 'Aprovado';
            } else if (status === 'failed') {
                badgeClass = 'bg-danger';
                statusText = 'Falhou';
            }

            // Adiciona indicador de status MQTT
            const mqttIndicator = mqtt_status === 'connected' ? 
                '<span class="badge bg-success">MQTT Conectado</span>' : 
                '<span class="badge bg-danger">MQTT Desconectado</span>';

            statusCell.innerHTML = `
                <span class="badge ${badgeClass}">${statusText}</span>
                ${mqttIndicator}
            `;

            // Continua verificando se ainda estiver pendente e MQTT estiver conectado
            if (status === 'pending' && mqtt_status === 'connected') {
                setTimeout(() => verificarStatus(requestId), 2000);
            } else if (status === 'pending' && mqtt_status !== 'connected') {
                console.log('Parando polling devido a desconexão do MQTT');
                statusCell.innerHTML += '<br><small class="text-danger">Serviço temporariamente indisponível</small>';
            }
        }
    } catch (error) {
        console.error('Erro ao verificar status:', error);
        if (error.response) {
            console.log('Detalhes do erro de status:', error.response.data);
        }
    }
}

// Inicializa os event listeners quando o DOM estiver carregado
document.addEventListener('DOMContentLoaded', () => {
    const btnCadastrar = document.getElementById('cadastrarDigital');
    if (btnCadastrar) {
        btnCadastrar.addEventListener('click', iniciarCadastroDigital);
    }
});

export { iniciarCadastroDigital, verificarStatus, setCurrentUserId };  