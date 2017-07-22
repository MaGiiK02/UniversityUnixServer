#!/bin/bash
# Angelini Mattia 502688

usage="

The script provide a way to menage the files stored in the server,
showing or delete them reading the server configuration file, and using
it's 'DirName' value.

Script usage:

  ./ServerFileUtil.sh [-help|-h] conig_file -t <code>

#######################################################################
-help,-h  : print this usage message.
-t <code> : is the parameter that specify the type of the operation to
             perform with the dir specified in the DirName field of the
             provided file.


"

SETTING_NAME = "dirname"

dirname_setting_row_matcher = "[Dd][Ii][Rr][Nn][Aa][Mm][Ee]*( )=*( )[^=]*"

for arg in $* do
    case "$arg" in
      -h* ) echo "$usage"
            exit 0
            ;;
      -t )

      :) printf "missing argument for -%s\n" "$OPTARG" >&2
         echo "$usage" >&2
         exit 1
         ;;
      \?) printf "illegal option: -%s\n" "$OPTARG" >&2
         echo "$usage" >&2
         exit 1
         ;;
    esac
done


find = false
setting_row = ""

while read -r line && [$find = false]; do
    is_dir_name_setting $line $find
    if ( $find  ) then
      setting_row = $line;
    fi
done < "$filename"

if $find = false then
  (>&2 echo "error: The file don't contains a valid DirName field.")
  exit 1
fi

exit 0
