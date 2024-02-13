/* SPDX-FileCopyrightText: Copyright (c) Randall Bohn 2024 */
/* SPDX-License-Identifier: gpl3 */
#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "nsio.h"
#include "sio.h"

#define SCR_WIDTH (40)
const char *api = "N:HTTPS://api.pota.app/spot/activator";
char buffer[256];

void slowly() {
    clock_t now = clock();
    int x = 0;
    while (clock() < now + 10)
        /* burn some cycles*/
        x = (x + 1);
}

void text(char *s) {
    int x;
    char ch;
    for (x=0; x < SCR_WIDTH; x++) {
        ch = s[x];
        if (ch == 0x9B) break;
        putchar(ch);
        slowly();
    }
    putchar(' ');
}
unsigned char query(char *path) {
    unsigned char err;
    int n;

    err = njsonquery(1, SIO_RW, path);
    err = nstatus(1);
    buffer[0] = '\0';
    n = (OS.dvstat[1]<<8)+OS.dvstat[0];
    err = nread(1, buffer, n);

    if (err == 1) {
        text(buffer);
    } else {
        puts("ERR");
    }
    return err;
}
int main(void) {
    unsigned char err;

    putchar(CH_CLR);
    puts("POTA Parks On The Air");
    err = nopen(1, (char *) api, SIO_RW);
    err = nchanmode(1, SIO_RW, CHANNELMODE_JSON);
    err = njsonparse(1, SIO_RW);

    /* TODO: Format the display. */    
    query("/0/spotId");
    query("/0/activator");
    query("/0/spotTime");
    putchar('\n');
    query("/1/spotId");
    query("/1/activator");
    query("/1/spotTime");
    putchar('\n');

    err = nchanmode(1, SIO_RW, CHANNELMODE_PROTOCOL);
    err = nclose(1);

    puts("load http://localhost:9000/potaspot.xex");
    sleep(30);
    exit(0);
}
