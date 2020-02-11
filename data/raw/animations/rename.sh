#!/bin/sh

if [ $# -ne 3 ]; then
  echo "Missing arguments"
  echo "Usage"
  echo "\t$0 WORKING_DIR BASE_NAME NEW_BASE_NAME"
  exit 1
fi

WORKING_DIR=$1
BASE_NAME=$2
NEW_BASE_NAME=$3

for FILE in "$WORKING_DIR/$BASE_NAME"*; do
  NUMBER=$(echo $FILE | sed -E 's/.+([0-9][0-9]).+/\1/')
  mv $FILE "$WORKING_DIR/${NEW_BASE_NAME}_$NUMBER.png"
done
