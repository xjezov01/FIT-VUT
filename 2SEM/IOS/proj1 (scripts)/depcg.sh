#!/usr/bin/env bash
# Autor: Filip Ježovica, xjezov01@stud.fit.vutbr.cz
# skript depcg.sh projekt1_uloha2
#filee="${@: -1}"


#funkcia na formatovanie CALLER -> CALLEE   grep -E '(<.*>:)|callq'
function callformat()
{
objdump -d -j .text "$filee" | sed -n "/<$1>:/,/^$/p" | grep 'call.*<.*>' | sed 's/.*<//g' | sed 's/>.*//g' | sed "s/^/$1 -> /"
}

# funkcia ktora vytiahne kto vola zadanu funkciu.... s parametrom -r
function All ()
{
pole=(`objdump -d -j .text "$filee" | grep "<.*>:" | sed 's/.*<//g' | sed 's/>.*//g'`)
ii=${#pole[*]}

while [ "$ii" -gt 0 ]; do
((ii--))

callformat "${pole[$ii]}"

done
}

#funkcia fajront na PLT upravy..
function fajront()
{
	if [ "$Parg" != "true" ] ; then
		sed '/@plt/d' | sort -u   #sed -n '/pattern/!p' file
	else 
		sort -u
	fi | if [ "$Garg" = true ] ; then
		echo 'digraph CG {'		
		sed 's/@plt/_PLT/g' | sed 's/$/;/g'
		echo '}'
	     else cat -
	     fi
}

# Spracovanie argumentov pomocou GETOPTS
while getopts :pr:gd: o
do      case "$o" in
	p)      Parg=true ;; #ak je zapnute zahrnie i funkcie s @plt/_plt
	r)      OPTARGUMG=$OPTARG ; Rarg=true;;
        g)      Garg=true;; 
        d)      OPTARGUMD=$OPTARG ; Darg=true;;
        *)      echo "Chyba pri spustani. Pouzi parameter -r / -d s argumentom, + volitelne -g -p" >&2
                exit 1;;
        esac
done

((OPTIND--))

shift $OPTIND
filee=$*

if [[ $# -eq 0 ]]; then
	echo "Ziadne argumenty!" >&2
	exit 1
fi

if [ "$Darg" = true ] && [ "$Rarg" = true ]; then
	echo "Zle zadane argumenty" >&2
	exit 1   
fi

	if [ "$Darg" = true ]; then
		callformat "$OPTARGUMD"
	elif [ "$Rarg" = true ]; then
		All | grep ".* -> $OPTARGUMG$"
	else
		All
	fi | fajront | sed '/+/d'
