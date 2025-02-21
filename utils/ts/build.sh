#!/bin/bash
set -e

cwd_from_root=utils/ts
if [[ -f $cwd_from_root/tsconfig.json ]]; then
  cd $cwd_from_root
fi

cwd_from_src=..
if [[ -f $cwd_from_src/tsconfig.json ]]; then
  cd $cwd_from_src
fi

if [[ -f tsconfig.json ]]; then
  echo "Compile..."
  rm -f out/*.js
  npx tsc
  npx prettier src/*.ts --write
  for a in out/*.js; do
    echo "/* See source code at $PWD/src */$(cat $a)" > $a
  done
  chmod -w out/*.js
  mv -f out/*.js ../
else
  echo "No tsconfig.json at $PWD"
fi
