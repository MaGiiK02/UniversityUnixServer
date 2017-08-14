
# @Author: angelini.mattia
# @StudentCode: 502688


FROM sol2017_gcc_gdb_server

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp/Chatty

RUN gcc -Wall -g -o myapp \
      ./List/list_test.c\
      ./List/list.c \
      ./Utils/Utils.c \
      ./Booleans/Booleans.h

EXPOSE 5555
