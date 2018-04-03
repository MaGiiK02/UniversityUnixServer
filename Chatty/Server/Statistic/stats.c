//
// Created by Mattia Angelini on 19/03/2018.
//

#include "stats.h"


pthread_mutex_t MUTEX_STATS = PTHREAD_MUTEX_INITIALIZER;

/* struttura che memorizza le statistiche del server, struct statistics
 * e' definita in stats.h.
 *
 */
struct statistics chattyStats = { 0,0,0,0,0,0,0 };

int printStats(FILE *fout) {

  if (fprintf(fout, "%ld - %ld %ld %ld %ld %ld %ld %ld\n",
              (unsigned long)time(NULL),
              chattyStats.nusers,
              chattyStats.nonline,
              chattyStats.ndelivered,
              chattyStats.nnotdelivered,
              chattyStats.nfiledelivered,
              chattyStats.nfilenotdelivered,
              chattyStats.nerrors
  ) < 0)return -1;
  fflush(fout);
  return 0;
}

int printStats_S(FILE *fout) {
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  int ris = printStats(fout);
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

/*############### INCREMENTS ##########################*/
int StatsIncNUser_S() {
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nusers != ULONG_MAX){
    ris = 0;
    chattyStats.nusers++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsIncNOnline_S() {
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  int ris = -1;
  if(chattyStats.nonline != ULONG_MAX){
    ris = 0;
    chattyStats.nonline++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsIncNDelivered_S() {
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.ndelivered != ULONG_MAX){
    ris = 0;
    chattyStats.ndelivered++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsIncNNotDelivered_S() {
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nnotdelivered != ULONG_MAX){
    ris = 0;
    chattyStats.nnotdelivered++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsIncNFileDelivered_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nfiledelivered != ULONG_MAX){
    ris = 0;
    chattyStats.nfiledelivered++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsIncNNotFileDelivered_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nfilenotdelivered != ULONG_MAX){
    ris = 0;
    chattyStats.nfilenotdelivered++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsIncNErrors() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nerrors != ULONG_MAX){
    ris = 0;
    chattyStats.nerrors++;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

/*############### DECREMENTS ##########################*/

int StatsDecNUser_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nusers != 0){
    ris = 0;
    chattyStats.nusers--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsDecNOnline_S() {
  
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  int ris = -1;
  if(chattyStats.nonline != 0){
    ris = 0;
    chattyStats.nonline--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsDecNDelivered_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.ndelivered != 0){
    ris = 0;
    chattyStats.ndelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsDecNNotDelivered_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nnotdelivered != 0){
    ris = 0;
    chattyStats.nnotdelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsDecNFileDelivered_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nfiledelivered != 0){
    ris = 0;
    chattyStats.nfiledelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsDecNNotFileDelivered_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if(chattyStats.nfilenotdelivered != 0){
    ris = 0;
    chattyStats.nfilenotdelivered--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}

int StatsDecNErrors_S() {
  
  int ris = -1;
  LOCK_MUTEX_EXIT(MUTEX_STATS)
  if (chattyStats.nerrors != 0) {
    ris = 0;
    chattyStats.nerrors--;
  }
  UNLOCK_MUTEX_EXIT(MUTEX_STATS)
  return ris;
}