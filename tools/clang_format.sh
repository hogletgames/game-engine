#!/usr/bin/env bash

CLANG_FORMAT_BIN=clang-format
EXIT_CODE=0
FILES=$(ls  examples/*.cpp                              \
            include/ge/*.h                              \
            include/ge/core/*.h                         \
            include/ge/imgui/*.h                        \
            include/ge/renderer/*.h                     \
            include/ge/window/*.h                       \
            src/ge/*.cpp                                \
            src/ge/core/*.cpp                           \
            src/ge/imgui/*.cpp                          \
            src/ge/imgui/unix/*.h                       \
            src/ge/imgui/unix/*.cpp                     \
            src/ge/renderer/*.cpp                       \
            src/ge/renderer/opengl/*.h                  \
            src/ge/renderer/opengl/*.cpp                \
            src/ge/renderer/unix/*.h                    \
            src/ge/renderer/unix/*.cpp                  \
            src/ge/utils/*.h                            \
            src/ge/window/*.cpp                         \
            src/ge/window/unix/*.h                      \
            src/ge/window/unix/*.cpp                    \
            tests/*.cpp                                 \
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
