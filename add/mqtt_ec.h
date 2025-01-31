/*
 * @Author: your name
 * @Date: 2020-04-22 09:16:33
 * @LastEditTime: 2020-06-18 14:15:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \AQM30JiaXing6-3\PubSubClient.hpp
 */
/*
PubSubClient Header File
2018.10.16
Li Shujian
Jecinfo
*/





#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4

// MQTT_VERSION : Pick the version
#define MQTT_VERSION MQTT_VERSION_3_1

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#define MQTT_MAX_PACKET_SIZE 500

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#define MQTT_KEEPALIVE 20

// MQTT_SOCKET_TIMEOUT: socket timeout interval in Seconds
#define MQTT_SOCKET_TIMEOUT 20

// Possible values for client.state()
#define MQTT_CONNECTION_TIMEOUT     -4
#define MQTT_CONNECTION_LOST        -3
#define MQTT_CONNECT_FAILED         -2
#define MQTT_DISCONNECTED           -1
#define MQTT_CONNECTED               0
#define MQTT_CONNECT_BAD_PROTOCOL    1
#define MQTT_CONNECT_BAD_CLIENT_ID   2
#define MQTT_CONNECT_UNAVAILABLE     3
#define MQTT_CONNECT_BAD_CREDENTIALS 4
#define MQTT_CONNECT_UNAUTHORIZED    5

#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)

short int write( char header, char* buf,   unsigned  short int length);
unsigned  short int writeString(char* string, char* buf,   unsigned  short int pos);
unsigned char mqtt_connect(char* id, char* imei,  char* rssi, char* user, char* pass);
unsigned char publish( char* topic, unsigned char Dup, char Qos, char MessageID, unsigned char Retain, char* payload,short int l);
void disconnect(void) ;
unsigned char unsubscribe( char* topic) ;
short int subscribe( char* topic,unsigned short int topiclen, unsigned char qos, char* payload) ;
unsigned char asctou8(unsigned char *head);
