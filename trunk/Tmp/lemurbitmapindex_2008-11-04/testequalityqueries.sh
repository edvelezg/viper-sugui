echo "This script makes sure that the bitmap index library is relatively bug-free."
PREFIX=tmpdeleteme
echo " I am going to check against the projection index"
let ok=0
let notok=0
for type in uncompressed 8 16 32 Kaser32 Kaser64 ; do
for file in ./synthetic/unidep3d.csv ./synthetic/zipf4d.csv ; do
  rm -f $PREFIX*
  echo "testing with file $file with projection type  $type"
  ./indexcsv -k 1 $file $PREFIX-reference.bin >/dev/null
      rm -f $PREFIX-current.bin*
      ./indexcsv -p $type $file $PREFIX-current.bin >/dev/null
      for dim  in 0 1 2 ; do 
        for value in  0 1 2 3 4 5 6 7 8 9 10 98 99 ; do #`seq 0 10`; do
          ./querybitmap $PREFIX-reference.bin -j $dim $value > $PREFIX-reference.txt
          ./queryprojection $PREFIX-current.bin -j $dim $value > $PREFIX-current.txt
          if diff $PREFIX-reference.txt $PREFIX-current.txt >/dev/null ; then
            let ok+=1
          else
            let notok+=1
            echo "bug found!"
            echo ./querybitmap $PREFIX-reference.bin  $dim $value 
            echo ./queryprojection $PREFIX-current.bin  $dim $value 
            exit 1
          fi
        done
  done
done
done
echo $ok successes and $notok failures

echo "Next, I am going to check against the interval bitmap index"
let ok=0
let notok=0
for rangeencoding in range sqrt ; do
echo "range encoding type: $rangeencoding"
for file in ./synthetic/unidep3d.csv ./synthetic/zipf4d.csv ; do
  rm -f $PREFIX*
  echo "testing with file $file"
  ./indexcsv -k 1 $file $PREFIX-reference.bin >/dev/null
      rm -f $PREFIX-current.bin*
      ./indexcsv -i $rangeencoding $file $PREFIX-current.bin >/dev/null
      for dim  in 0 1 2 ; do 
        for value in  0 1 2 3 4 5 6 7 8 9 10 50 80 84 85 98 99 ; do #`seq 0 10`; do
          ./querybitmap $PREFIX-reference.bin -j $dim $value > $PREFIX-reference.txt
          ./queryintervalbitmap $PREFIX-current.bin  -i $rangeencoding -j r$dim $value $value > $PREFIX-current.txt
          if diff $PREFIX-reference.txt $PREFIX-current.txt >/dev/null ; then
            let ok+=1
          else
            let notok+=1
            echo "bug found! $value"
            echo ./queryintervalbitmap $PREFIX-current.bin -i $rangeencoding  -j r$dim $value $value
            echo ./querybitmap $PREFIX-reference.bin -j $dim $value
            exit 1
          fi
        done
  done
done
done
echo $ok successes and $notok failures
let ok=0
let notok=0
echo "This testing of the equality queries can take a few minutes."
for file in ./synthetic/unidep3d.csv ./synthetic/zipf4d.csv ; do
  rm -f $PREFIX*
  echo "testing with file $file"
  ./indexcsv -k 1 $file $PREFIX-reference.bin >/dev/null
  for bits in 16 32 64 ; do
    for k in 1 2 3 4 ; do 
      rm -f $PREFIX-current.bin*
      ./indexcsv -k $k -B $bits $file $PREFIX-current.bin >/dev/null
      for dim  in 0 1 2 ; do 
        for value in 0 1 2 3 4 5 6 7 8 9 10 98 99 ; do #`seq 0 10`; do
          ./querybitmap $PREFIX-reference.bin -j $dim $value > $PREFIX-reference.txt
          ./querybitmap $PREFIX-current.bin -j $dim $value > $PREFIX-current.txt
          if diff $PREFIX-reference.txt $PREFIX-current.txt >/dev/null ; then
            let ok+=1
          else
            let notok+=1
            echo diff $PREFIX-reference.txt $PREFIX-current.txt
            echo "bug found!"
            exit 1
            exit
          fi
        done
      done 
    done
  done
done
echo $ok successes and $notok failures
