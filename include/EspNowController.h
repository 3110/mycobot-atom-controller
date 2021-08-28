#ifndef ESP_NOW_CONTROLLER_H_
#define ESP_NOW_CONTROLLER_H_

#include <WiFi.h>
#include <esp_now.h>

class EspNowController {
public:
    EspNowController(uint8_t channel);
    virtual ~EspNowController(void);

    bool begin(void);
    esp_err_t send(const uint8_t *data, const size_t len);

protected:
    void initEspNow(void);
    void initPeer(const char *addr, esp_now_peer_info_t &peer);

private:
    const char ESP_NOW_BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN];
    const uint8_t CHANNEL;
    esp_now_peer_info_t broadcastPeer;
};

#endif