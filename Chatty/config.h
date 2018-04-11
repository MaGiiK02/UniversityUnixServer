/*
 * membox Progetto del corso di LSO 2017
 *
 * Dipartimento di Informatica Università di Pisa
 * Docenti: Prencipe, Torquati
 *
 */
/**
 * @file config.h
 * @brief File contenente alcune define con valori massimi utilizzabili
 */

#if !defined(CONFIG_H_)
#define CONFIG_H_

#define MAX_NAME_LENGTH                  32

/* aggiungere altre define qui */
#define DEFAULT_HASH_SIZE 128
#define SERVER_SENDER_NAME "ChattyServer"

#define SETTING_DEFAULT_UNIX_PATH "./tmp/chatty_socket\0"
#define SETTING_DEFAULT_MAX_CONNECTIONS 32
#define SETTING_DEFAULT_THREADS_IN_POOL 8
#define SETTING_DEFAULT_MAX_MSG_SIZE 512
#define SETTING_DEFAULT_MAX_FILE_SIZE 1024
#define SETTING_DEFAULT_MAX_HITS_MSG 16
#define SETTING_DEFAULT_DIR_NAME "./tmp/chatty\0"
#define SETTING_DEFAULT_STAT_FILE_NAME "./tmp/chatty\0"
#define SETTING_MAX_SETTING_LINE_LENGTH 1024


// to avoid warnings like "ISO C forbids an empty translation unit"x
typedef int make_iso_compilers_happy;

#endif /* CONFIG_H_ */
