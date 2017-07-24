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

  ./ServerFileUtil.sh [-h] [-b <base_path>] -t <0|1> config_file

#########################################################################
-h           : print this usage message.
-t <0|1>     : is the parameter that specify the type of the operation to
               perform with the dir specified in the DirName field of the
               provided file.
               Operations :
                  0.Print the path for all the server's media file in the
                    DirName path of the server's config.
                  1.Dellete all the file in DirName path of the server's
                    config file.
-b <dirpath> : the base path from where apply the setting in the file,
               if not provided, the script will assume the current path
               as base path.

"


SETTING_NAME="dirname"
# A Bash RegEx matching pattner for ( case insensitive ) : [not#]dirname[n-spaces]=[n-spaces][anyting that don't start with "="]
dirname_setting_row_matcher="^(([Dd][Ii][Rr][Nn][Aa][Mm][Ee])*( )=*( )[^=]*(.))"

RED='\033[0;31m'
NC='\033[0m' # No Color
#END_CONSTATS --------------------

# GLOBAL VARS --------------------

Operation=-1
Filename=""
BasePath="./"

# END GRLOBAL VARS ---------------


# Parsing the comand line
while getopts "h:t:b:" arg; do
    case $arg in
      h) echo "$usage"
         exit 0
         ;;

      t) Operation=$OPTARG
         ;;

      b) BasePath="$OPTARG"
         if [ ! -d $BasePath ]
         then
           printf "${RED}the provided base path is invalid!\n---->base_path_value=%s \n${NC}" "$BasePath">&2
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
    esac
done

Filename="${@: -1}"

if [ $Operation != 0 -a $Operation != 1 ]
then
    printf "${RED}invalid argument for -t!\n${NC}" >&2
    echo "$usage" >&2
    exit 1
fi

if [ -z $Filename ]
then
    printf "${RED}invalid file path\n${NC}" >&2
    echo "$usage" >&2
    exit 1
fi

printf "Start scanning :$Filename............"

# Find the dirname field
find=0
setting_row=""
while read -r line && [ $find -eq 0 ] ; do
    if [[ "$line" =~ $dirname_setting_row_matcher ]]
    then
       find=1
    fi

    if [ $find ]
    then
      setting_row=$line;
    fi
done < "$Filename"

echo "Ended!"

if [ $find -eq 0 ]
then
    printf "${RED}error: The file don't contains a valid DirName field.\n${NC}" >&2
    exit 1
fi

# value cutting
dirname_path=${setting_row#*=}
dirname_path=${dirname_path//[[:blank:]]/}
dirname_path=$BasePath$dirname_path

if [ ! -d "$dirname_path" ]
then
  printf "${RED}error: The DirName setting have an invalid floder: \"%s\"\n${NC}" "$dirname_path">&2
  echo "$usage" >&2
  exit 1
fi

if [ $Operation -eq 0 ]
then
  echo "Performing Operation -t=0(Listing) for directory:$dirname_path."
  ls -l "$dirname_path"
else
  echo "Performing Operation -t=1(Cleanig) for directory:$dirname_path."
  rm -r "$dirname_path\*"
fi

echo "Operation performed with success!"
exit 0
