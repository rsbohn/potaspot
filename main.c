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
    err = nread(1, buffer, n);
    if (err == 1) {
        puts(buffer);
    } else {
        puts("ERR");
    }
    return err;
}
void main(void) {
    unsigned char err;
    unsigned char c;
    int n;

    /* putchar(CH_CLR); */
    puts("POTA Parks On The Air");
    err = nopen(1, (char *) api, SIO_RW);
    err = nchanmode(1, SIO_RW, CHANNELMODE_JSON);
    err = njsonparse(1, SIO_RW);

    err = njsonquery(1, SIO_RW, "/0/activator");
    if (err == 1) {
        n = (OS.dvstat[1] << 8) + OS.dvstat[0];
        if (n == 0) {
            buffer[0] = '\0';
            err = nread(1, buffer, 32);
            if (err == 1) {
                puts(buffer);
            } else {
                puts("ERR: nread");
            }
        } else {
            puts("ERR: zero");
        }
    } else {
        puts("ERR: njsonquery");
    }
    puts("DONE!");
    OS.ch = KEY_NONE;
    do {
        c = OS.ch;
    } while (c == KEY_NONE);

    if (c == KEY_ESC) {
        abort();
    }
}