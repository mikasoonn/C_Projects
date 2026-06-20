💬 Multi-Client Real-Time Chat Application
A simple and fast command-line chat application built from scratch in C. It allows multiple users to join a single chat room and message each other instantly.

This project uses a Multi-Threaded Server to handle multiple users and an Epoll-powered Client to handle typing and receiving messages at the same time.

⚙️ How It Works
The Server 🖥️: Every time a new client connects, the server spawns a new thread (pthread) just for that user. To prevent users from crashing into each other when joining or leaving, the server uses a Mutex Lock to keep the client list safe and synchronized.

The Client 📱: Instead of getting stuck waiting for you to type, the client uses Linux epoll. This allows the program to watch your keyboard (so you can type) AND watch the server (so you can receive messages) at the exact same time.

✨ Features
👥 Multi-User Support: Up to 100 people can join the chat room simultaneously.

⚡ Instant Broadcasting: When you send a message, everyone else in the chat receives it instantly.

🔄 Smart Asynchronous I/O: You can receive incoming messages even while you are halfway through typing your own message.

🔒 Safe Memory Management: Uses mutexes to prevent bugs when multiple users join or leave at the exact same moment.

📁 Project Structure
Here is how the project files are organized:
├── server.c          # The multi-threaded server source code
├── client.c          # The epoll-powered client source code
├── chat_server       # Executable server binary (generated after compilation)
└── chat_client       # Executable client binary (generated after compilation)