
./client -l /tmp/chatty_socket -c pippo &
clientpid+="$! "
./client -l /tmp/chatty_socket -c pluto &
clientpid+="$! "
./client -l /tmp/chatty_socket -c minni &
clientpid+="$! "
./client -l /tmp/chatty_socket -c topolino &
clientpid+="$! "
./client -l /tmp/chatty_socket -c paperino &
clientpid+="$! "
./client -l /tmp/chatty_socket -c qui &
clientpid+="$! "
./client -l /tmp/chatty_socket -c quo &
clientpid+="$! "
./client -l /tmp/chatty_socket -c qua &
clientpid+="$! "
./client -l /tmp/chatty_socket -c "zio paperone" &
clientpid+="$! "
./client -l /tmp/chatty_socket -c "clarabella"

wait $clientpid

for ((i=0;i<16;++i)); do
    ./client -l /tmp/chatty_socket -t 200 -k topolino -S "aaaaaaaaaaaaaaaaaaaaaaaaaaa":minni -S "bbbbbbbbbbbbbbbbb":pluto -S "ccccccccccccccccc": -S "ddddddddddddddddddddd":paperino -s client:minni -s chatty:qua -p -R 1 &
    ./client -l /tmp/chatty_socket -t 600 -k paperino -R 1  -S "aaaaaaaaaaaaaaaaaaaaaaaaaaa":minni -S "bbbbbbbbbbbbbbbbb":pluto -S "ccccccccccccccccc": -S "ddddddddddddddddddddd":topolino -s ./libchatty.a:pluto -p &
    ./client -l /tmp/chatty_socket -t 300 -k pluto -R 1  -S "aaaaaaaaaaaaaaaaaaaaaaaaaaa":minni -S "bbbbbbbbbbbbbbbbb":pluto -S "ccccccccccccccccc": -S "ddddddddddddddddddddd":topolino -s ./libchatty.a:minni -p &
    ./client -l /tmp/chatty_socket -t 300 -k qui -S "aaaaaaaaaaaaaaaaaaaaaaaaaaa":pluto -S "bbbbbbbbbbbbbbbbb": -S "ccccccccccccccccc": -S "ddddddddddddddddddddd": -S "eeeeeeeeeeeeeeeeeeeee": -S "fffffffffffffffff": -S "gggggggggggggggd": -S "hhhhhhhhhhhhh": -S "iiiiiiiiiiiiiiiiiiiiii": -S "llllllllllllllllll": -p &
    ./client -l /tmp/chatty_socket -t 500 -k quo -L -p -S "aaaaaaaaaaaaaaaaaaaaaaaaaaa":pluto -S "bbbbbbbbbbbbbbbbb": -S "ccccccccccccccccc": -S "ddddddddddddddddddddd": -S "eeeeeeeeeeeeeeeeeeeee": -S "fffffffffffffffff": -S "gggggggggggggggd": -S "hhhhhhhhhhhhh": -S "iiiiiiiiiiiiiiiiiiiiii": -S "llllllllllllllllll": &
    ./client -l /tmp/chatty_socket -t 500 -k pippo -L -s config.h:qua -s client:qua -s libchatty.a:qua -p &
    ./client -l /tmp/chatty_socket -t 200 -k qua -R 2 -s Config/chatty.conf1:pippo -S "aaaaaaaaaaaaaaaaaaaaaaaaaa":pippo -S "bbbbbbbbbbbbbbbbbb": -S "ccccccccccccccccc": -S "ddddddddddddddddddddd": -S "eeeeeeeeeeeeeeeeeeeee": -S "fffffffffffffffff": -S "gggggggggggggggd": -S "hhhhhhhhhhhhh": -S "iiiiiiiiiiiiiiiiiiiiii": -S "llllllllllllllllll": &
    ./client -l /tmp/chatty_socket -t 100 -k minni -S "aaaaaaaaaaaaaaaaaaaaaaaaaaa":qua -S "bbbbbbbbbbbbbbbbb":pluto -S "ccccccccccccccccc": -S "ddddddddddddddddddddd":topolino -s ./libchatty.a:pluto -p &
    ./client -l /tmp/chatty_socket -t 300 -k "zio paperone" -S "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa":clarabella -S "bbbbbbbbbbbbbbbbbb": -S "ccccccccccccccccc": -S "ddddddddddddddddddddd":topolino -p &
    ./client -l /tmp/chatty_socket -t 100 -k clarabella -S "bbbbbbbbbbbbbbbbbbbbbbbbbbbbb": -R 1 -s chatty:minni -S "ccccccccccccccccc": -S "ddddddddddddddddddddd": -S "eeeeeeeeeeeeeeeeeeeee": -S "fffffffffffffffff": -S "gggggggggggggggd": -S "hhhhhhhhhhhhh": -S "iiiiiiiiiiiiiiiiiiiiii": -S "llllllllllllllllll": -p &


    for((k=0;k<5;++k)); do
        # questi comandi falliscono tutti
        ./client -l /tmp/chatty_socket -k "utente$k" -S "ciao":
        # statistiche
        killall -USR1 chatty
    done

    ./client -l /tmp/chatty_socket -c utente1
    ./client -l /tmp/chatty_socket -c utente2
    ./client -l /tmp/chatty_socket -k utente1 -R 5 -S Connections/connections.o:utente2 -p
    ./client -l /tmp/chatty_socket -k utente2 -R 5 -p -S config.h:utente1

    ./client -l /tmp/chatty_socket -C utente1
    ./client -l /tmp/chatty_socket -C utente2

    wait
    echo "#########################################################################################"
    echo "                                     TEST $i FINE!!"
    echo "#########################################################################################"
done