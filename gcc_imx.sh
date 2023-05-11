



. /opt/fsl-imx-x11/4.1.15-2.0.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi demo_gcc.sh


echo gcc: $CC
echo

proc=imx_client
echo $CC -o $proc $proc.c -I../include -L../lib -lwebsockets



