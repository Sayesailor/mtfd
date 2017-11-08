// Copyright 2017-2018 SeetaTech

#include <eupulogger4system.h>
#include "include/config.h"
#include "include/domtfd.h"

#ifndef SHARED_LIB
int main(int argc, char *argv[]) {
    show_usage(argc, argv);
    domtfd();
    exit(EXIT_SUCCESS);
}
#endif
