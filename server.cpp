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
	struct sockaddr_in address; // Estrutura de endere�o
	char buffer[BUFFER_SIZE]; //Buffer == segura os dados
	int opt = 1; //Op��o para o socket
	int addrlen = sizeof(address); //Tamanho do endere�o  #tudo usando a biblioteca sys/socket.h pesquisar mais tarde

	//Criar o socket agora, socket � um ponto final de comunica��o.

	server_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET == IPv4, SOCK_STREAM == TCP, e o 0 � o protocolo padr�o da fam�lia de protocolos.
	if (server_fd == 0) {
		perror("Nao foi possivel criar o socket");
		exit(EXIT_FAILURE);
	}
	//Reusar o porta.

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Configura o socket para reusar o endere�o, setckopt � usado para configurar op��es de socket. server_fd � o socket que estamos configurando, SOL_SOCKET � o n�vel do socket, SO_REUSEADDR � a op��o que estamos configurando para reusar, &opt � o valor da op��o e sizeof(opt) � o tamanho da op��o.

	//Definir informa�oes do socket

	address.sin_family = AF_INET; // AF_INET � a fam�lia de endere�os IPv4. caso eu queira usar IPv6, eu usaria AF_INET6.
	address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY significa que o socket ir� aceitar conex�es de qualquer endere�o IP.
	address.sin_port = htons(PORT); //escolher a porta , htons converte a porta para a ordem de bytes da rede (big-endian).

	//associas porta ao socket

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { //bind binda o socket ao endere�o e porta especificados. oserver_fd � o socket que estamos associando, (struct sockaddr*)&address � o endere�o e porta que estamos associando, sizeof(address) � o tamanho do endere�o. o tamanho do endere�o � necess�rio para que o sistema saiba quanto espa�o alocar para o endere�o. geralmente ele tem um tamanho fixo, mas � uma boa pr�tica passar o tamanho do endere�o para a fun��o bind. o tamanho padrao � 16 bytes para IPv4, mas pode variar dependendo da implementa��o e do sistema operacional.
		perror("Erro ao bindar o socket");
		exit(EXIT_FAILURE);
	}

	//come�ar a escutar conex�es

	if (listen(server_fd, 3) < 0) { //listen escuta o server_fd que foi setado  anteriormente no codigo com o int server_fd, o 3 � o numero maximo de conexoes que ele consegue manter ao mesmo tempo, o listen � usado para colocar o socket em modo de escuta, ou seja, ele fica esperando conex�es de clientes. se a fun��o listen falhar, ela retorna um valor negativo e o programa imprime uma mensagem de erro e sai.
		perror("Erro ao escutar o socket");
		exit(EXIT_FAILURE);
	}

	std::cout << "Servidor escutando na porta: " << PORT << std::endl;

	//Aceitar a conexao de um cliente.

	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen); //aceita a conexao do cliente em  um socket novo, o accept � usado para aceitar uma conex�o de um cliente. o server_fd � o socket que est� escutando, (struct sockaddr*)&address � o endere�o do cliente que est� se conectando, (socklen_t*)&addrlen � o tamanho do endere�o do cliente. se a fun��o accept falhar, ela retorna um valor negativo e o programa imprime uma mensagem de erro e sai.
	if (new_socket < 0) {
		perror("Erro no accept: verificar se o cliente esta conectado");
		exit(EXIT_FAILURE);
	}

	std::cout << "Cliente conectado." << std::endl;

	//loop de mensagens

	while (true) {
		memset(buffer, 0, BUFFER_SIZE);
		int bytes = read(new_socket, buffer, BUFFER_SIZE); // le os dados do cliente, o read � usado para ler os dados do socket. o new_socket � o socket que estamos lendo, buffer � o buffer onde os dados ser�o armazenados e BUFFER_SIZE � o tamanho do buffer. se a fun��o read falhar, ela retorna um valor negativo e o programa imprime uma mensagem de erro e sai.
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