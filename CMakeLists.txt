project("sol_2017")

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
        Chatty/Message/message.c
        Chatty/Operations/ops.h
        Chatty/Server/Statistic/stats.h
        Chatty/SettingManager/SettingManager.h
        Chatty/SettingManager/SettingManager.c
        Chatty/Sync/Sync.h
        Chatty/User/User.c
        Chatty/User/User.h
        Chatty/Utils/Utils.c
        Chatty/Utils/Utils.h
        Chatty/config.h
        Chatty/MemoryHelper/MemoryHelper.h
        )

find_package (Threads)

#add_executable(TEST-CHANNEL "${SOURCE_FILE}" Chatty/Channel/Channel_Test.c)

add_executable(TEST-HASH-TABLES  "${SOURCE_FILE}" Chatty/HashTable/HashTable_Test.c)
target_link_libraries (TEST-HASH-TABLES ${CMAKE_THREAD_LIBS_INIT})