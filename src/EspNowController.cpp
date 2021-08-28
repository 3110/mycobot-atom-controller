#include "EspNowController.h"

EspNowController::EspNowController(uint8_t channel) : ESP_NOW_BROADCAST_ADDRESS{
                                                                0xFF,
                                                                0xFF,
                                                                0xFF,
                                                                0xFF,
                                                                0xFF,
                                                                0xFF,
                                                            },
                                                            CHANNEL(channel)
{
}

EspNowController::~EspNowController(void) {
}

bool EspNowController::begin(void) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    initEspNow();
    initPeer(ESP_NOW_BROADCAST_ADDRESS, broadcastPeer);
    return esp_now_add_peer(&broadcastPeer) == ESP_OK;
}
esp_err_t EspNowController::send(const uint8_t *data, const size_t len) {
    return esp_now_send(broadcastPeer.peer_addr, data, len);
}

void EspNowController::initEspNow(void) {
    if (esp_now_init() != ESP_OK) {
        ESP.restart();
    }
}

void EspNowController::initPeer(const char *addr, esp_now_peer_info_t &peer) {
    memset(&peer, 0, sizeof(peer));
    memcpy(&(peer.peer_addr), addr, ESP_NOW_ETH_ALEN);
    peer.channel = CHANNEL;
    peer.encrypt = 0;
}
