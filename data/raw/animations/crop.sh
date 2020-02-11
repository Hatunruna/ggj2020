#!/bin/sh

if [ $# -ne 3 ]; then
  echo "Missing parameters"
  echo "Usage:"
  echo -e "\t$0 WORKING_DIR TEXTURE_SIZE TEXTURE_POSITION"
  exit 1
fi

WORKING_DIR=$1
TEXTURE_SIZE=$2
TEXTURE_POSITION=$3

{
  cd $WORKING_DIR
  mkdir -p crops
  for IMAGE in *.png; do
    SUFFIX="${IMAGE##*_}"
    PREFIX="${IMAGE%_*}"

    if [ -f $IMAGE ]; then
      montage $IMAGE -background none -crop $TEXTURE_SIZE$TEXTURE_POSITION -geometry $TEXTURE_SIZE+0 png32:crops/${PREFIX}_crop_${SUFFIX}
    fi
  done

}
