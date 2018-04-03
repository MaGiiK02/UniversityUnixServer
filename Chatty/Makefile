#
# chatterbox Progetto del corso di LSO 2017
#
# Dipartimento di Informatica Università di Pisa
# Docenti: Prencipe, Torquati
#
#

##########################################################
# IMPORTANTE: completare la lista dei file da consegnare
#
FILE_DA_CONSEGNARE = Makefile \
    config.h\
    Booleans/Booleans.h\
    Channel/Channel.c\
    Channel/Channel.h\
    Channel/Channel_Test.c\
    Client/client.c\
    Config/chatty.conf1\
    Config/chatty.conf2\
    Connections/connections.c\
    Connections/connections.h\
    Connections/test_client.c\
    Connections/test_server.c\
    Debugger/Debugger.h\
    Error/Error.h\
    HashTable/HashTable.c\
    HashTable/HashTable.h\
    HashTable/HashTable_Test.c\
    HashTable/HashTableSynchronized.c\
    HashTable/HashTableSynchronized.h\
    List/list.c\
    List/list.h\
    List/list_test.c\
    List/ListSynchronized.c\
    List/ListSynchronized.h\
    MemoryHelper/MemoryHelper.h\
    Message/message.c\
    Message/message.h\
    Operations/ops.h\
    Scripts/ServerFileUtil.sh\
    Server/chatty.c\
    Server/ServerGlobalData.c\
    Server/ServerGlobalData.h\
    Server/ServerOperations/ServerOperations.c\
    Server/ServerOperations/ServerOperations.h\
    Server/SocketSync/SocketSync.c\
    Server/SocketSync/SocketSync.h\
    Server/Statistic/stats.c\
    Server/Statistic/stats.h\
    Server/User/User.c\
    Server/User/User.h\
    Server/Worker/Worker.c\
    Server/Worker/Worker.h\
    Server/WorkerPool/WorkerPool.c\
    Server/WorkerPool/WorkerPool.h\
    Server/ServerMessages/ServerMessages.h\
    Server/ServerMessages/ServerMessages.c\
    SettingManager/SettingManager.c\
    SettingManager/SettingManager.h\
    SettingManager/SettingManager_Test.c\
    SettingManager/Tests/test.config\
    Sync/Sync.h\
    Tests/testconf.sh\
    Tests/testfile.sh\
    Tests/testleaks.sh\
    Tests/teststress.sh\
    Utils/Utils.c\
    Utils/Utils.h

# inserire il nome del tarball: es. NinoBixio
TARNAME=MattiaAngelini
# inserice il corso di appartenenza: CorsoA oppure CorsoB
CORSO=CorsoA
#
###########################################################

###################################################################
# NOTA: Il nome riportato in UNIX_PATH deve corrispondere al nome
#       usato per l'opzione UnixPath nel file di configurazione del
#       server (vedere i file nella directory DATA).
#       Lo stesso vale per il nome riportato in STAT_PATH e DIR_PATH
#       che deveno corrispondere con l'opzione StatFileName e
#       DirName, rispettivamente.
#
# ATTENZIONE: se il codice viene sviluppato sulle macchine del
#             laboratorio utilizzare come nomi, nomi unici,
#             ad esempo /tmp/chatty_sock_<numero-di-matricola> e
#             /tmp/chatty_stats_<numero-di-matricola>.
#
###################################################################
UNIX_PATH       = /tmp/chatty_socket
STAT_PATH       = /tmp/chatty_stats.txt
DIR_PATH        = /tmp/chatty

CC		=  gcc
AR              =  ar
CFLAGS	        += -std=c99 -Wall -pedantic -g -DMAKE_VALGRIND_HAPPY
ARFLAGS         =  rvs
INCLUDES	= -I.
LDFLAGS 	= -L.
OPTFLAGS	= #-O3
LIBS            = -pthread -lm

# aggiungere qui altri targets se necessario
TARGETS		= chatty        \
		  client


# aggiungere qui i file oggetto da compilare
OBJECTS		= Server/SocketSync/SocketSync.o\
        Channel/Channel.o\
        Connections/connections.o\
        HashTable/HashTable.o\
        HashTable/HashTableSynchronized.o\
        List/list.o\
        List/ListSynchronized.o\
        Server/ServerGlobalData.o\
        Server/ServerMessages/ServerMessages.o\
        Server/ServerOperations/ServerOperations.o\
        Server/User/User.o\
        Server/Statistic/stats.o\
        Server/Worker/Worker.o\
        SettingManager/SettingManager.o\
        Utils/Utils.o


# aggiungere qui gli altri include
INCLUDE_FILES   = config.h\
      Booleans/Booleans.h\
      Server/SocketSync/SocketSync.h\
      Channel/Channel.h\
      Connections/connections.h\
      Debugger/Debugger.h\
      Error/Error.h\
      HashTable/HashTable.h\
      HashTable/HashTableSynchronized.h\
      List/list.h\
      List/ListSynchronized.h\
      MemoryHelper/MemoryHelper.h\
      Message/message.h\
      Operations/ops.h\
      Server/ServerGlobalData.h\
      Server/ServerOperations/ServerOperations.h\
      Server/ServerMessages/ServerMessages.h\
      Server/Statistic/stats.h\
      Server/User/User.h\
      Server/Worker/Worker.h\
      SettingManager/SettingManager.h\
      Sync/Sync.h\
      Utils/Utils.h



.PHONY: all clean cleanall test1 test2 test3 test4 test5 consegna
.SUFFIXES: .c .h

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $<

all		: $(TARGETS)


chatty: ./Server/chatty.c libchatty.a $(INCLUDE_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

client: ./Client/client.c Connections/connections.o Message/message.h
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

############################ non modificare da qui in poi

libchatty.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean		:
	rm -f $(TARGETS)

cleanall	: clean
	\rm -f *.o *~ libchatty.a valgrind_out $(STAT_PATH) $(UNIX_PATH)
	\rm -fr  $(DIR_PATH)

killchatty:
	killall -9 chatty

# test base
test1:
	make cleanall
	\mkdir -p $(DIR_PATH)
	make all
	./chatty -f Config/chatty.conf1&
	./client -l $(UNIX_PATH) -c pippo
	./client -l $(UNIX_PATH) -c pluto
	./client -l $(UNIX_PATH) -c minni
	./client -l $(UNIX_PATH) -k pippo -S "Ciao pluto":pluto -S "come stai?":pluto
	./client -l $(UNIX_PATH) -k pluto -p -S "Ciao pippo":pippo -S "bene e tu?":pippo -S "Ciao minni come stai?":minni
	./client -l $(UNIX_PATH) -k pippo -p
	./client -l $(UNIX_PATH) -k pluto -p
	./client -l $(UNIX_PATH) -k minni -p
	killall -QUIT -w chatty
	@echo "********** Test1 superato!"

# test scambio file
test2:
	make cleanall
	\mkdir -p $(DIR_PATH)
	make all
	./chatty -f Config/chatty.conf1&
	./Tests/testfile.sh $(UNIX_PATH) $(DIR_PATH)
	killall -QUIT -w chatty
	@echo "********** Test2 superato!"

# test parametri di configurazione e statistiche
test3:
	make cleanall
	\mkdir -p $(DIR_PATH)
	make all
	./chatty -f Config/chatty.conf2&
	./Tests/testconf.sh $(UNIX_PATH) $(STAT_PATH)
	killall -QUIT -w chatty
	@echo "********** Test3 superato!"


# verifica di memory leaks
test4:
	make cleanall
	\mkdir -p $(DIR_PATH)
	make all
	./Tests/testleaks.sh $(UNIX_PATH)
	@echo "********** Test4 superato!"

# stress test
test5:
	make cleanall
	\mkdir -p $(DIR_PATH)
	make all
	./chatty -f Config/chatty.conf1&
	./Tests/teststress.sh $(UNIX_PATH)
	killall -QUIT -w chatty
	@echo "********** Test5 superato!"


# target per la consegna
consegna:
	make test1
	sleep 3
	make test2
	sleep 3
	make test3
	sleep 3
	make test4
	sleep 3
	make test5
	sleep 3
	tar -cvf $(TARNAME)_$(CORSO)_chatty.tar $(FILE_DA_CONSEGNARE)
	@echo "*** TAR PRONTO $(TARNAME)_$(CORSO)_chatty.tar "
	@echo "Per la consegna seguire le istruzioni specificate nella pagina del progetto:"
	@echo " http://didawiki.di.unipi.it/doku.php/informatica/sol/laboratorio17/progetto"
	@echo
