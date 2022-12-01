<?php
declare(strict_types=1);

$a = 5;                     // 5
$b = $a;                    // $a
$c = $b + 5 - $a;           // $b 5 + $a -
$d = $c * ($b + $a) / 5;    // $c $b $a + * 5 /
$e = $d + $a;               // $d $a +
$f = 1 + ($e === null);     // 1 $e null === +

$i = "abc\n\tdef"           // "abc\n\tdef"         
$k = $i . " ghi";           // $i " ghi" .
$j = $k . $i . ",jkl\n";    // $f $i . ",jkl\n" . 
$g = $j . null              // $j null .

$h = null                   // null
$l = "hello" . $h           // "hello" $h .

$a = 7;                     // 7
$a = $a + null;             // $a null +

write($a);