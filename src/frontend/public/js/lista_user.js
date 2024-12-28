document.querySelector("#filtraNome").addEventListener("click", function() {
  const nome = document.querySelector("#nome").value;
  location.href = '/?nome=' + nome;
});