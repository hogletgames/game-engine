#!/usr/bin/env bash

CLANG_FORMAT_BIN=clang-format
EXIT_CODE=0
FILES=$(ls  examples/*.cpp \
            include/ge/*.h \
            src/ge/*.cpp   \
            tests/*.cpp    \
        )

while [ ! -z $1 ]; do
    case $1 in
    -f|--fix)
        FIX=true
        ;;
    --clang-format-bin)
        shift
        CLANG_FORMAT_BIN=$1
        ;;
    *)
        echo "Not supported arg: $1"
        exit 1
    esac
    shift
done

function clang_format_fix() {
    for FILE in $FILES; do
        echo "clang-format fix: $FILE"
        $CLANG_FORMAT_BIN --style=file -i $FILE
    done
}

function clang_format_check() {
    for FILE in $FILES; do
        echo "clang-format check: $FILE"

        $CLANG_FORMAT_BIN --style=file --output-replacements-xml $FILE | \
        grep "<replacement " > /dev/null

        if [ $? -eq "0" ]; then
            echo "replacement: $FILE"
            EXIT_CODE=1
        fi
    done
}

if [ $FIX ]; then
    clang_format_fix
else
    clang_format_check
fi

exit $EXIT_CODE
