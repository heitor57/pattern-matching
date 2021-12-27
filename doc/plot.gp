set dgrid3d 30,30
set hidden3d
splot "< awk '{if($1 == \"p\" && ($3<=5 || $3==15 || $3==30 || $3==50)) print}' ../data/data.txt" u 2:3:4 t "BMHS Paralelo" w p pt 2
