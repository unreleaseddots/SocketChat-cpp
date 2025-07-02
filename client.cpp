#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
	int sock = 0; //
	struct sockaddr_in serv_addr; // Estrutura de endereço do servidor
	char buffer[BUFFER_SIZE];

	//criar socket
	sock = socket(AF_INET, SOCK_STREAM, 0); // AF_INET == IPv4, SOCK_STREAM == TCP
	if (sock < 0) {
		perror("Erro ao criar o socket");
		return 1;
	}
	//configurar o endereço do servidor
	serv_addr.sin_family = AF_INET; // AF_INET é a família de endereços IPv4
	serv_addr.sin_port = htons(PORT); // htons converte a porta para a ordem de bytes da rede (big-endian).

	//converter o endereço IP 127.0.0.1 para localhost.
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { // inet_pton converte o endereço IP de string para binário
		perror("Endereco IP invalido ou nao suportado");
		return 1;
	}

	//conectar ao servidor
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){  // connect conecta o socket ao servidor.  struct serve para o endereço do servidor, sizeof(serv_addr) é o tamanho do endereço do servidor. caso todos sejam menor que zero da erro
		perror("Erro ao conectar ao servidor");
		return 1;
	}
	
	std::cout << "Conectado ao servidor na porta: " << PORT << std::endl;

	//loop de envio
	while (true) {
		std::string input; // Variável para armazenar a entrada do usuário a variavel é input
		std::cout << ">..."; // Exibe o prompt para o usuário
		std::getline(std::cin, input);	 // Lê a entrada do usuário
		if (input == "sair") break; // Se o usuário digitar "sair", o loop é interrompido
		send(sock, input.c_str(), input.size(), 0); // Envia a entrada do usuário para o servidor
	}

	close(sock); // Fecha o socket
	std::cout << "Conexão encerrada." << std::endl; // Exibe mensagem de encerramento
	return 0; // Retorna 0 para indicar que o programa terminou com sucesso
}