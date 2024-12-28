document.getElementById('btnMaisCampos').addEventListener('click', function() {
    const camposExtras = document.getElementById('camposExtras');
    if (camposExtras.style.display === 'none') {
      camposExtras.style.display = 'block';
    } else {
      camposExtras.style.display = 'none';
    }
  });