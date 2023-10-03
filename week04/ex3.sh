gcc ex3.c -o ex3

./ex3 3 &

sleep 15

pstree -p

./ex3 5 &

sleep 30

pstree -p

rm ex3
