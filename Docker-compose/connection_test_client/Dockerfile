
# @Author: angelini.mattia
# @StudentCode: 502688


FROM sol2017_gcc_gdb_server

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp/Chatty

RUN gcc -Wall -g -o client \
      ./Connections/test_client.c \
      ./Connections/connections.c \
      ./Utils/Utils.c \
      ./Booleans/Booleans.h \
      ./Error/Error.h \
      ./Message/message.h

EXPOSE 5556

CMD ["gdb","client"]
