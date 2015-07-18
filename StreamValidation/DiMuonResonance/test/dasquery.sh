#!/bin/bash

# Simple script to check for new data in a given dataset

# Query DAS
mv das.out das_old.out
das_client.py --limit=0 --query="file dataset=/StreamExpressCosmics/Commissioning2015-TkAlCosmics0T-Express-v1/ALCARECO" > das.out
if [ $? -ne 0 ]; then
    echo "DAS query failed. Please try again."
	exit 1
fi
diff -u das_old.out das.out | diffstat

# Extract run range
awk 'BEGIN {FS="/"} NR==1 { print "First run: " $9 $10 } END { print "Last  run: " $9 $10; print "No. of records: " NR }' das.out

#echo "Last few lines of query output:"
#tail -5 das.out

