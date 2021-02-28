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
    // Serial initializing
    
    /* Timer start */
    timer.start();
    
    float serial_last_sent = 0;

    /* Lidar variables */
    int cur_alt;
    
    /* GPS */
    char temp1[9];
    char temp2[10];
    float current_lat;
    float current_lng;
    
    /* Main Loop start */
    while (true) {
        // debugging
        if(timer.read() - serial_last_sent > 0.05){
            //pc.printf("%.6f\t%.6f\t%d\n", current_lat, current_lng, cur_alt);
            serial_last_sent = timer.read();
        }
        
        // Lidar signal processing
        if(LidarSerial.readable()){
            cur_alt = (int)(LidarSerial.getc());
            pc.printf("%.6f\t%.6f\t%d\n", current_lat, current_lng, cur_alt);
        }
        
        /* GPS UART processing */
        if(GpsSerial.readable()){
            if(GpsSerial.getc() == '!'){
                for(int i=0;i<9;i++){
                    temp1[i] = (int)(GpsSerial.getc());
                }
                if(GpsSerial.getc() != '*') { continue; }
                for(int i=0;i<10;i++){
                    temp2[i] = (int)(GpsSerial.getc());
                }
                if(GpsSerial.getc() != '#') { continue; }
                current_lat = std::atof(temp1);
                current_lng = std::atof(temp2);
            }
        }
    }
}
