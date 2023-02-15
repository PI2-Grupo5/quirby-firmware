#include "http_client.h"

#define MAX_HTTP_OUTPUT_BUFFER 1024
#define TAG "HTTP"

esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch((int)evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

void http_request(){

    long int valorLouco = 0;
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    char buffer[255];
    int valueRes = 0;

    esp_http_client_config_t config = {
        .url = "http://quirby-api.up.railway.app/robot/7",
        .event_handler = _http_event_handle,
        .user_data = local_response_buffer,
        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);

    valueRes = esp_http_client_read(client, buffer, 255);

    std::cout <<" comeca aqui: " << valueRes << " show? ->" << buffer << '\n';

    if (err == ESP_OK) {
    ESP_LOGI(TAG, "Status = %d, content_length = %"PRIu64"",
            esp_http_client_get_status_code(client),
            esp_http_client_get_content_length(client));
    } else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);

    ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
}

void http_put(){

    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    std::string jsonMesssage = "{\"nameRobot\":\"Nome\",\"functionMode\":\"Estevao\",\"batteryStatus\":30,\"blockedAlert\":\"false\",\"powerState\":false,\"controlAccess\":1,\"cleaningSchedule\":[\"2020-02-25T20:10:01.000Z\",\"2020-02-25T10:10:01.000Z\"]}";

    esp_http_client_config_t config = {
        .url = "http://quirby-api.up.railway.app/robot/7",
        .event_handler = _http_event_handle,
        .user_data = local_response_buffer,
        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    //PUT
    esp_http_client_set_url(client, "http://quirby-api.up.railway.app/robot/7");
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_method(client, HTTP_METHOD_PUT);
    esp_http_client_set_post_field(client, jsonMesssage.c_str(), jsonMesssage.size());
    err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP PUT Status = %d, content_length = %"PRIu64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP PUT request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);

}

void http_patch(){

    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    std::string jsonMesssage = "{\"batteryStatus\":50,\"blockedAlert\":\"false\"}";

    esp_http_client_config_t config = {
        .url = "http://quirby-api.up.railway.app/robot/7",
        .event_handler = _http_event_handle,
        .user_data = local_response_buffer,
        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    //PUT
    esp_http_client_set_url(client, "http://quirby-api.up.railway.app/robot/7");
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_method(client, HTTP_METHOD_PATCH);
    esp_http_client_set_post_field(client, jsonMesssage.c_str(), jsonMesssage.size());
    err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP PUT Status = %d, content_length = %"PRIu64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP PUT request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);

}
