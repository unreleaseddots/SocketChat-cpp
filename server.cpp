#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
	int server_fd, new_socket; // Um novo socket para o servidor e outro para o novo socket
	struct sockaddr_in address; // Estrutura de endereço
	char buffer[BUFFER_SIZE]; //Buffer == segura os dados
	int opt = 1; //Opção para o socket
	int addrlen = sizeof(address); //Tamanho do endereço  #tudo usando a biblioteca sys/socket.h pesquisar mais tarde

	//Criar o socket agora, socket é um ponto final de comunicação.

	server_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET == IPv4, SOCK_STREAM == TCP, e o 0 é o protocolo padrão da família de protocolos.
	if (server_fd == 0) {
		perror("Nao foi possivel criar o socket");
		exit(EXIT_FAILURE);
	}
	//Reusar o porta.

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Configura o socket para reusar o endereço, setckopt é usado para configurar opções de socket. server_fd é o socket que estamos configurando, SOL_SOCKET é o nível do socket, SO_REUSEADDR é a opção que estamos configurando para reusar, &opt é o valor da opção e sizeof(opt) é o tamanho da opção.

	//Definir informaçoes do socket

	address.sin_family = AF_INET; // AF_INET é a família de endereços IPv4. caso eu queira usar IPv6, eu usaria AF_INET6.
	address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY significa que o socket irá aceitar conexões de qualquer endereço IP.
	address.sin_port = htons(PORT); //escolher a porta , htons converte a porta para a ordem de bytes da rede (big-endian).

	//associas porta ao socket

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { //bind binda o socket ao endereço e porta especificados. oserver_fd é o socket que estamos associando, (struct sockaddr*)&address é o endereço e porta que estamos associando, sizeof(address) é o tamanho do endereço. o tamanho do endereço é necessário para que o sistema saiba quanto espaço alocar para o endereço. geralmente ele tem um tamanho fixo, mas é uma boa prática passar o tamanho do endereço para a função bind. o tamanho padrao é 16 bytes para IPv4, mas pode variar dependendo da implementação e do sistema operacional.
		perror("Erro ao bindar o socket");
		exit(EXIT_FAILURE);
	}

	//começar a escutar conexões

	if (listen(server_fd, 3) < 0) { //listen escuta o server_fd que foi setado  anteriormente no codigo com o int server_fd, o 3 é o numero maximo de conexoes que ele consegue manter ao mesmo tempo, o listen é usado para colocar o socket em modo de escuta, ou seja, ele fica esperando conexões de clientes. se a função listen falhar, ela retorna um valor negativo e o programa imprime uma mensagem de erro e sai.
		perror("Erro ao escutar o socket");
		exit(EXIT_FAILURE);
	}

	std::cout << "Servidor escutando na porta: " << PORT << std::endl;

	//Aceitar a conexao de um cliente.

	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen); //aceita a conexao do cliente em  um socket novo, o accept é usado para aceitar uma conexão de um cliente. o server_fd é o socket que está escutando, (struct sockaddr*)&address é o endereço do cliente que está se conectando, (socklen_t*)&addrlen é o tamanho do endereço do cliente. se a função accept falhar, ela retorna um valor negativo e o programa imprime uma mensagem de erro e sai.
	if (new_socket < 0) {
		perror("Erro no accept: verificar se o cliente esta conectado");
		exit(EXIT_FAILURE);
	}

	std::cout << "Cliente conectado." << std::endl;

	//loop de mensagens

	while (true) {
		memset(buffer, 0, BUFFER_SIZE);
		int bytes = read(new_socket, buffer, BUFFER_SIZE); // le os dados do cliente, o read é usado para ler os dados do socket. o new_socket é o socket que estamos lendo, buffer é o buffer onde os dados serão armazenados e BUFFER_SIZE é o tamanho do buffer. se a função read falhar, ela retorna um valor negativo e o programa imprime uma mensagem de erro e sai.
		if (bytes < 0) {
			std::cout << "cliente foi desconectado. \n";
			break;
		}
		std::cout << "mensagem recebida: " << buffer << std::endl;
	}
	close(new_socket); // fecha o socket do cliente
	close(server_fd); // fecha o socket do servidor
	return 0; // retorna 0 para indicar que o programa terminou com sucesso
}