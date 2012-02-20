//
//  test.c
//  VitaMTP
//
//  Created by Yifan Lu on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vitamtp.h>

unsigned char data[] = {
    0xCC, 0x00, 0x00, 0x00, 0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E, 0x30, 0x22, 0x20, 0x65, 0x6E, 0x63, 0x6F, 0x64, 0x69, 0x6E, 0x67, 0x3D, 0x22, 0x55, 0x54, 0x46, 0x2D, 0x38, 0x22, 0x3F, 0x3E, 0x0A, 0x3C, 0x69, 0x6E, 0x69, 0x74, 0x69, 0x61, 0x74, 0x6F, 0x72, 0x49, 0x6E, 0x66, 0x6F, 0x20, 0x70, 0x6C, 0x61, 0x74, 0x66, 0x6F, 0x72, 0x6D, 0x54, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x50, 0x43, 0x22, 0x20, 0x70, 0x6C, 0x61, 0x74, 0x66, 0x6F, 0x72, 0x6D, 0x53, 0x75, 0x62, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x4D, 0x61, 0x63, 0x22, 0x20, 0x6F, 0x73, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x30, 0x2E, 0x37, 0x22, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E, 0x31, 0x30, 0x2E, 0x34, 0x30, 0x38, 0x36, 0x2E, 0x37, 0x38, 0x22, 0x20, 0x70, 0x72, 0x6F, 0x74, 0x6F, 0x63, 0x6F, 0x6C, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x30, 0x31, 0x32, 0x30, 0x30, 0x30, 0x31, 0x30, 0x22, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x79, 0x69, 0x66, 0x61, 0x6E, 0x73, 0x2D, 0x69, 0x6D, 0x61, 0x63, 0x22, 0x20, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x54, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x35, 0x22, 0x20, 0x2F, 0x3E, 0x0A, 0x00
};

void *vita_event_listener(LIBMTP_mtpdevice_t *device);

int event_listen;

void *vita_event_listener(LIBMTP_mtpdevice_t *device)
{
    LIBMTP_event_t event;
    int event_id;
    settings_info_t info;
    while(event_listen)
    {
        if(LIBMTP_Read_Event(device, &event) < 0)
        {
            fprintf(stderr, "Error recieving event.\n");
        }
        event_id = event.Param1;
        fprintf(stderr, "Event: 0x%x id %d\n", event.Code, event_id);
        switch(event.Code)
        {
            case PTP_EC_VITA_RequestGetSettingInfo:
                VitaMTP_GetSettingInfo(device, event_id, &info);
                break;
        }
    }
    
    return NULL;
}

int main(int argc, char** argv)
{
    LIBMTP_error_number_t err;
    
    LIBMTP_Set_Debug(9);
    
    LIBMTP_Init();
    
    LIBMTP_mtpdevice_t *device;
    pthread_t event_thread;
    PTPParams *params;
    
    device = LIBVitaMTP_Get_First_Vita();
    if (device == NULL)
    {
        fprintf(stderr, "Unable to open Vita\n");
        return 1;
    }
    
    params = (PTPParams*)device->params;
    
    event_listen = 1;
    if(pthread_create(&event_thread, NULL, (void*(*)(void*))vita_event_listener, device) != 0)
    {
        fprintf(stderr, "Cannot create event listener thread.\n");
        return 1;
    }
    
    vita_info_t info;
    initiator_info_t info2;
    info2.raw_len = 0xD0;
    info2.raw_xml = data;
    VitaMTP_GetVitaInfo(device, &info);
    VitaMTP_SendInitiatorInfo(device, &info2);
    VitaMTP_SendHostStatus(device, 0);
    
    sleep(30);
    
    event_listen = 0;
    
    LIBMTP_Release_Device(device);
    
    printf("OK.\n");
    
    return 0;
}