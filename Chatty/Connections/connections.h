/*
 * chatterbox Progetto del corso di LSO 2017
 *
 * Dipartimento di Informatica Università di Pisa
 * Docenti: Prencipe, Torquati
 *
 */
#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

#define MAX_RETRIES     10
#define MAX_SLEEPING     3
#if !defined(UNIX_PATH_MAX)
#define UNIX_PATH_MAX  64
#endif

#if !defined(STREAM_BUFFER)
#define STREAM_BUFFER  1024
#endif

#include "../Message/message.h"

/**
 * @file  connection.h
 * @brief Contiene le funzioni che implementano il protocollo
 *        tra i clients ed il server
 */

/**
 * @function openConnection
 * @brief Apre una connessione AF_UNIX verso il server
 *
 * @param path Path del socket AF_UNIX
 * @param ntimes numero massimo di tentativi di retry
 * @param secs tempo di attesa tra due retry consecutive
 *
 * @return il descrittore associato alla connessione in caso di successo
 *         -1 in caso di errore
 */
int openConnection(char* path, unsigned int ntimes, unsigned int secs);

// -------- server side -----
/**
 * @function readHeader
 * @brief Legge l'header del messaggio
 *
 * @param fd     descrittore della connessione
 * @param hdr    puntatore all'header del messaggio da ricevere
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int readHeader(long fd, message_hdr_t *hdr);
/**
 * @function readData
 * @brief Legge il body del messaggio
 *
 * @param fd     descrittore della connessione
 * @param data   puntatore al body del messaggio
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int readData(long fd, message_data_t *data);

/**
 * @function readData
 * @brief Legge il body del messaggio ma senza il relativo buffer.
 *
 * @param fd     descrittore della connessione
 * @param data   puntatore al body del messaggio
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int readDataNoBuffer(long fd, message_data_t *data);

/**
 * @function readMsg
 * @brief Legge l'intero messaggio
 *
 * @param fd     descrittore della connessione
 * @param data   puntatore al messaggio
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int readMsg(long fd, message_t *msg);

/* da completare da parte dello studente con altri metodi di interfaccia */


// ------- client side ------
/**
 * @function sendRequest
 * @brief Invia un messaggio di richiesta al server
 *
 * @param fd     descrittore della connessione
 * @param msg    puntatore al messaggio da inviare
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int sendRequest(long fd, message_t *msg);

/**
 * @function sendData
 * @brief Invia il body del messaggio al server
 *
 * @param fd     descrittore della connessione
 * @param msg    puntatore al messaggio da inviare
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int sendData(long fd, message_data_t *msg);

/**
 * @function sendHeader
 * @brief Invia lo header dell messaggio al server.
 *
 * @param fd     descrittore della connessione
 * @param msg    puntatore al header da inviare
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int sendHeader(long fd, message_hdr_t *msg);

/**
 * @function dumpBufferOnStream
 * @brief Copia il contenuto del buffer nello stream passato in input,
 *  si aspetta che sullo stream sia presente un paccetto message_data.
 *
 * @param fd      descrittore della connessione
 * @param stream  untatore ad uno stream
 * @param size    the size of stream to dump
 *
 * @return 0 in caso di successo -1 in caso di errore
 */
int dumpBufferOnStream(long fd,FILE* stream, int size);

/* da completare da parte dello studente con eventuali altri metodi di interfaccia */


#endif /* CONNECTIONS_H_ */
