#!/bin/bash

/bin/rm -rf tmp
mkdir -p tmp
touch tmp/run

make web 2> tmp/errors.log | tee tmp/output.log

echo "==============================ERRORS in stderr================================="
cat tmp/errors.log
echo "==============================WARNING in output.log================================="
cat tmp/output.log | grep arning
echo "==============================ERRORS================================="
cat tmp/output.log | grep rror


if ! [ -f "tmp/rsync.ok" ]; then
    echo "==============================================================="
    echo "==============================================================="
    echo "Did not rzync==> remove last version"
    find doc/download -name "*`cat doc/VERSION`*" -exec rm {} \;
    cp -f doc/VERSION.old doc/VERSION
fi

/bin/rm -f tmp/run tmp/rsync.ok


