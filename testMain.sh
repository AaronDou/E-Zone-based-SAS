clear

gcc -o scheme main.c common.c commit.c IUs.c utility.c -lpbc -lgmp -L/usr/local/lib/ -I/usr/local/include/pbc -L/usr/lib -lssl -lcrypto -w

./scheme < param/a.param
