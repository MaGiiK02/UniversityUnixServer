#!/bin/bash
# Angelini Mattia 502688


#CONSTANTS -----------------------------
usage="
${NC}
The script provide a way to manage the files stored in the server,
showing or delete them using the server configuration file in order
to find where the server store it's media files.
(The DirName setting of the file)

Script usage:

  ./ServerFileUtil.sh [-h] -t <int> config_file

#########################################################################
-h           : print this usage message.
-t <int>     : an integer that specify to the scrit h

"


SETTING_NAME="dirname"
# A Bash RegEx matching pattner for ( case insensitive ) : [not#]dirname[n-spaces]=[n-spaces][anyting that don't start with "="]
dirname_setting_row_matcher="^(([Dd][Ii][Rr][Nn][Aa][Mm][Ee])*( )=*( )[^=]*(.))"

RED='\033[0;31m'
NC='\033[0m' # No Color
#END_CONSTATS --------------------

# GLOBAL VARS --------------------

Time=-1
Filename=""

# END GRLOBAL VARS ---------------


# Parsing the comand line
while getopts "h:t:" arg; do
    case $arg in
      h) echo "$usage"
         exit 0
         ;;

      t) Time=$OPTARG
         ;;

      :) printf "${RED}missing argument for -%s\n${NC}" "$OPTARG"
         echo "$usage"
         exit 1
         ;;

      \?) printf "${RED}illegal option: -%s\n${NC}" "$OPTARG"
          echo "$usage"
          exit 1
          ;;
    esac
done

Filename="${@: -1}"

if [ $Time < 0 ]
then
    printf "${RED}%d is a invalid argument for -t\n${NC}"
    echo "$usage"
    exit 1
fi

if [ -z $Filename ]
then
    printf "${RED}invalid file path\n${NC}"
    echo "$usage"
    exit 1
fi

printf "Start scanning :$Filename............"

# Find the dirname field
find=0
setting_row=""
while read -r line && [ $find -eq 0 ] ; do
    if [[ "$line" == *""*$dirname_setting_row_matcher ]]
    then
       find=1
       setting_row=$line;
    fi
done < "$Filename"

echo "Ended!"

if [ $find -eq 0 ]
then
    printf "${RED}error: The file don't contains a valid DirName field.\n${NC}"
    exit 1
fi

# value cutting
dirname_path=${setting_row#*=}#Removing the part before the equal

if [ ! -d "$dirname_path" ]
then
  printf "${RED}error: The DirName setting have an invalid floder: \"%s\"\n${NC}" "$dirname_path"
  echo "$usage"
  exit 1
fi

if [ $Time -eq 0 ]
then
  echo "Performing listing operation for directory:$dirname_path."
  ls -l "$dirname_path"
else
  echo "Performing cleaning operation for files older than $Time minutes for directory:$dirname_path."
  find  $dirname_path -mmin +$Time -type f -delete
fi

echo "Operation performed with success!"
exit 0
