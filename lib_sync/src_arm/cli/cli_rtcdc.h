#include <rtcdc.h>
#include "../sy_rtcdc.h"

void upload_client_on_channel(struct rtcdc_peer_connection *peer,
        struct rtcdc_data_channel *dc, 
        void *user_data);


void upload_client_on_connect(struct rtcdc_peer_connection *peer,
        void *user_data);



void download_client_on_channel(struct rtcdc_peer_connection *peer,
        struct rtcdc_data_channel *dc, 
        void *user_data);


void download_client_on_connect(struct rtcdc_peer_connection *peer,
        void *user_data);

