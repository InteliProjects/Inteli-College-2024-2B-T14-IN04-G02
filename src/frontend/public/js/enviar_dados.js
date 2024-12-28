import { setCurrentUserId } from './request_digital.js';

async function enviarDados() {
  const name = document.getElementById('nome').value.trim();
  const cpf = document.getElementById('cpf').value.trim();
  

  if (!name || !cpf) {
    alert('Nome e CPF são campos obrigatórios');
    return;
  }

  try {
    console.log('Tentando criar usuário:', { name, cpf });
    
    const baseURL = 'http://localhost:5000';
    const config = {
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
      },
      withCredentials: true
    };

    const responseUsuario = await axios.post(`${baseURL}/api/users`, {
      name: name,
      cpf: cpf
    }, config);
    
    console.log('Resposta completa:', responseUsuario);
    console.log('Usuário criado:', responseUsuario.data);
    
    // Define o ID do usuário atual para uso no cadastro de digital
    setCurrentUserId(responseUsuario.data.user.id);
    
    const camposExtras = document.getElementById('camposExtras');
    

    if (camposExtras.style.display !== 'none') {
      const nameResponsavel = document.getElementById('nomeResponsavel').value.trim();
      const cpfResponsavel = document.getElementById('cpfResponsavel').value.trim();
      const telefoneResponsavel = document.getElementById('telefoneResponsavel').value.trim();
      
      if (nameResponsavel && cpfResponsavel && telefoneResponsavel) {
        console.log('Tentando criar responsável:', { 
          nameResponsavel, 
          cpfResponsavel, 
          telefoneResponsavel,
          user_id: responseUsuario.data.user.id 
        });

        const responseResponsavel = await axios.post(`${baseURL}/api/responsaveis/registrar-responsavel`, {
          name: nameResponsavel,
          cpf: cpfResponsavel,
          telefone: telefoneResponsavel,
          user_id: responseUsuario.data.user.id 
        });
        console.log('Responsável criado:', responseResponsavel.data);
      }
    }

    alert('Dados salvos com sucesso! Agora você pode cadastrar a digital do usuário.');
    
  } catch (error) {
    console.log('URL tentada:', error.config?.url);
    console.log('Dados enviados:', error.config?.data);
    console.log('Resposta do servidor:', error.response?.data);
    
    const errorMessage = error.response?.data?.message || 'Erro ao salvar os dados';
    alert(`Erro: ${errorMessage}`);
    console.error('Erro completo:', error);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  const btnSalvar = document.getElementById('salvarUsuario');
  if (btnSalvar) {
    btnSalvar.addEventListener('click', enviarDados);
  }
});

export { enviarDados };