for f in `ls test/test.*`; do
  x=`echo $f | awk -F/ '{print $2}'`
  echo writing $x
  ./Main --reformat
  ./Main --write $f $x > $x.csv
done

