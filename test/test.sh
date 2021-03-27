#!/bin/bash

rvc_exe=../build/rvc

if [ ! -x "$rvc_exe" ]; then
  echo "rvc is not executable"
  exit
fi

tests=(
  '1 = 1'
  '1+1 = 4-2'
  '2*3+4*6 = 30'
  '20/4 = 5'
  '40/4-5 = 5'
  '(((((1))))) = 2-----1'
  '1+2+3*4+5+6 = 52/(1+(1))'
  '2*3*4 = 4*3*2'
  '1+(2+(4+5)) = ((1+2)+4)+5'
  '1+2*3+4*5*-6 = -113'
  '1+2*(3+4)*5*-6 = -419'
)

for test_case in "${tests[@]}"; do
  lhs=$(echo $test_case | cut -d'=' -f1 | tr -d '[:space:]')
  rhs=$(echo $test_case | cut -d'=' -f2 | tr -d '[:space:]')

  echo "print($lhs);" > lhs.pgm
  $rvc_exe lhs.pgm && clang rv.bc 2>/dev/null && ./a.out > lhs.out

  echo "print($rhs);" > rhs.pgm
  $rvc_exe rhs.pgm && clang rv.bc 2>/dev/null && ./a.out > rhs.out

  diff lhs.out rhs.out > /dev/null
  if [ $? -ne 0 ]; then
    lhs_val=$(cat lhs.out)
    rhs_val=$(cat rhs.out)
    echo "Test failed: $lhs != $rhs (actual: $lhs_val != $rhs_val)"
  fi

  rm *.pgm *.bc *.out
done
