//
// Created by louis on 11/1/23.
//

#ifndef WOODY_WOODPACKER_WOODY_H
#define WOODY_WOODPACKER_WOODY_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BASE_ADDRESS 0x400000

#define ALIGN_MASK(x, mask)       (((x)+(mask))&~(mask))
//#define ALIGN(x, a)             ALIGN_MASK( x, (typeof(x))(a) -1 )
#define ALIGN_DOWN(x, a)          (x & ~((typeof(x))(a)-1))
#define ALIGN(x, a)               ALIGN_DOWN(x, a)

#include "structure.h"
#include "functions.h"

extern uint64_t base_address;

#endif //WOODY_WOODPACKER_WOODY_H
