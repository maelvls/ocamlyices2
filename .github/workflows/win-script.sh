#! /bin/sh
set -e

eval $(opam config env)
CFLAGS="-I/usr/${HOST}/sys-root/mingw/include -L/usr/${HOST}/sys-root/mingw/lib/" \
    opam install -y ocamlfind zarith ounit

./configure --host=${HOST} --with-static-gmp=/usr/${HOST}/sys-root/mingw/lib/libgmp.a
make
# We must uninstall first in case any previous build installed but failed before
# uninstalling.
make uninstall
make install
make test

cd examples
make
./test.byte && ./test.opt
cd ..

make uninstall

make dist
DIST=ocamlyices2-$(git describe --tags)-${HOST}
mv dist $DIST
tar czf ${DIST}.tar.gz $DIST
echo "$(sha256sum ${DIST}.tar.gz)" >${DIST}.tar.gz.sha256
