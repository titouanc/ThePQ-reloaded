#!/bin/bash
TITOUAN=0
ANTOINE=0
FLORENTIN=0
JEROME=0
ANTHONY=0
MIRCEA=0

echo -e "\033[31mDISCLAIMER : \033[0mCes chiffres sont purement informatifs."
echo "             Le nombre de lignes peut varier incroyablement en fonction de refactoring divers."
echo -e "\033[32mCommits\033[0m : `git log | grep "Author:" | wc -l`"
echo -e "\033[32mCommits par personne : \033[0m"
git shortlog -sn | cat

echo "Calcul du nombre de lignes par personne..."

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

echo -e "\033[32mNombre de lignes total\033[0m : $((TITOUAN+ANTOINE+FLORENTIN+JEROME+ANTHONY+MIRCEA))"
