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
#define BSIZE (256)
const char *api = "N:HTTPS://api.pota.app/spot/activator";
char buffer[BSIZE];
char qbuffer[BSIZE];

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
unsigned char query(char *pathfmt, int i) {
    unsigned char err;
    int n;
    int len;
    len = sprintf(qbuffer, pathfmt, i, BSIZE);
    err = njsonquery(1, SIO_RW, qbuffer);
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

void loop() {
    unsigned char err;
    int n;

    putchar(CH_CLR);
    puts("POTA Parks On The Air");
    err = nopen(1, (char *) api, SIO_RW);
    err = nchanmode(1, SIO_RW, CHANNELMODE_JSON);
    err = njsonparse(1, SIO_RW);

    for (n = 0; n < 3; n++) {
        query("/%d/spotId", n);
        query("/%d/spotTime", n);
        putchar('\n');
        query("/%d/activator", n);
        query("/%d/mode", n);
        query("/%d/grid4", n);
        putchar('\n');
        query("/%d/name", n);
        putchar('\n');
        putchar('\n');
    }
    
    err = nchanmode(1, SIO_RW, CHANNELMODE_PROTOCOL);
    err = nclose(1);
}

int main(void) {
    for (;;) {
        loop();
        sleep(30);
    }
    exit(0);
}
