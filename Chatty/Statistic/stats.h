#if !defined(MEMBOX_STATS_)
#define MEMBOX_STATS_

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>

#include "../Sync/Sync.h"

struct statistics {
    unsigned long nusers;                       // n. di utenti registrati
    unsigned long nonline;                      // n. di utenti connessi
    unsigned long ndelivered;                   // n. di messaggi testuali consegnati
    unsigned long nnotdelivered;                // n. di messaggi testuali non ancora consegnati
    unsigned long nfiledelivered;               // n. di file consegnati
    unsigned long nfilenotdelivered;            // n. di file non ancora consegnati
    unsigned long nerrors;                      // n. di messaggi di errore
};

//Mutex Definite singolarmente per ogni statistica
pthread_mutex_t MUTEX_NUSER = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_NONLINE = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_NDELIVERED = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_NNOTDELIVERED = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_NFILEDELIVERED = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_NFILENOTDELIVERED = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_NERRORS = PTHREAD_MUTEX_INITIALIZER;

/* aggiungere qui altre funzioni di utilita' per le statistiche */


/**
 * @function printStats
 * @brief Stampa le statistiche nel file passato come argomento
 *
 * @param fout descrittore del file aperto in append.
 *
 * @return 0 in caso di successo, -1 in caso di fallimento
 */
static inline int printStats(FILE *fout) {
    extern struct statistics chattyStats;

    if (fprintf(fout, "%ld - %ld %ld %ld %ld %ld %ld %ld\n",
		(unsigned long)time(NULL),
		chattyStats.nusers,
		chattyStats.nonline,
		chattyStats.ndelivered,
		chattyStats.nnotdelivered,
		chattyStats.nfiledelivered,
		chattyStats.nfilenotdelivered,
		chattyStats.nerrors
		) < 0) return -1;
    fflush(fout);
    return 0;
}

/*############### INCREMENTS ##########################*/
static inline int StatsIncNUser() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NUSER)
    if(chattyStats.nusers != ULONG_MAX){
      ris = 0;
      chattyStats.nusers++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NUSER)
  return ris;
}

static inline int StatsIncNOnline() {
  extern struct statistics chattyStats;
  LOCK_MUTEX_EXIT(MUTEX_NONLINE)
  int ris = -1;
    if(chattyStats.nonline != ULONG_MAX){
      ris = 0;
      chattyStats.nonline++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NONLINE)
  return ris;
}

static inline int StatsIncNDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NDELIVERED)
    if(chattyStats.ndelivered != ULONG_MAX){
      ris = 0;
      chattyStats.ndelivered++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NDELIVERED)
  return ris;
}

static inline int StatsIncNNotDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NNOTDELIVERED)
    if(chattyStats.nnotdelivered != ULONG_MAX){
      ris = 0;
      chattyStats.nnotdelivered++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NNOTDELIVERED)
  return ris;
}

static inline int StatsIncNFileDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NFILEDELIVERED)
    if(chattyStats.nfiledelivered != ULONG_MAX){
      ris = 0;
      chattyStats.nfiledelivered++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NFILEDELIVERED)
  return ris;
}

static inline int StatsIncNNotFileDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NFILENOTDELIVERED)
    if(chattyStats.nfilenotdelivered != ULONG_MAX){
      ris = 0;
      chattyStats.nfilenotdelivered++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NFILENOTDELIVERED)
  return ris;
}

static inline int StatsIncNErrors(int Inc_val) {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NERRORS)
    if(chattyStats.nerrors != ULONG_MAX){
      ris = 0;
      chattyStats.nerrors++;
    }
  UNLOCK_MUTEX_EXIT(MUTEX_NERRORS)
  return ris;
}

/*############### DECREMENTS ##########################*/

static inline int StatsDecNUser() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NUSER)
  if(chattyStats.nusers != 0){
    ris = 0;
    chattyStats.nusers--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NUSER)
  return ris;
}

static inline int StatsDecNOnline() {
  extern struct statistics chattyStats;
  LOCK_MUTEX_EXIT(MUTEX_NONLINE)
  int ris = -1;
  if(chattyStats.nonline != 0){
    ris = 0;
    chattyStats.nonline--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NONLINE)
  return ris;
}

static inline int StatsDecNDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NDELIVERED)
  if(chattyStats.ndelivered != 0){
    ris = 0;
    chattyStats.ndelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NDELIVERED)
  return ris;
}

static inline int StatsDecNNotDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NNOTDELIVERED)
  if(chattyStats.nnotdelivered != 0){
    ris = 0;
    chattyStats.nnotdelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NNOTDELIVERED)
  return ris;
}

static inline int StatsDecNFileDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NFILEDELIVERED)
  if(chattyStats.nfiledelivered != 0){
    ris = 0;
    chattyStats.nfiledelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NFILEDELIVERED)
  return ris;
}

static inline int StatsDecNNotFileDelivered() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NFILENOTDELIVERED)
  if(chattyStats.nfilenotdelivered != 0){
    ris = 0;
    chattyStats.nfilenotdelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NFILENOTDELIVERED)
  return ris;
}

static inline int StatsDecNErrors() {
  extern struct statistics chattyStats;
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_NERRORS)
  if(chattyStats.nerrors != 0){
    ris = 0;
    chattyStats.nerrors--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_NERRORS)
  return ris;
}

#endif /* MEMBOX_STATS_ */
