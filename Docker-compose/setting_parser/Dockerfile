FROM gcc:4.9
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN gcc -Wall -o myapp ./SettingManager/SettingManager_Test.c ./SettingManager/SettingManager.c ./Utils/Utils.c ./Exception/Exception.h
CMD ["./myapp","./SettingManager/Tests/test.config"]