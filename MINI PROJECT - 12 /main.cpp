#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

struct Book {
    int id;
    string title, author;
    bool issued;
};

vector<Book> books;
mutex booksMutex;
const string DATA_FILE = "library.csv";

string jsonEscape(const string& s) {
    string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

string getJsonString(const string& body, const string& key) {
    string needle = "\"" + key + "\"";
    size_t p = body.find(needle);
    if (p == string::npos) return "";
    p = body.find(':', p + needle.size());
    if (p == string::npos) return "";
    p = body.find('"', p + 1);
    if (p == string::npos) return "";
    ++p;
    string value;
    bool escaped = false;
    for (; p < body.size(); ++p) {
        char c = body[p];
        if (escaped) {
            if (c == 'n') value += '\n';
            else if (c == 'r') value += '\r';
            else value += c;
            escaped = false;
        } else if (c == '\\') escaped = true;
        else if (c == '"') break;
        else value += c;
    }
    return value;
}

void loadBooks() {
    ifstream in(DATA_FILE);
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, title, author, issued;
        if (getline(ss, id, '|') && getline(ss, title, '|') &&
            getline(ss, author, '|') && getline(ss, issued, '|')) {
            try { books.push_back({stoi(id), title, author, issued == "1"}); }
            catch (...) {}
        }
    }
}

void saveBooks() {
    ofstream out(DATA_FILE, ios::trunc);
    for (const auto& b : books)
        out << b.id << '|' << b.title << '|' << b.author << '|'
            << (b.issued ? 1 : 0) << '\n';
}

string booksJson() {
    lock_guard<mutex> lock(booksMutex);
    string out = "[";
    for (size_t i = 0; i < books.size(); ++i) {
        const auto& b = books[i];
        if (i) out += ",";
        out += "{\"id\":" + to_string(b.id) +
               ",\"title\":\"" + jsonEscape(b.title) +
               "\",\"author\":\"" + jsonEscape(b.author) +
               "\",\"issued\":" + string(b.issued ? "true" : "false") + "}";
    }
    return out + "]";
}

string makeResponse(const string& status, const string& type, const string& body) {
    stringstream ss;
    ss << "HTTP/1.1 " << status << "\r\n"
       << "Content-Type: " << type << "; charset=utf-8\r\n"
       << "Access-Control-Allow-Origin: *\r\n"
       << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
       << "Access-Control-Allow-Headers: Content-Type\r\n"
       << "Content-Length: " << body.size() << "\r\n"
       << "Connection: close\r\n\r\n" << body;
    return ss.str();
}

string readFile(const string& path) {
    ifstream in(path, ios::binary);
    if (!in) return "";
    stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

void sendAll(SOCKET client, const string& data) {
    size_t sent = 0;
    while (sent < data.size()) {
        int n = send(client, data.c_str() + sent,
                     static_cast<int>(data.size() - sent), 0);
        if (n == SOCKET_ERROR || n == 0) break;
        sent += n;
    }
}

void handleClient(SOCKET client) {
    string request;
    char buffer[4096];
    size_t headerEnd = string::npos;
    size_t contentLength = 0;

    while (true) {
        int n = recv(client, buffer, sizeof(buffer), 0);
        if (n <= 0) break;
        request.append(buffer, n);
        headerEnd = request.find("\r\n\r\n");
        if (headerEnd != string::npos) {
            string headers = request.substr(0, headerEnd);
            size_t cl = headers.find("Content-Length:");
            if (cl != string::npos) {
                cl += 15;
                while (cl < headers.size() && headers[cl] == ' ') ++cl;
                try { contentLength = stoul(headers.substr(cl)); } catch (...) {}
            }
            if (request.size() >= headerEnd + 4 + contentLength) break;
        }
    }

    if (headerEnd == string::npos) {
        sendAll(client, makeResponse("400 Bad Request", "application/json", "{\"error\":\"Bad request\"}"));
        closesocket(client);
        return;
    }

    string headers = request.substr(0, headerEnd);
    string body = request.substr(headerEnd + 4, contentLength);
    stringstream hs(headers);
    string method, path, version;
    hs >> method >> path >> version;

    string result;
    if (method == "OPTIONS") {
        result = makeResponse("204 No Content", "text/plain", "");
    } else if (path == "/api/books" && method == "GET") {
        result = makeResponse("200 OK", "application/json", booksJson());
    } else if (path == "/api/books" && method == "POST") {
        string title = getJsonString(body, "title");
        string author = getJsonString(body, "author");
        if (title.empty() || author.empty() ||
            title.find('|') != string::npos || author.find('|') != string::npos) {
            result = makeResponse("400 Bad Request", "application/json",
                "{\"error\":\"Enter a title and author. The | character is not allowed.\"}");
        } else {
            lock_guard<mutex> lock(booksMutex);
            int nextId = 1;
            for (const auto& b : books) nextId = max(nextId, b.id + 1);
            books.push_back({nextId, title, author, false});
            saveBooks();
            result = makeResponse("201 Created", "application/json",
                "{\"message\":\"Book added\",\"id\":" + to_string(nextId) + "}");
        }
    } else if (path.rfind("/api/books/", 0) == 0 &&
               (method == "PUT" || method == "DELETE")) {
        string tail = path.substr(11);
        string action;
        size_t slash = tail.find('/');
        if (slash != string::npos) {
            action = tail.substr(slash + 1);
            tail = tail.substr(0, slash);
        }
        int id = 0;
        try { id = stoi(tail); } catch (...) {}
        lock_guard<mutex> lock(booksMutex);
        auto it = find_if(books.begin(), books.end(),
            [id](const Book& b) { return b.id == id; });

        if (it == books.end()) {
            result = makeResponse("404 Not Found", "application/json", "{\"error\":\"Book not found\"}");
        } else if (method == "DELETE" && action.empty()) {
            books.erase(it);
            saveBooks();
            result = makeResponse("200 OK", "application/json", "{\"message\":\"Book deleted\"}");
        } else if (method == "PUT" && (action == "issue" || action == "return")) {
            if (action == "issue" && it->issued) {
                result = makeResponse("409 Conflict", "application/json", "{\"error\":\"This book is already issued\"}");
            } else if (action == "return" && !it->issued) {
                result = makeResponse("409 Conflict", "application/json", "{\"error\":\"This book is not currently issued\"}");
            } else {
                it->issued = (action == "issue");
                saveBooks();
                result = makeResponse("200 OK", "application/json",
                    string("{\"message\":\"Book ") + (it->issued ? "issued" : "returned") + " successfully\"}");
            }
        } else {
            result = makeResponse("404 Not Found", "application/json", "{\"error\":\"Unknown action\"}");
        }
    } else if (method == "GET") {
        string file = path == "/" || path == "/index.html" ? "index.html" :
                      path == "/style.css" ? "style.css" :
                      path == "/script.js" ? "script.js" : "";
        if (file.empty()) {
            result = makeResponse("404 Not Found", "text/plain", "Not found");
        } else {
            string data = readFile(file);
            if (data.empty()) result = makeResponse("404 Not Found", "text/plain", "File not found");
            else {
                string type = file == "index.html" ? "text/html" :
                              file == "style.css" ? "text/css" : "application/javascript";
                result = makeResponse("200 OK", type, data);
            }
        }
    } else {
        result = makeResponse("404 Not Found", "application/json", "{\"error\":\"Route not found\"}");
    }

    sendAll(client, result);
    closesocket(client);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock initialization failed.\n";
        return 1;
    }

    loadBooks();

    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        cerr << "Could not create server socket.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(8080);

    if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        cerr << "Port 8080 is busy. Close the other server and try again.\n";
        closesocket(server);
        WSACleanup();
        return 1;
    }

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Could not start server.\n";
        closesocket(server);
        WSACleanup();
        return 1;
    }

    cout << "Library Management System running at http://localhost:8080\n";
    cout << "Keep this window open. Press Ctrl+C to stop.\n";

    while (true) {
        SOCKET client = accept(server, nullptr, nullptr);
        if (client != INVALID_SOCKET)
            thread(handleClient, client).detach();
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
