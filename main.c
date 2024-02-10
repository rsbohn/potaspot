/* SPDX-FileCopyrightText: Copyright (c) Randall Bohn 2024 */
/* SPDX-License-Identifier: gpl3 */
#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nsio.h"
#include "sio.h"

const char *api = "N:HTTPS://api.pota.app/spot/activator";

char buffer[256];

unsigned char query(char *path) {
    unsigned char err;
    int n;
    err = njsonquery(1, SIO_RW, path);
    buffer[0] = '\0';
    n = (OS.dvstat[1]<<8)+OS.dvstat[0];
    err = nread(1, buffer, 32);
    if (err == 1) {
        puts(buffer);
    } else {
        puts("ERR");
    }
    return err;
}
void main(void) {
    unsigned char err;

    /* putchar(CH_CLR); */
    puts("POTA Parks On The Air");
    err = nopen(1, (char *) api, SIO_RW);
    err = nchanmode(1, SIO_RW, CHANNELMODE_JSON);
    err = njsonparse(1, SIO_RW);

    /* TODO: Format the display. */    
    query("/0/spotId");
    query("/0/spotTime");
    query("/0/activator");

}