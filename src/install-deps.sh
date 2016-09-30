

export CXXFLAGS="-Wno-error -Wno-unknown-pragmas ${CXXFLAGS}";
export CFLAGS="-Wno-error -Wno-error=format-security ${CFLAGS}";

git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
cmake .
make CFLAGS = "-Wno-error -Wno-error=format-security"
sudo make install
