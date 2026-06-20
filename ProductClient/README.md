
# 🚀 Multi-Threaded TCP Product Database Server

A simple client-server application written in **C** using **POSIX sockets** and **pthreads**. The server stores products in a text database and allows multiple clients to connect simultaneously.

---

## ✨ Features

🔹 Multi-threaded server architecture using `pthread`

🔹 Concurrent client handling

🔹 Thread-safe file operations with mutex locking

🔹 Persistent product storage in `database.txt`

🔹 TCP socket communication

🔹 Simple and lightweight custom protocol

🔹 Linux system programming practice

---

## 📂 Project Structure

```text
.
├── server.c
├── client.c
├── database.txt
└── README.md
```

---

##  Supported Commands

### Add a Product

```text
POST Apple
```

 Response:

```text
Product successfully added
```

###  Search for a Product

```text
GET Apple
```

 Response:

```text
Apple
```

 If not found:

```text
NOT FOUND
```

###  Retrieve All Products

```text
GET ALL
```

 Example Response:

```text
Apple
Orange
Banana
Milk
```

---

## 🧵 How It Works

### 🖥️ Server

• Creates a TCP socket and listens on port **8080**

• Accepts incoming client connections

• Creates a separate thread for each connected client

• Uses a mutex to ensure safe concurrent access to the database

• Processes client requests and sends appropriate responses

### 💻 Client

• Connects to the server using TCP

• Sends a user-provided request

• Receives and displays the server response

• Supports product insertion, search, and retrieval operations

---

## 🔒 Thread Safety

The server uses:

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

to ensure that multiple clients cannot access and modify the database simultaneously, preventing race conditions and data corruption.

---


