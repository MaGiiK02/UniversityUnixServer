FROM sol2017_gcc_gdb_server
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp/Chatty
RUN gcc -Wall -g -o myapp ./SettingManager/SettingManager_Test.c ./SettingManager/SettingManager.c ./Utils/Utils.c ./Exception/Exception.h
CMD ["gdb","--args","./myapp","./SettingManager/Tests/test.config"]
