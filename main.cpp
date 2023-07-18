#include "mbed.h"
#include "DFRobot_RGBLCD.h"
#include <chrono>
#include <cstdio>
#include "rtos.h"
#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include "HTS221Sensor.h"
#include <TLSSocket.h>
#include <TLSSocketWrapper.h>
#include <NetworkInterface.h>
#include <MbedJSONValue.h>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include "string.h"
#include <iostream>
#include "wifi.h"
#define JSON_NOEXCEPTION
#include "json.hpp"



static const char root_ca_cert[] = \
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
    "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
    "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
    "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
    "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
    "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
    "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
    "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
    "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
    "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
    "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
    "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
    "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
    "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
    "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
    "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
    "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
    "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
    "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
    "-----END CERTIFICATE-----";

InterruptIn button1(D13, PullUp);
InterruptIn button2(D12, PullUp);
InterruptIn button3(D10, PullUp);
PwmOut buzzer(D9);

Thread t2;
Thread t3;
DevI2C i2c(PB_11, PB_10);
HTS221Sensor sensor(&i2c);

int currentmenu= 0;
float current_sec = 0;
float current_min = 0;
float current_h = 0;
float currtime = 4;    
float alarmtime = 46800;
float alarmhours;
float alarmminutes;
bool alarming = 0;
float checktime= 0;

using json = nlohmann::json;

void if_button1(void){
    if (currentmenu == 0){
        if (alarming == 1){ 
        alarming = 0;
        buzzer = 0;
        alarmtime =0;
        }
        if (currentmenu == 0){
            alarmtime = -1;
        }
    }
    if (currentmenu == 1){
        alarmtime+=3600;
    }
}
void if_button2(void){
    if (currentmenu == 0){
        alarming = 0;
        buzzer = 0;
        alarmtime= currtime + 300;
    }
    if (currentmenu == 1){
          
        alarmtime+=60;
    }
}


void alarm_trigger(){
     
    alarmhours = floor(alarmtime/3600);
    alarmminutes = floor(fmod((alarmtime/60),60));
    
    
    while(1){
        
    if(alarmminutes == current_min && alarmhours == current_h){
            
            alarming = 1;
            printf("\n");
        }
        else{
            alarming = 0;
            
        }
}
}

void alarm(){
    while(1){
        while(alarming == 1){
            //buzzer
            
            buzzer.period(1.0/759.0);
            buzzer=0.5;
            ThisThread::sleep_for(1s);
            buzzer.period(1.0/394.0);
            buzzer=0.5;
            ThisThread::sleep_for(1s);
            buzzer.period(1.0/654.0);
            buzzer=0.5;
            ThisThread::sleep_for(1s);
            buzzer=0.0;
            buzzer.period(1.0/969.0);
            buzzer = 1/50.0f;
            ThisThread::sleep_for(1s);
            buzzer.period(1.0/800.0);
            ThisThread::sleep_for(1s);
        }  
        buzzer = 0;
    }
}
void nextmenu(){
    currentmenu += 1;
    if(currentmenu == 4){
        currentmenu = 0;
    }
}

void weather(NetworkInterface *network, char buffer[], std::string hostname, TCPSocket *socket){

    nsapi_size_or_error_t result;
 // Check if the connection is successful
    if (result != 0) {
        printf("Failed to connect to network: %d\n", result);
        while (1);
    }
 
    printf("Connection to network successful!\n");
    


    
    
    result = socket->open(network);
     if (result != 0) {
            printf("Error! _socket.open() returned: %d\r\n", result);
            result = socket->open(network);
     }

    
     //result = socket->set_root_ca_cert(root_ca_cert);
    // Create destination address
    SocketAddress address;
 
    // Get IP address of host by name
    result = network->gethostbyname(hostname.c_str(), &address);
    
    // Check result
    if (result != 0) {
        printf("Failed to get IP address of host: %d\n", result);
        while (1);
    }
    printf("IP address: %s\n", address.get_ip_address());
    // Set server port number
    address.set_port(80);
    printf("IP address: %i\n", address.get_port());
    // Connect to server at the given address
    
    result = socket->connect(address);
      // Check result
    if (result != 0) {
        printf("Failed to connect to server: %d\n", result);
        
    }

    printf("Connection to server successful!\n");
    // Create HTTP request
    const char request[] = "GET /current?access_key=d6e1572464c4a220ef460cfdf5ed04a6&query=Grimstad&units=m  HTTP/1.1\r\n"
                           "Host: api.weatherstack.com\r\n"
                           "Connection: close\r\n"
                           "\r\n";
 
    // Send request
    


    result = send_request_tcp(socket, request);
 
    

    // Check result
    if (result != 0) {
        printf("Failed to send request: %d\n", result);
        
    }
 
    
    // Read response
    result = read_response_tcp(socket, buffer,sizeof(&buffer));

     if (result != 0) {
        printf("Failed to read response: %d\n", result);
        while (1);
    }
    
}



void dateandtime(NetworkInterface *network, char buffer[], std::string hostname, TLSSocket *socket){
    nsapi_size_or_error_t result;
    if (result != 0) {
        printf("Failed to connect to network: %d\n", result);
        while (1);
    }
 
    printf("Connection to network successful!\n");
    

   
    
    
    result = socket->open(network);
     if (result != 0) {
            printf("Error! _socket.open() returned: %d\r\n", result);
            
     }

    
     result = socket->set_root_ca_cert(root_ca_cert);
        if (result != NSAPI_ERROR_OK) {
            printf("Error: _socket.set_root_ca_cert() returned %d\n", result);

        }
    SocketAddress address;
 
    result = network->gethostbyname(hostname.c_str(), &address);
    
    if (result != 0) {
        printf("Failed to get IP address of host: %d\n", result);
        while (1);
    }
    printf("IP address: %s\n", address.get_ip_address());
    address.set_port(443);
    printf("IP address: %i\n", address.get_port());
    
    result = socket->connect(address);
    if (result != 0) {
        printf("Failed to connect to server: %d\n", result);
        
    }

    printf("Connection to server successful!\n");
   
    const char request[] = "GET /timezone?apiKey=9331b59aad4d4c968abc2798b67aab6c HTTP/1.1\r\n"
                           "Host: api.ipgeolocation.io\r\n"
                           "Connection: close\r\n"
                           "\r\n";
 
    
    


    result = send_request(socket, request);
 
    

    if (result != 0) {
        printf("Failed to send request: %d\n", result);
        
    }
 
    
   
    result = read_response(socket, buffer,sizeof(&buffer));

     if (result != 0) {
        printf("Failed to read response: %d\n", result);
        
    }
    
}


int main()
{
    DFRobot_RGBLCD lcd(16,2,D14,D15); lcd.init(); lcd.setRGB( 0, 0, 255);
   
    sensor.init(nullptr);
    sensor.enable();
    button1.rise(&if_button1);
    button2.rise(&if_button2);
    
    button3.rise(&nextmenu);
    t2.start(alarm_trigger);
    t3.start(alarm);

   


    std::string t_24;
    std::string date_;
    std::string dayofweek;
    std::string dayof;

 while(true){

     NetworkInterface *network = NetworkInterface::get_default_instance();
 
    if (!network) {
        printf("Failed to get default network interface\n");
        while (1);
    }
    nsapi_size_or_error_t result = network->connect();
    static char buffer[4096];
    TLSSocket socket1 ;
    dateandtime(network, buffer,"api.ipgeolocation.io", &socket1);
    socket1.close();
      

    MbedJSONValue jsonobject;
    parse(jsonobject, strstr(buffer, "{"));
    
    dayofweek  = jsonobject["date_time_wti"].get<std::string>();
    date_ = jsonobject["date"].get<std::string>();
    t_24 = jsonobject["time_24"].get<std::string>();
    
    int j;
    j = strcspn(dayofweek.c_str(), ",");
    dayof = dayofweek.substr(0,j);
    int hours = atoi(t_24.c_str());
    strcpy(&t_24[0], &t_24[0+3]);
    int minutes = atoi(t_24.c_str());
    strcpy(&t_24[0], &t_24[0+6]);
    int seconds = atoi(t_24.c_str());
    printf("day: %s\r\n", dayof.c_str());
    printf("date: %s\r\n", date_.c_str());
   
   
    
    currtime = currtime + seconds + (minutes *60) + (hours *3600);
    checktime = currtime;
    printf("time is in seconds : %f\n", currtime);

   


// Siden Mikroen kræsja så mye så valgte vi heller å finne været uten å parse med en lib
   static char buffer2[4096];
   TCPSocket socket; 
   weather(network, buffer2,"api.weatherstack.com",&socket);
   socket.close();
   
   
   int j2= 0;
   int j5 = 0;
   char *json3= strstr(buffer2, "current");
   char*json2 = strstr(json3,"{");
   int j8;
    
    
   j2 = strcspn(json2,"}");
   
   strcpy(&json2[j2], &json2[j2+1]);
   
   char *json4 = strstr(json2,"weather_descriptions");
   
   char *json5 = strstr(json4,"[");
   printf("json5 %s :",json5);
    j5 = strcspn(json5,",");
    
    char d[50];
    memcpy(d,json5, j5);
    strcpy(&d[0], &d[2]);
    strcpy(&d[strlen(d)-2], &d[strlen(d)]);
    
    
while(1){
     
 

    if (currtime < 86400){
         currtime ++;
         current_h = floor(currtime/3600);
         current_sec = floor(fmod(currtime,60));
         current_min = floor(fmod((currtime/60),60));
         alarmhours = floor(alarmtime/3600);
         alarmminutes = floor(fmod((alarmtime/60),60)); 
         
         lcd.clear();
         if (currentmenu == 0){
             
         lcd.printf(" %02.0f:%02.0f:%02.0f", current_h,current_min,current_sec);
         lcd.setCursor(1, 2);
         lcd.printf("%s %s",dayof.c_str(),date_.c_str()); 

         }
         
         if (currentmenu == 1 ){
            lcd.printf("Set Alarm:");
             lcd.setCursor(1, 2);
         lcd.printf("%02.0f:%02.0f", alarmhours, alarmminutes);
         }
         
         

        if (currentmenu == 2){
            
                float temp, hum;
             if (sensor.get_humidity(&hum)!= 0) {
                    printf("error\n");
             }
            if (sensor.get_temperature(&temp)!= 0) {
                    printf("error\n");
            }
            
            lcd.printf("Humidity[%]:%02.0f",hum);
            lcd.setCursor(1, 2);
            lcd.printf("Tempeture[C]:%02.0f",temp);
            
        }
        if (currentmenu == 3){

            lcd.printf("Weather:");
            lcd.setCursor(1, 2);
            lcd.printf("%s", d);

        }   


          if (currtime == 86400){
             currtime = 0;
            
             break;
         }   

        ThisThread::sleep_for(702ms);

}
}
}
}