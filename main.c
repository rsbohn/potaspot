/* SPDX-FileCopyrightText: Copyright (c) Randall Bohn 2024 */
/* SPDX-License-Identifier: gpl3 */
#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nsio.h"

const char *api = "N:HTTPS://api.pota.app/spot/activator";

char query[256];
char buffer[256];
void main(void) {
    unsigned char err;
    int n;
    putchar(CH_CLR);
    
}