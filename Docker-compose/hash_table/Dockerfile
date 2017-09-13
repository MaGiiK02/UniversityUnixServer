# @Author: angelini.mattia
# @StudentCode: 502688

FROM sol2017_gcc_gdb_server

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp/Chatty

RUN gcc -std=c99 -Wall -pedantic -g -DMAKE_VALGRIND_HAPPY -o myapp \
      ./HashTable/HashTable_Test.c\
      ./HashTable/HashTable.c \
      ./List/list.c \
      ./Utils/Utils.c \
      ./Booleans/Booleans.h

EXPOSE 5555
