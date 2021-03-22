#ifndef ESP_NOW_CONTROLLER_H_
#define ESP_NOW_CONTROLLER_H_

#include <esp_now.h>
#include <WiFi.h>

class EspNowController
{
public:
    EspNowController(const uint8_t channel);
    virtual ~EspNowController(void);

    bool begin(void);
    esp_err_t send(const uint8_t *data, const size_t len);

protected:
    void initEspNow(void);
    void initPeer(const uint8_t *addr, esp_now_peer_info_t &peer);

private:
    const uint8_t ESP_NOW_BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN];
    const uint8_t CHANNEL;
    esp_now_peer_info_t broadcastPeer;
};

#endif