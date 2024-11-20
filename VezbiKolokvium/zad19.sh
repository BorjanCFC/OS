# Да се напише командна процедура што ќе ги прикаже на екран броевите од 0 до 20. 

#!/bin/bash

broj=0

while [ $broj -le 20 ]
do
    echo "$broj"
    broj=`expr $broj + 1`
done
