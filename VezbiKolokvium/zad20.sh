# Да се напише командна процедура што ќе одбројува одреден број секунди до почетокот на
# натпреварот. 

#!/bin/bash

sekundi=$1

while [ $sekundi -gt 0 ]
do
    echo "preostanato $sekundi sekundi"
    sekundi=`expr $sekundi - 1`
    sleep 1
done

echo "Pocnuva"
