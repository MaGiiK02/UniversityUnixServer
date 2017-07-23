#!/bin/bash
# Angelini Mattia 502688


#CONSTATS -----------------------------
usage="
${NC}
The script provide a way to manage the files stored in the server,
showing or delete them using the server configuration file in order
to find where the server store it's media files. 
(The DirName setting of the file) 

Script usage:

  ./ServerFileUtil.sh [-help|-h] config_file -t <0|1>

#######################################################################
-help,-h  : print this usage message.
-t <0|1> : is the parameter that specify the type of the operation to
            perform with the dir specified in the DirName field of the
            provided file. 
            Operations : 
               0.Print the path for all the server's media file in the 
                 DirName path of the server's config. 
               1.Dellete all the file in DirName path of the server's 
                 config file.

"


SETTING_NAME="dirname"
# A Bash RegEx matching pattner for ( case insensitive ) : dirname[n-spaces]=[n-spaces][anyting that don't start with "="]
dirname_setting_row_matcher="[Dd][Ii][Rr][Nn][Aa][Mm][Ee]*( )=*( )[^=]*"

RED='\033[0;31m'
NC='\033[0m' # No Color
#END_CONSTATS --------------------

# GLOBAL VARS --------------------

Operation=-1
Filename=""

# END GRLOBAL VARS ---------------


# Parsing the comand line
shift
for arg in $*; do
    case "$arg" in
      -h*) echo "$usage"
           exit 0
           ;;

      -t) shift
          if  arg $# -gt 0; then
              Operation=$2
          else
              printf "${RED}missing operation code for -t\n${NC}" >&2
              echo "$usage" >&2
              exit 1
          fi       
          ;;

      :) printf "${RED}missing argument for -%s\n${NC}" "$OPTARG" >&2
         echo "$usage" >&2
         exit 1
         ;;

      \?) printf "${RED}illegal option: -%s\n${NC}" "$OPTARG" >&2
          echo "$usage" >&2
          exit 1
          ;;

      *) Filename=$arg
         ;;
    esac
    shift
done

if [ $Operation != 0 -o $Operation != 1 ] 
then 
    printf "${RED}invalid argument for -t!${NC}" >&2
    echo "$usage" >&2
    exit 1
fi

if [ $Filename = "" ] 
then
    printf "${RED}invalid file path${NC}" >&2
    echo "$usage" >&2
    exit 1
fi  

# Find the dirname field
find=[ false ]
setting_row=""
while [ read -r line -a  $find = false ] 
do
    find = [ $line = $dirname_setting_row_matcher ]
    if [ $find ] 
    then
      setting_row=$line;
    fi
done < "$Filename"

if [ !$find ] 
then
    printf "${RED}error: The file don't contains a valid DirName field.${NC}" >&2
    exit 1
fi



exit 0
