/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "platform/mbed_thread.h"
#include <string.h>
#include <stdlib.h>

Serial pc(USBTX, USBRX, 115200);
Serial LidarSerial(PA_9, PA_10, 115200);
Serial GpsSerial(PC_10, PC_11, 9600);

Timer timer;

int main(){
    /* Timer start */
    timer.start();

    /* Lidar variables */
    int cur_alt;
    
    /* GPS */
    char temp1[9];
    char temp2[10];
    float current_lat;
    float current_lng;
    int count = 0;
    char temp_read;
    
    /* Main Loop start */
    while (true) {
        // Lidar signal processing
        if(LidarSerial.readable()){
            cur_alt = (int)(LidarSerial.getc());
            pc.printf("!%.6f*%.6f*%04d#", current_lat, current_lng, cur_alt);
        }
        
        /* GPS UART processing */
        if(GpsSerial.readable()){
            if(GpsSerial.getc() == '!'){
                count = 0;
                while(1){
                    temp_read = GpsSerial.getc();
                    if(temp_read == '*') { break; }
                    temp1[count] = temp_read;
                    count+=1;
                }
                
                count = 0;
                while(1){
                    temp_read = GpsSerial.getc();
                    if(temp_read == '#') { break; }
                    temp2[count] = temp_read;
                    count+=1;
                }
                
                current_lat = std::atof(temp1);
                current_lng = std::atof(temp2);
            }
        }
    }
}
