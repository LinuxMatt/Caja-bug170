gcc -Wall -Wno-unused-but-set-variable -ggdb -O0 -DEGG_SM_CLIENT_BACKEND_XSMP -I. `pkg-config --cflags gtk+-2.0` *.c -o caja-bug170 `pkg-config --libs gtk+-2.0` -lX11 -lSM -lICE
