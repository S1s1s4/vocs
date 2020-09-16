#include "mqtt_ec.h"
#include "uart.h"
#include "ec20.h"
#include <String.h>
#include "mytime.h"
#include "stdio.h"
char buffer[MQTT_MAX_PACKET_SIZE];
int _state;
unsigned short int nextMsgId;
unsigned char mqtt_connect(char* id, char* imei,  char* rssi, char* user, char* pass)
{
    //bool result = false;

    unsigned char length = 5;
    unsigned char j;
    short int len;
    char v;

#if MQTT_VERSION == MQTT_VERSION_3_1
    char d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
    char d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
    memset(buffer, 0, MQTT_MAX_PACKET_SIZE);
    // printf("\r\n@@@@ Connect server");

    // result = Connected(fd, ip, port, imei, imsi, rssi);   //取出来


    if (1)
    {
        // Leave room in the buffer for header and variable length field
        for (j = 0; j<MQTT_HEADER_VERSION_LENGTH; j++)
        {
            buffer[length++] = d[j];
        }


        v = 0x02;

        if(user != NULL)
        {
            v = v|0x80;
            if(pass != NULL)
            {
                v = v|(0x80>>1);
            }
        }

        buffer[length++] = v;

        buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
        buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);
        sprintf(id, "Jecinfo%s", imei);
        //printf("\r\nid %s", id);
        length = writeString(id, buffer, length);

        if(user != NULL)
        {
            length = writeString(user, buffer, length);
            if(pass != NULL)
            {
                length = writeString(pass, buffer, length);
            }
        }
        len = write(MQTTCONNECT, buffer, length-5);
        //printf("\r\nlen = %d", len);
        if (len > 0)
        {
            //printf("\r\nlen = %d,%x %x %x %x", len, buffer[0], buffer[1], buffer[2], buffer[3]);
            if ((buffer[0] == MQTTCONNACK)&&(buffer[3] == 0x00))
            {
                _state = MQTT_CONNECTED;
                //printf("\r\n@@@@ Connect server ok");
                return 1;
            }
        }
    }
    //printf("\r\n#### Connect server error");
    _state = MQTT_DISCONNECTED;
    return 0;
}

short int mqtt_wr( char* cBuf, unsigned short int usLength)
{
    char cTemp[120];
    unsigned  short int i,j;
    short int si, sPos;

    memset(cTemp, 0, 120);
    i=sprintf(cTemp, "AT+QISEND=0,%d\r\n", usLength);
    USART2SendString((unsigned char*)cTemp,i);
    // sNwrite = write(iFd, cTemp, cLen);

    Msg2RxCount = 0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    //memset(cTemp, 0, 120);
    delay_ms(5000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
    Msg2RxCount=0;
    for (j = 0; j < 30; j++)
        Msg2RxBuf[j] = 0;
    USART2SendString((unsigned char*)cBuf,usLength);
    delay_ms(2000);
    Msg2RxLen = Msg2RxCount+1;
    USART1SendString(Msg2RxBuf,Msg2RxLen);
//    if (Msg2RxLen == usLength)
//    {
    //for (si = 0;si < usLength; si++)
    //{
    //    printf("%02x ", cBuf[si]);
    //}
    //printf("\r\n");
//        memset(cTemp, 0, 120);
//        sleep(5);
//        sNread = read(iFd, cTemp, 120);
    if (Msg2RxCount > 0)
    {
        sPos=FindString((char*)Msg2RxBuf,Msg2RxCount,"SEND OK",7);
        if (sPos > 0)
        {
            sPos=FindString((char*)Msg2RxBuf,Msg2RxCount,"recv",4);
            if (sPos > 0)
            {
                sPos=sPos-1;

                for (si = 0; si < Msg2RxCount-sPos-14; si++)
                {
                    //printf("%02X ", cTemp[sPos+12+si]);
                    cBuf[si] = Msg2RxBuf[(char)sPos+12+si];
                }

                return Msg2RxCount-(char)sPos-14;
            }
            return 0;
        }
    }

    return -1;
}

short int write( char header, char* buf, unsigned  short int length)
{
    char lenBuf[4];
    unsigned char llen = 0;
    unsigned char  digit;
    unsigned char  pos = 0;
    short int rc;
    unsigned short int len=length;
    unsigned  short int i=0;
    do
    {
        digit = len % 128;
        len = len / 128;
        if (len > 0)
        {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
    } while(len>0);

    buf[4-llen] = header;
    for ( i=0; i<llen; i++)  //填入 mqtt buf
    {
        buf[5-llen+i] = lenBuf[i];
    }
    //printf("\r\nlength = %d,%d", length,llen);
    rc = mqtt_wr(buf+4-llen,length+1+llen);
    //printf("\r\nrc = %d", rc);
    if (rc > 0)
    {
        for ( i=0; i<rc; i++)
        {
            buf[i] = buf[i+4-llen];
        }
    }
    return rc;
}


unsigned  short int writeString(char* string, char* buf, unsigned  short int pos)
{
    char* idp = string;
    unsigned  short int i = 0;
    pos += 2;
    while (*idp)
    {
        buf[pos++] = *idp++;
        i++;
    }
    buf[pos-i-2] = (i >> 8);
    buf[pos-i-1] = (i & 0xFF);
    return pos;
}



unsigned char publish( char* topic, unsigned char Dup, char Qos, char MessageID, unsigned char Retain, char* payload,short int l)
{
    unsigned short int plength;
    char header;
    unsigned short int i;
    unsigned short int length ;
    short int len;
    memset(buffer, 0, MQTT_MAX_PACKET_SIZE);
    plength = l;//strlen(payload);
    // printf("\r\n@@@@ Publish data");
    USART1SendString("\r\n@@@@ Publish data",19);
    if (_state == MQTT_CONNECTED)
    {
        if (MQTT_MAX_PACKET_SIZE < (5 + strlen(topic) + 2 + plength))
        {
            // Too long
            //printf("\r\n#### Data too long");
            return 0;
        }
        // Leave room in the buffer for header and variable length field
        length = 5;
        length = writeString(topic,buffer,length);
        if (Qos > 0)
        {
            //Message ID,QOS=1,2
            buffer[length++] = 0x01;
            buffer[length++] = MessageID;
        }

        for (i=0; i<plength; i++)
        {
            buffer[length++] = payload[i];
        }
        header = MQTTPUBLISH;
        if (Dup)
        {
            header |= 0x08;
        }
        if (Retain)
        {
            header |= 0x01;
        }

        if (Qos == 2)
        {
            header |= MQTTQOS2;
            len = write( header,buffer,length-5);
            //printf("\r\nlen = %d", len);
            if (len > 0)
            {
                //printf("\r\nlen = %d,%x %x %x %x", len, buffer[0], buffer[1], buffer[2], buffer[3]);
                if ((buffer[0] == MQTTPUBREC)&&((unsigned char)buffer[3] == MessageID))
                {
                    //printf("\r\n@@@@ Publish ok");
                    return 1;
                }
            }
        }
        else
        {
            len = write( header,buffer,length-5);
            //printf("\r\nlen = %d", len);
            // printf("\r\n@@@@ Publish ok");
            return (len == 0);
        }
    }
    //printf("\r\n#### Publish error");
    return 0;
}


void disconnect(void)
{
    memset(buffer, 0, 5);
//    printf("\r\n@@@@ Disconnect server");
    if (_state == MQTT_CONNECTED)
    {
        buffer[0] = MQTTDISCONNECT;
        buffer[1] = 0x00;
        mqtt_wr( buffer, 2);

        _state = MQTT_DISCONNECTED;
    }
//    printf("\r\n@@@@ Disconnect server ok");
}


short int subscribe( char* topic,unsigned short int topiclen,unsigned char qos, char* payload)
{
    short int sPos = 0,si;

    unsigned short int length = 5;
    short int slen=0;
    char null[] = "";
    memset(buffer, 0, MQTT_MAX_PACKET_SIZE);
    // printf("\r\n@@@@ Subscribe topic");
    if (_state == MQTT_CONNECTED)
    {
        if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic))
        {
            // Too long
            // printf("\r\n#### Data too long");
            return -1;
        }
        // Leave room in the buffer for header and variable length field

        nextMsgId++;
        if (nextMsgId == 0)
        {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, buffer,length);
        buffer[length++] = qos;
        slen = write( MQTTSUBSCRIBE|MQTTQOS1, buffer, length-5);
        if (slen > 0)
        {
            // printf("\r\nslen = %d,%x %x %x %x %x", slen, (uint8_t)buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
            if (((unsigned char)buffer[0] == MQTTSUBACK)&&((unsigned char)buffer[3] == (unsigned char)nextMsgId))
            {
                //printf("\r\n@@@@ Subscribe ok");

                USART1SendString("\r\n@@@@ Subscribe ok",19);
                if (slen > 7)
                {
                    if ((buffer[30]&0xf0) == MQTTPUBLISH)
                    {
                        for (si = 0; si < slen-34; si++)
                        {
                            buffer[si] = buffer[34+si];
                            //printf("%02X ", buffer[si]);
                        }
                        buffer[si] = '\0';

                        sPos = FindString(buffer,sizeof(buffer),topic,topiclen);//sStr.find(sStrfind);
                        if (sPos == 0)
                        {
                            //cLen = sizeof(topic);
                            //printf("\r\nsPos = %d, cLen = %d", sPos, cLen);
                            // string sStrRest(sStr, cLen, slen-34-cLen);
                            //cout << sStrRest << endl;
                            //cLen = strlen(sStrRest);
                            for (si = 0; si < (slen-34-topiclen); si++)
                            {
                                payload[si] = buffer[topiclen+si];
                                //printf("%02X ", command[si]);
                            }
                            payload[si] = '\0';
                            //printf("\r\nsi = %d, %s\r\n", si, payload);
                            USART1SendString("\r\n",2);
                            USART1SendString((unsigned char*)payload,si);
                            publish(topic,0,0,1,1, null,1);
                            return si;
                        }
                    }
                }
                return 0;
            }
        }
    }
    USART1SendString("\r\n@@@@ Subscribe err",20);
    return -1;
}


unsigned char unsubscribe( char* topic)
{
    unsigned short int length = 5;
    short int len ;
    memset(buffer, 0, MQTT_MAX_PACKET_SIZE);
    //printf("\r\n@@@@ Unsubscribe topic");
    if (_state == MQTT_CONNECTED)
    {
        if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic))
        {
            // Too long
            //printf("\r\n#### Data too long");
            return 0;
        }

        nextMsgId++;
        if (nextMsgId == 0)
        {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, buffer,length);
        len = write(MQTTUNSUBSCRIBE|MQTTQOS1,buffer,length-5);
        if (len > 0)
        {
            //printf("\r\nlen = %d,%x %x %x %x", len, (uint8_t)buffer[0], buffer[1], buffer[2], buffer[3]);
            if (((unsigned char)buffer[0] == MQTTUNSUBACK)&&((unsigned char)buffer[3] == (unsigned char)nextMsgId))
            {
                // printf("\r\n@@@@ Unsubscribe ok");
                return 1;
            }
        }
    }
    // printf("\r\n#### Unsubscribe error");
    return 0;
}


unsigned char asctou8(unsigned char *head)
{
    unsigned char k=0;

    if((*head)>='0'&&(*head)<='9')
    {
        k=((*head)-0x30)<<4;
    }
    else if((*head)>='a'&&(*head)<='f')
    {
        k=((*head)-0x57)<<4;
    }
    if((*(head+1))>='0'&&(*(head+1))<='9')
    {
        k|=(*(head+1)-0x30)&0x0f;
    }
    else if((*(head+1))>='a'&&(*(head+1))<='f')
    {
        k|=(*(head+1)-0x57)&0x0f;
    }
    return k;
}
