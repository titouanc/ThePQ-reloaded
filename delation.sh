TITOUAN=0
ANTOINE=0
FLORENTIN=0
JEROME=0
ANTHONY=0
MIRCEA=0

for file in `find . -name *.[c,h]pp`; do
	TITOUAN=$((TITOUAN + `git blame $file | grep -c "Titouan Christophe"`))
	ANTOINE=$((ANTOINE + `git blame $file | grep -c "Antoine Carpentier"`))
	FLORENTIN=$((FLORENTIN + `git blame $file | grep -c "Florentin Hennecker"`))
	JEROME=$((JEROME + `git blame $file | grep -c "Jerome Hellinckx"`))
	ANTHONY=$((ANTHONY + `git blame $file | grep -c "Anthony Caccia"`))
	MIRCEA=$((MIRCEA + `git blame $file | grep -c "Mircea Mudura"`))
done

echo "Titouan : $TITOUAN"
echo "Antoine : $ANTOINE"
echo "Florentin : $FLORENTIN"
echo "Jerome : $JEROME"
echo "Anthony : $ANTHONY"
echo "Mircea : $MIRCEA"