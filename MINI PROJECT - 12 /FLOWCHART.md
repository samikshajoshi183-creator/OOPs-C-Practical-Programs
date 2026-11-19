# Library Management System – Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[Start C++ backend server]
    B --> C[Open web application]
    C --> D[Load book records from library.csv]
    D --> E[Display dashboard and book list]
    E --> F{User action}

    F -->|Add book| G[Enter title and author]
    G --> H{Valid input?}
    H -->|No| I[Display validation error]
    H -->|Yes| J[Create book record and save file]
    J --> E

    F -->|Search / Refresh| K[Fetch and filter book records]
    K --> E

    F -->|Issue book| L{Book exists and available?}
    L -->|No| M[Display error message]
    L -->|Yes| N[Mark book as issued and save]
    N --> E

    F -->|Return book| O{Book exists and is issued?}
    O -->|No| M
    O -->|Yes| P[Mark book as available and save]
    P --> E

    F -->|Delete book| Q{Book exists?}
    Q -->|No| M
    Q -->|Yes| R[Remove record and save file]
    R --> E

    F -->|Exit| S([Stop server])
    I --> E
    M --> E
```

## Flow explanation

1. The C++ server starts and loads previously saved book records.
2. The browser requests the current list of books from the backend.
3. The user can add, search, issue, return or delete a book.
4. The backend validates each operation and updates the data file.
5. The frontend refreshes the catalogue and dashboard after a successful operation.
