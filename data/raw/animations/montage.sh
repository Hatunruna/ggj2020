#!/bin/sh

if [ $# -ne 4 ]; then
  echo "Missing arguments"
  echo "Usage"
  echo -e "\t$0 IMAGES_FOLDER IMAGE_SIZE TILESET_SIZE TILESET_NAME"
  exit 1
fi

montage $1/* -background none -tile $3 -geometry $2+0 png32:$4
