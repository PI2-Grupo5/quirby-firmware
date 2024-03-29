#include <Http.h>

void Http::run(void)
{
    wifiState = Wifi.GetState();

    switch (wifiState)
    {
    case WIFI::Wifi::state_e::READY_TO_CONNECT:
        std::cout << "Wifi Status: READY_TO_CONNECT\n";
        Wifi.Begin();
        break;
    case WIFI::Wifi::state_e::DISCONNECTED:
        std::cout << "Wifi Status: DISCONNECTED\n";
        Wifi.Begin();
        break;
    case WIFI::Wifi::state_e::CONNECTING:
        std::cout << "Wifi Status: CONNECTING\n";
        break;
    case WIFI::Wifi::state_e::WAITING_FOR_IP:
        std::cout << "Wifi Status: WAITING_FOR_IP\n";
        break;
    case WIFI::Wifi::state_e::ERROR:
        std::cout << "Wifi Status: ERROR\n";
        break;
    case WIFI::Wifi::state_e::CONNECTED:
        std::cout << "Wifi Status: CONNECTED\n";
		// testServer();
        break;
    case WIFI::Wifi::state_e::NOT_INITIALIZED:
        std::cout << "Wifi Status: NOT_INITIALIZED\n";
        break;
    case WIFI::Wifi::state_e::INITIALIZED:
        std::cout << "Wifi Status: INITIALIZED\n";
        break;
    }
}

void Http::TrataClienteTCP(int socketCliente) {
	char buffer[256];
	int tamanhoRecebido;

	if((tamanhoRecebido = recv(socketCliente, buffer, 256, 0)) < 0)
		printf("Erro no recv()\n");
    else 
        printf("Valor lido: %s", buffer);
}

// void Http::testServer(void) {
// 	int servidorSocket;
// 	int clienteSocket;
// 	struct sockaddr_in servidorAddr;
// 	struct sockaddr_in clienteAddr;
// 	unsigned short servidorPorta = 16188;
// 	unsigned int clienteLength;

// 	while(1) {
// 		// Criar Socket
// 		if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
// 			printf("Erro no socket()\n");

// 		// Construir struct sockaddr_in
// 		memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
// 		servidorAddr.sin_family = AF_INET;
// 		servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
// 		servidorAddr.sin_port = htons(servidorPorta);

// 	    // Bind
// 	    if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
// 		printf("Falha no Bind\n");

// 	    // Listen
// 	    if(listen(servidorSocket, 10) < 0)
// 		printf("Falha no Listen\n");	

//         while(1) {
//             clienteLength = sizeof(clienteAddr);
//             if((clienteSocket = accept(servidorSocket, 
//                                     (struct sockaddr *) &clienteAddr, 
//                                     &clienteLength)) < 0)
//                 printf("Falha no Accept\n");
            
//             printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
            
//             TrataClienteTCP(clienteSocket);
//             close(clienteSocket);
//         }

// 		close(servidorSocket);

//         ESP_LOGI( "TCP", "Fechando socket de conexao!");
//         vTaskDelay(pdMS_TO_TICKS(10000));
// 	}


// }

void Http::setup(void)
{
    esp_event_loop_create_default();
    nvs_flash_init();

    Wifi.SetCredentials(WIFI_SSID, WIFI_PASS);
    Wifi.Init();
}
