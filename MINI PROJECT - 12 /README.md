A working browser-based Library Management System mini-project. The frontend uses **HTML, CSS and JavaScript**, while a **C++17 backend** provides HTTP API endpoints and stores records in a CSV-style data file.

## Features

- Add a book with title and author
- View all books and their availability
- Search by book ID, title or author
- Issue an available book
- Return an issued book
- Delete a book
- Dashboard counts for total, available and issued books
- Persistent storage in `library.csv`
- Error handling for duplicate actions, missing books and invalid input

## Technology stack

- Frontend: HTML5, CSS3, Vanilla JavaScript
- Backend: C++17 using POSIX sockets
- Storage: local text file (`library.csv`)

## Project structure

```text
Library-Management-System/
├── main.cpp
├── index.html
├── style.css
├── script.js
├── README.md
├── FLOWCHART.md
└── library.csv       # created automatically when books are added
```

## Requirements

- C++ compiler with C++17 support (g++ or clang++)
- macOS or Linux (the socket implementation uses POSIX APIs)
- A modern web browser

## Run the project

1. Download or clone this repository.
2. Open a terminal in the project folder.
3. Compile the C++ backend:

   ```bash
   g++ -std=c++17 -pthread main.cpp -o library
   ```

4. Start the backend server:

   ```bash
   ./library
   ```

5. Open this address in your browser:

   `http://localhost:8080`

Keep the terminal running while using the application. Press `Ctrl+C` to stop the server.

### Windows note

This version uses POSIX socket headers (`sys/socket.h`, `unistd.h`) and is intended for macOS/Linux. On Windows, run it inside WSL or use a Windows-compatible socket implementation.

## API endpoints

| Method | Endpoint | Purpose |
|---|---|---|
| GET | `/api/books` | List all books |
| POST | `/api/books` | Add a book |
| PUT | `/api/books/{id}/issue` | Issue a book |
| PUT | `/api/books/{id}/return` | Return a book |
| DELETE | `/api/books/{id}` | Delete a book |

## Data storage

The backend creates `library.csv` in the project folder when a book is first added. Each record stores:

`id|title|author|issued`

The issued field is `0` for available and `1` for issued. Do not manually add the `|` character in book titles or author names.

## Testing checklist

1. Add a book and confirm it appears in the catalogue.
2. Refresh the browser and confirm the book is still present.
3. Issue the book and confirm its status changes to Issued.
4. Try issuing it again and confirm an error is shown.
5. Return the book and confirm its status changes to Available.
6. Search for a book by title, author or ID.
7. Delete a book and confirm it disappears.

## Limitations

- This is an educational local prototype, not a production library system.
- No login, user roles, fine calculation or multi-branch support is included.
- The server is designed for local demonstration and should not be exposed directly to the public internet.
- Book titles and author names are stored as simple delimited text; the `|` character is not supported.

## Conclusion

The project demonstrates how a browser-based interface can communicate with a C++ backend through HTTP APIs. It combines OOP, file handling, data management and frontend development in one small application.
