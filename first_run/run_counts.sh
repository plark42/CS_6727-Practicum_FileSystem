n=0
num=`ls test | wc -l`
for i in `ls test`; do
  let "n++"
  echo running $n of $num >&2
  f="test/$i"
  python3 count_block_predictions.py $f
done
