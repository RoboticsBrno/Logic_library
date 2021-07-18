#!/bin/sh
set -eu

failed=false;
revision="$(git rev-parse HEAD)"

rm -rf .tmpbuild .tmppio

for d in $(find examples -maxdepth 1 -mindepth 1 -type d); do
    echo "::group::$(basename $d)"

    cp -a $d .tmpbuild
    rm -rf .tmpbuild/.pio
    cp -a .tmppio .tmpbuild/.pio || true

    cd .tmpbuild
    sed -i "s/Logic_library\.git/Logic_library\.git#$revision/g" platformio.ini
    cat platformio.ini

    if ! platformio run -e normal; then
        echo "::error ::Failed to build $d"
        failed=true;
    else
        rm -rf ../.tmppio
        mv .pio ../.tmppio
    fi
    cd ..
    rm -rf .tmpbuild
    echo "::endgroup::"
done

if $failed; then
    exit 1
fi
