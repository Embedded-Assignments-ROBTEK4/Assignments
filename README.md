# TM4C123G
Template code for the Tiva TM4C123G ARM Cortex m4 board

#DEPENDENCIES

libusb
automake
build-essential

arm-none-eabi-gcc (on ubuntu, get it from https://launchpad.net/~terry.guo/+archive/ubuntu/gcc-arm-embedded)

clang if you want to build with that.

You first need to install lm4flash


`git clone https://github.com/utzig/lm4tools.git`


`cd lm4tools/lm4flash/`


`make`


`sudo cp lm4flash /usr/bin/`


You then need to install OpenOCD with ICDI support (for debugging)


`git clone git://git.code.sf.net/p/openocd/code`


`cd code`


`./bootstrap`


`./configure --prefix=/usr --enable-maintainer-mode --enable-stlink --enable-ti-icdi`


`make`


`sudo make install`

Install nmiver to get a graphical debugger

`sudo apt-get install nemiver`
