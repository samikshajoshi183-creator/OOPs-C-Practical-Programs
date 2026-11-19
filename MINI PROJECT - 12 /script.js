const API = "/api/books";
const rows = document.getElementById("bookRows");
const message = document.getElementById("message");
let allBooks = [];

function showMessage(text, isError = false) {
  message.textContent = text;
  message.classList.toggle("error", isError);
}

async function request(url, options = {}) {
  const response = await fetch(url, {
    headers: { "Content-Type": "application/json" },
    ...options
  });
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || "Request failed");
  return data;
}

async function loadBooks() {
  try {
    allBooks = await request(API);
    renderBooks();
    updateStats();
  } catch (error) {
    rows.innerHTML = `<tr><td colspan="5" class="empty">Could not load books. Is the C++ server running?</td></tr>`;
    showMessage(error.message, true);
  }
}

function updateStats() {
  document.getElementById("totalCount").textContent = allBooks.length;
  document.getElementById("availableCount").textContent = allBooks.filter(b => !b.issued).length;
  document.getElementById("issuedCount").textContent = allBooks.filter(b => b.issued).length;
}

function renderBooks() {
  const term = document.getElementById("search").value.trim().toLowerCase();
  const filtered = allBooks.filter(book =>
    String(book.id).includes(term) ||
    book.title.toLowerCase().includes(term) ||
    book.author.toLowerCase().includes(term)
  );
  document.getElementById("resultCount").textContent = `${filtered.length} record(s)`;

  if (!filtered.length) {
    rows.innerHTML = `<tr><td colspan="5" class="empty">No matching books found.</td></tr>`;
    return;
  }

  rows.innerHTML = filtered.map(book => `
    <tr>
      <td>#${book.id}</td>
      <td><strong>${escapeHtml(book.title)}</strong></td>
      <td>${escapeHtml(book.author)}</td>
      <td><span class="pill ${book.issued ? "issued" : "available"}">${book.issued ? "Issued" : "Available"}</span></td>
      <td><div class="actions">
        <button class="${book.issued ? "return" : "issue"}" data-action="${book.issued ? "return" : "issue"}" data-id="${book.id}">${book.issued ? "Return" : "Issue"}</button>
        <button class="delete" data-action="delete" data-id="${book.id}">Delete</button>
      </div></td>
    </tr>
  `).join("");
}

function escapeHtml(value) {
  return value.replace(/[&<>"']/g, char => ({
    "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;"
  }[char]));
}

document.getElementById("bookForm").addEventListener("submit", async event => {
  event.preventDefault();
  const title = document.getElementById("title").value.trim();
  const author = document.getElementById("author").value.trim();
  if (!title || !author) return showMessage("Please enter both title and author.", true);

  try {
    await request(API, { method: "POST", body: JSON.stringify({ title, author }) });
    event.target.reset();
    showMessage("Book added successfully.");
    await loadBooks();
  } catch (error) {
    showMessage(error.message, true);
  }
});

rows.addEventListener("click", async event => {
  const button = event.target.closest("button[data-action]");
  if (!button) return;
  const { action, id } = button.dataset;

  try {
    if (action === "delete") {
      if (!confirm("Are you sure you want to delete this book?")) return;
      await request(`${API}/${id}`, { method: "DELETE" });
      showMessage("Book deleted.");
    } else {
      const result = await request(`${API}/${id}/${action}`, { method: "PUT" });
      showMessage(result.message);
    }
    await loadBooks();
  } catch (error) {
    showMessage(error.message, true);
  }
});

document.getElementById("search").addEventListener("input", renderBooks);
document.getElementById("refreshBtn").addEventListener("click", loadBooks);
loadBooks();
