#!/usr/bin/env bash
# Autor: Filip Ježovica, xjezov01@stud.fit.vutbr.cz
# skript depsym.sh projekt1_uloha1
function remPATH()
{
	while read line 
		do
		
	    	OBJ1=$(basename "$(echo $line | sed 's/ ->.*//g')" )
	    	OBJ2=$(basename "$(echo $line | sed 's/.*-> //g' | sed 's/(.*//g')" )
	    	echo $line | sed "s/.*->/"$OBJ1" ->/g" | sed "s/->.*(/-> $OBJ2(/g"
	    done    
}

function All()
{
	premM=$(nm $filee | while read line
			do
				if [ -z "${line}" ]; then
					continue
				fi

				if [[ $line =~ .*: ]]; then
      				  justfile=$(echo $line | sed 's/://g')
 				
				elif [[ $line =~ .*U.* ]]; then  ## U
      				 printf "$justfile <-> $(echo $line | sed "s/.*U //g")"
      				 printf '\\n'
 				
 				else   ## nie U
      				 printf "$justfile >-< $(echo $line | cut -d ' ' -f3)"
      				 printf '\\n'
 				fi
			done)

echo -e $premM | while read line
					do
						if [[ -z "${line}"  ]]; then
							continue
						fi

						funkc=$(echo $line | sed 's/\//<<<>>>/g' | sed 's/.*>-< //g')
						file2=$(echo $line | sed 's/ >-<.*//g' | sed 's/\//<<<>>>/g')

						echo -e $premM | grep ".*<-> $funkc" | sed "s/ <->.*/ -> $file2 ($funkc)/g" | sed 's/<<<>>>/\//g'

					done | sort -u

}
# Spracovanie argumentov pomocou GETOPTS
while getopts :r:gd: o
do      case "$o" in
		r)      OPTARGUMR=$(basename "$OPTARG") ; Rarg=true;;
        g)      Garg=true;; 
        d)      OPTARGUMD=$(basename "$OPTARG") ; Darg=true;;
        *)      echo "Chyba pri spustani. Pouzi parameter -r / -d s argumentom, + volitelne -g" >&2
                exit 1;;
        esac
done

((OPTIND--))

shift $OPTIND

if [[ $# -eq 0 ]]; then
	echo "Ziadne argumenty!" >&2
	exit 1
fi

if [ "$Darg" = true ] && [ "$Rarg" = true ]; then
	echo "Zle zadane argumenty" >&2
	exit 1   
fi

filee=$*
PaTH=$(dirname $(echo $* | sed 's/ .*//g' | sed 's/\/.o//g'))
if [[ $PaTH = . ]]; then
	PaTH=""
else PaTH=$(echo $PaTH | sed 's/$/\//g')
fi

	PREMENNA=$(if [ "$Darg" = true ]; then
					All | grep "^$PaTH$OPTARGUMD -> .*"
				elif [ "$Rarg" = true ]; then
					All | grep ".* -> $PaTH$OPTARGUMR (.*"
				else
					All

				fi)

	if [ "$Garg" = true ] ; then
	
		LastTAB=$(echo "$PREMENNA" | sed 's/ -> /\n/g' | sed 's/ (.*//g' | sort -u | 
			while read line
				do
					if [ -z "${line}" ]; then
						continue
					fi

					printf $(basename $line) | sed 's/\./D/g' |  sed 's/+/P/g' | sed 's/-/_/g' | sed 's/_>/->/g'
					printf " [label=\"$line\"];\n"
				done)

				echo 'digraph GSYM {'		
				echo -e "$PREMENNA" | remPATH | sed 's/\./D/g' |  sed 's/+/P/g'| sed 's/(/[label=\"/g' | sed 's/)/\"];/g' | sed 's/-/_/g' | sed 's/_>/->/g'
				echo -e "$LastTAB"
				echo '}'
	    	else
	    		echo -e "$PREMENNA"
	fi | sed '/^$/d'