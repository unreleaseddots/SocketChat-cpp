# 🔌 SocketChat-cpp

Um chat simples entre cliente e servidor usando sockets TCP em C++.

> 💡 Projeto criado para estudo de programação de baixo nível com sockets, voltado para aprendizado em redes, comunicação TCP e C++ moderno.  
> Testado em Raspberry Pi (DietPi) via SSH.

---

## 🚀 Funcionalidades

- Comunicação via TCP usando `socket()`, `bind()`, `listen()`, `accept()`, `connect()`
- Chat entre um cliente e um servidor
- Totalmente funcional via localhost ou IP local
- Código comentado, leve e direto

---


---

## 🛠️ Como compilar

No Linux:
```bash
g++ server.cpp -o server
g++ client.cpp -o client
