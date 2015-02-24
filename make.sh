
NAME=EceCity
SRC_DIR=src
INC_DIR=inc
SRC_FILES=$SRC_DIR/*
CFLAGS="-Wall -Wextra -pedantic -ansi -O2 `pkg-config allegro --cflags`"
LIBS=`pkg-config allegro --libs`

CMD="gcc $CFLAGS -I $INC_DIR $LIBS $SRC_FILES -o $NAME"

echo $CMD
$CMD
