#!/bin/sh

echo -n "Enter your name: "
read name

if [ -z "$name" ]; then
    name="tmpuser"
fi

echo "Hello, $name!"
