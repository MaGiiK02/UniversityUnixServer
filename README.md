# Chatty Server

**Author:** Mattia Angelini

## Overview

Chatty Server is a multi-threaded server that handles client connections using synchronized data structures, channels for thread communication, and safe socket operations. This document outlines the architecture, implementation details, and known limitations of the system.

## Conventions

### Memory Management (Malloc and Free)

* Global variables are freed only upon application exit.
* The main thread ensures all sub-threads are terminated before freeing shared data to avoid undefined behavior.

### Naming Conventions

* **Functions:**

  * Functions performing lock/unlock operations end with `_S`.
  * Functions allocating structures include `new` in their name.
  * Functions are prefixed with the module name they belong to.
* **Variables:**

  * Global variables from `ServerGlobalData.h` are prefixed with `GD_`.

## Module Descriptions

### Channel

* Facilitates communication between threads using a synchronized array.
* Supports `PUSH` and `POP` operations.
* Automatically blocks threads that cannot proceed due to unavailable resources.
* Typically used by a single producer thread and multiple consumer threads.

### Generic Structures (HashTable & List)

* Designed using `void*` pointers for generic data handling.
* **Shallow copies** are used to balance performance and safety.
* Uses function pointers to handle operations like free, copy, and comparison.

#### List

* Implemented as doubly-linked lists.
* Managed within a wrapper structure storing head/tail pointers and element count.

#### HashTable

* Encapsulated with metadata including size.
* Handles collisions with overflow lists.

### Synchronized Structures

* **Synchronized List:** Single mutex protects the entire list.
* **Synchronized HashTable:** Uses partitioned mutexes indexed by hash key.

### Synchronized Socket

* Must be initialized before use.
* Uses separate mutexes for read/write operations to avoid race conditions.

### SettingManager

* Loads server settings from a configuration file.
* Ignores casing, spacing, and minor formatting issues.

## Implementation Details

### Synchronization

* **Synchronized Socket** and **Synchronized HashTable** used to avoid race conditions.
* **Channel** enables main-to-worker thread communication.
* Additional mutexes protect global variables like statistics.

### Request Handling

* Worker threads:

  * Read and route client requests.
  * Send responses if successful.
  * Notify the main thread using `SIGUSR2` if the FD should be re-monitored.
  * Handle errors by either responding with an error or destroying the socket.

### Main Thread Responsibilities

* Load settings and initialize global state.
* Spawn worker threads that wait on the channel.
* Accept new connections and dispatch them via the channel.
* Handle `SIGUSR1` for printing statistics and termination signals for graceful shutdown.

## Data Structures

* **HashTables:** Used for user and group tracking.
* **Lists:** Used for user message history and as helpers for HashTables.

## Additional Notes

* The **User** structure contains all relevant user data.
* Online user discovery involves scanning the user HashTable.
* Debugging code is only compiled with `-DDEBUG` flag.
* Signal handling is centralized in the main thread.

## Assumptions

1. Group names must begin with the keyword `gruppo`.

## Potential Improvements

* Improve hash function for better distribution.
* Optimize HashTable and mutex sizes.
* Move suitable structures to the stack.
* Implement soft-close mechanism.
* Refine the main thread loop to reduce signal race conditions.
* Clean up and optimize imports.

## Known Issues

* Some Valgrind versions may falsely report `pselect` errors due to an empty mask.
* May not compile with newer GCC versions due to flag changes.
