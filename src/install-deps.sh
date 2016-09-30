export CXXFLAGS="-w -Wno-error -Wno-unknown-pragmas ${CXXFLAGS}";
export CFLAGS="-w -Wno-error -Wno-error=format-security ${CFLAGS}";
git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
cmake .
make
sudo make install
