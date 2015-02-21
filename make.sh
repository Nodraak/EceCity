
NAME=EceCity
SRC_DIR=src
INC_DIR=inc
SRC_FILES=$SRC_DIR/*
CFLAGS="-Wall -Wextra -pedantic -ansi -I $INC_DIR `pkg-config allegro --cflags`"
LIBS=`pkg-config allegro --libs`

CMD="gcc $CFLAGS $LIBS $SRC_FILES -o $NAME"

echo $CMD
$CMD
