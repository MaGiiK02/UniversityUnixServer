
project("sol_2017")
cmake_minimum_required(VERSION 3.8)
add_definitions(-DDEBUG)
set(CMAKE_C_FLAGS "-std=c99 -g -Wall -pedantic")

set(SOURCE_FILE
        Chatty/Booleans/Booleans.h
        Chatty/Channel/Channel.h
        Chatty/Channel/Channel.c
        Chatty/Connections/connections.h
        Chatty/Connections/connections.c
        Chatty/Error/Error.h
        Chatty/HashTable/HashTable.h
        Chatty/HashTable/HashTable.c
        Chatty/List/list.h
        Chatty/List/list.c
        Chatty/Message/message.h
        Chatty/Operations/ops.h
        Chatty/Server/Statistic/stats.h
        Chatty/SettingManager/SettingManager.h
        Chatty/SettingManager/SettingManager.c
        Chatty/Sync/Sync.h
        Chatty/Server/User/User.c
        Chatty/Server/User/User.h
        Chatty/Utils/Utils.c
        Chatty/Utils/Utils.h
        Chatty/config.h
        Chatty/MemoryHelper/MemoryHelper.h
        Chatty/Debugger/Debugger.h Chatty/HashTable/HashTableSynchronized.c Chatty/HashTable/HashTableSynchronized.h Chatty/List/ListSynchronized.c Chatty/List/ListSynchronized.h Chatty/Server/Worker/Worker.c Chatty/Server/Worker/Worker.h Chatty/Server/ServerGlobalData.h Chatty/Server/SocketSync/SocketSync.c Chatty/Server/SocketSync/SocketSync.h Chatty/Server/ServerGlobalData.c Chatty/Server/ServerOperations/ServerOperations.c Chatty/Server/ServerOperations/ServerOperations.h Chatty/Server/Statistic/stats.c
        )

#add_executable(TEST-CHANNEL "${SOURCE_FILE}" Chatty/Channel/Channel_Test.c)
add_executable(TEST-HASH-TABLES  "${SOURCE_FILE}" Chatty/HashTable/HashTable_Test.c)
add_executable(TEST-LIST  "${SOURCE_FILE}" Chatty/List/list_test.c)
add_executable(SERVER  "${SOURCE_FILE}" Chatty/Server/chatty.c Chatty/Server/ServerMessages/ServerMessages.c Chatty/Server/ServerMessages/ServerMessages.h Chatty/Server/Group/Group.c Chatty/Server/Group/Group.h Chatty/Server/FileSync/FileSync.h Chatty/Server/FileSync/FileSync.c)
add_executable(SETTING-MANAGER  "${SOURCE_FILE}" Chatty/SettingManager/SettingManager_Test.c)
add_executable(Client "${SOURCE_FILE}" Chatty/ChattyClient/client.c)
find_package (Threads)
target_link_libraries(TEST-HASH-TABLES ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(TEST-LIST ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(SERVER ${CMAKE_THREAD_LIBS_INIT})