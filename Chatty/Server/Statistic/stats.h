#ifndef MEMBOX_STATS_
#define MEMBOX_STATS_

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>

#include "../../Sync/Sync.h"

struct statistics {
    unsigned long nusers;                       // n. di utenti registrati
    unsigned long nonline;                      // n. di utenti connessi
    unsigned long ndelivered;                   // n. di messaggi testuali consegnati
    unsigned long nnotdelivered;                // n. di messaggi testuali non ancora consegnati
    unsigned long nfiledelivered;               // n. di file consegnati
    unsigned long nfilenotdelivered;            // n. di file non ancora consegnati
    unsigned long nerrors;                      // n. di messaggi di errore
};

/* aggiungere qui altre funzioni di utilita' per le statistiche */


/**
 * @function printStats
 * @brief Stampa le statistiche nel file passato come argomento
 *
 * @param fout descrittore del file aperto in append.
 *
 * @return 0 in caso di successo, -1 in caso di fallimento
 */
int printStats(FILE *fout);

int printStats_S(FILE *fout);

/*############### INCREMENTS ##########################*/
int StatsIncNUser_S();
int StatsIncNOnline_S();
int StatsIncNDelivered_S();
int StatsIncNNotDelivered_S();
int StatsIncNFileDelivered_S();
int StatsIncNNotFileDelivered_S();
int StatsIncNErrors();

/*############### DECREMENTS ##########################*/
int StatsDecNUser_S();
int StatsDecNOnline_S();
int StatsDecNDelivered_S();
int StatsDecNNotDelivered_S();
int StatsDecNFileDelivered_S();
int StatsDecNNotFileDelivered_S();
int StatsDecNErrors_S();

#endif /* MEMBOX_STATS_ */
