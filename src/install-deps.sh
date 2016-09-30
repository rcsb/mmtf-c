

export CXXFLAGS="-Wno-unknown-warning-option -Wno-unused-local-typedefs -Wno-unknown-pragmas ${CXXFLAGS}";
export CFLAGS="-Wno-error -Wno-error=format-security ${CFLAGS}";

git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
cmake .
make CFLAGS = "-Wno-error -Wno-error=format-security"
sudo make install
