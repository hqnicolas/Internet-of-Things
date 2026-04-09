async function loadInfo() {
  const cards = document.querySelectorAll("#info dd");

  try {
    const response = await fetch("/api/info");
    if (!response.ok) {
      throw new Error("Falha ao buscar informacoes");
    }

    const data = await response.json();
    cards[0].textContent = data.board;
    cards[1].textContent = data.storage;
    cards[2].textContent = data.status;
    cards[3].textContent = data.ip;
  } catch (error) {
    cards.forEach((card) => {
      card.textContent = "Erro";
    });
  }
}

document.getElementById("refreshButton").addEventListener("click", loadInfo);

loadInfo();
