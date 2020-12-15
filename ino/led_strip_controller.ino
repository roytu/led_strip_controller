#include "WiFi.h"
#include "AsyncUDP.h"

uint8_t ledR = 12;
uint8_t ledG = 13;
uint8_t ledB = 14;

const char* ssid = "******************";
const char* password = "*****************";

uint8_t rbuf[1024];
uint8_t gbuf[1024];
uint8_t bbuf[1024];

AsyncUDP udp;

int t = 0;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 20);
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);


void setup()
{
    memset(rbuf, 0, 1024);
    memset(gbuf, 0, 1024);
    memset(bbuf, 0, 1024);

    t = 0;
    ledcAttachPin(ledR, 1);
    ledcAttachPin(ledG, 2);
    ledcAttachPin(ledB, 3);

    ledcSetup(1, 12000, 8);
    ledcSetup(2, 12000, 8);
    ledcSetup(3, 12000, 8);

    Serial.begin(115200);
    Serial.println("Testing...");

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    if(udp.listen(1234)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            parsePacket(packet.data(), packet.length());
            /*
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            parsePacket(packet.data(), packet.length());
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());
            */
        });
    }
}

void loop()
{
    ledcWrite(1, rbuf[t]);
    ledcWrite(2, gbuf[t]);
    ledcWrite(3, bbuf[t]);
    t++;
    if (t >= 1024) {
        t = 0;
    }
    delay(10);
}

// PROTOCOL
// ---------------------------------------
// | COMMAND (1 byte) | PARAMS (x bytes) |
// ---------------------------------------
//
// Commands:
// 
// r (1) / start (2) / data (bytes)
// g (1) / start (2) / data (bytes)
// b (1) / start (2) / data (bytes)
// c (1) / rperiod (4) / gperiod (4) / bperiod (4)

void parsePacket(uint8_t* data, uint32_t length) {
    if (length <= 0) {
        return;
    }

    uint8_t command = data[0];
    switch (command) {
        case 'r': {
            if (length < 3) {
                Serial.println("[R] Bad packet; invalid length");
                return;
            }

            uint16_t start = (data[1] << 8) | data[2];

            if (start + (length - 3) >= 1024) {
                Serial.println("[R] Warning: overflow");
            }

            Serial.print("[R] Setting buffer from ");
            Serial.print(start);
            Serial.print(" with length ");
            Serial.println(length);
            memcpy(rbuf + start, data + 1, length - 1);
        }
        break;
        case 'g': {
            if (length < 3) {
                Serial.println("[G] Bad packet; invalid length");
                return;
            }

            uint16_t start = (data[1] << 8) | data[2];

            if (start + (length - 3) >= 1024) {
                Serial.println("[G] Warning: overflow");
            }

            Serial.print("[G] Setting buffer from ");
            Serial.print(start);
            Serial.print(" with length ");
            Serial.println(length);
            memcpy(gbuf + start, data + 1, length - 1);
        }
        break;
        case 'b': {
            if (length < 3) {
                Serial.println("[B] Bad packet; invalid length");
                return;
            }

            uint16_t start = (data[1] << 8) | data[2];

            if (start + (length - 3) >= 1024) {
                Serial.println("[B] Warning: overflow");
            }

            Serial.print("[B] Setting buffer from ");
            Serial.print(start);
            Serial.print(" with length ");
            Serial.println(length);
            memcpy(bbuf + start, data + 1, length - 1);
        }
        break;
    }
}
