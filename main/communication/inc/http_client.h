#pragma once

#include <iostream>
#include <string>
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

void http_request();
void http_put();
void http_patch();
