export CXXFLAGS="-w -Wno-error -Wno-unknown-pragmas ${CXXFLAGS}";
export CFLAGS="-w -Wno-error -Wno-error=format-security ${CFLAGS}";
git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr .
make
sudo make install
