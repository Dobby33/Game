#!/bin/bash
set -e

cwd_from_src=..
if [[ -f main.c ]]; then
  cd $cwd_from_src
fi

rm -f src/head.h
node utils/compose src head.h

mkdir -p out
echo
echo
gcc -o out/a.out src/main.c -lm -fsanitize=address -g
./out/a.out
echo
echo
gcc -o out/a.out src/main.c -lm -Wall -Wpedantic -Wextra -Werror

if [[ $1 == "-f" ]]; then
  if [[ -z $(ls src/.*.swp 2>/dev/null) ]]; then
    clang-format --style=chromium --verbose -i src/*
  else
    echo "Reject formatting"
  fi
else
  echo "Omit formatting"
fi

chmod -w src/head.h
