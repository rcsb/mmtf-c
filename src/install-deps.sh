
CFLAGS = "-Wno-error -Wno-error=format-security"

git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
cmake .
make
sudo make install
