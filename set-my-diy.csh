#!/bin/csh

set diy = $HOME/.diy
set name = lifegame

mkdir -p ${diy}/{bin,include,lib,man/{man1,man2},libdata/pkgconfig}

cp src/bin/${name} ${diy}/bin/

rehash
