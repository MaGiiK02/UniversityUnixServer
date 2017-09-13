

FROM sol2017_gcc_gdb_server

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp/Chatty

RUN gcc -std=c99 -Wall -pedantic -g -DMAKE_VALGRIND_HAPPY -o server \
      ./Connections/test_server.c \
      ./Connections/connections.c \
      ./Utils/Utils.c \
      ./Booleans/Booleans.h \
      ./Error/Error.h \
      ./Message/message.h

EXPOSE 5555
