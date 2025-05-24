#!/bin/sh

sum() {
    total=0
    for arg in "$@"; do
        if echo "$arg" | grep -Eq '^[-]?[0-9]+$'; then
            total=$(expr "$total" + "$arg" 2>/dev/null)
            if [ $? -ne 0 ]; then
                echo 0
                return
            fi
        else
            echo 0
            return
        fi
    done
    echo "$total"
}

read -r line1
read -r line2

sum1=$(sum $line1)
sum2=$(sum $line2)

if [ "$sum1" -eq "$sum2" ]; then
    echo "Equal"
else
    echo "Not equal"
fi
