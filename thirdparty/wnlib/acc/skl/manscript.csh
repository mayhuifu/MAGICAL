#!/bin/csh -f

set nonomatch

foreach manpage (*.man)
  rm -f ../../doc/man/$manpage
  ln -s ../../acc/skl/$manpage ../../doc/man/
end
