#!/bin/sh
set -eu

failed=false;

base="$(pwd)"
for d in $(find examples -maxdepth 1 -mindepth 1 -type d); do
    echo "::group::$(basename $d)"
    cd "$d"
    if ! platformio run -e normal; then
        echo "::error ::Failed to build $d"
        failed=true;
    fi
    cd "$base"
    echo "::endgroup::"
done

if $failed; then
    exit 1
fi
