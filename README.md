# Chatty Server


**Autore:** --------------

**Matricola:** ----------

### Standard:

#### Malloc e Free:
Nel progetto mi sono assicurato che, se venva effettuata una allocazione nello Heap,
mi assicuravo sempre di effettuare la Free relativa prima di uscire dallo Stack.
Mentre per quanto riguarda le **Variabili Gobali** mi sono assicurato che venissero liberase solamente quando,
il thread principale si era assicurato che tutti i sotto-thread fossero terminati, in quanto la rimozione premature di queste variabili,
avrebbe causto delle situazioni imprevedibili.

#### Nomi:
* **Funzioni:** le funzioni che effettuano lock e relative unlock sui dati utilizzati finiscono sempre con il suffisso: **"_S"**,
e le funzioni che allocavo e creavano strutture sono state provviste di voce **new**.
* **Variabili:** le variabili globali definite nel file *ServerGlobalData.h* hanno tutte il prefisso: **"GD_"**.


### Descrizione dei moduli
Inizio con il dire che in questa sezione,
verranno spiegati solo i moduli con le funzionalità meno intuitive,
o che implementano funzionalità non banali.

##### Channel:
Il channel è la parte del progetto che svolge il compito di mettere in comuniczaione i thred tra di loro.

Il suo funzionamento è basato su un array nel quale vengono messi i messaggi,
che i thread possono quindi inserire e/o leggere,più nello specifico si parla di operazioni di
POP e di PUSH (dunque gli elemeni vengono rimossi dopo una POP).
Il channel assicura che le succitate operazioni di POP e/o PUSH siano sincronizzate e, che quindi,
non ci siano dunque race condition. In secondo luogo, il channel mette in attesa passiva i thread,
in caso non sia possibile soddisfare la loro richiesta.

Il numero di thread in pop o push non è limitato in nessun modo,
ma per lo scopo del progetto esiste un unico thread che crea messaggi ( un  nuovo fd è disponigile per la lettura)
e una serie di thread che eseguono operazioni di push sul canale per gestire poi il nuovo fd.

##### Strutture generiche (HashTable & List):
Nel porgetto sono state utilizzate 2 tipologie di strutture generiche, come da titolo Hash e Liste.
Ma sebbene questa soluzione era molto buona per Applicativi *single-thread*,
si è rilevato di difficile implementazione nel caso di sistemi *multi-thread*.

Nel progetto dunque ho dovuto scegliere come gestire questi problemi sia per le liste che per le hash.

* Se provavo a rendere il sistema sicuro allora avrei dovuto effettuare la
Deep-copy(copiare tutti i valori interni del valore, cosa che nel caso di liste, array o simili erà lenta, e dunque non fattibile),
in secondo luogo con questa soluzione avrei introdotto problemi di sincronizzazione dei dati.

* Se invece provavo a effettuare copie per puntatore, avrei velocizzato le operazioni ma d'altro lato avrei introdotto tanti problemi di race condition.

Alla fine dunque ho scelto la soluzione intermedia, ovvero effettuare shallow-copy(copio solo la struttura puntata dai nodi della lista e non eventuali valori puntati dalla stessa),
assicurandomi, la dove neccessario che le neccessrie lock e unlock venivano effettuate.

Altro problema relativo ad entrabe le strutture, era che erano generiche, ciò non mi permetteva senza informazioni extra,
di capire come effettuare operazioni di Free, copia o coparazione tra i valori contenuti nelle strutture.
Per ovviare al problema ho utilizzato puntatori a funzione in modo da specificare come eseguire tali operazioni, in base all tipologia di elementi che mettevo nella lista.

Parliamo ora più nello specifico come ho implementato le liste e HashTable.

###### Liste:
Le liste sono state implementate con nodi **Bidirezionali**, in quanto per il peso di un puntatore extra,
ottengo dei sitemi migliorati per rimuovere elementi in coda. De quali la struttura vera e propria che rappresenta la lista,
tiene traccia dei nodi in prima ed ultima posizione della lista più qualche informazione utile : come il numero di elementi presenti.

Sopra i nodi bidirezionali abbiamo invece la struttura **List** che contine il puntatore al primo ed ultimo elemento della lista,
e qualche altra informazione come: il numero totale di elementi e i puntatori alle funzioni citate sopra.

###### HashTable:
Come per le liste ho usato una struttura che contine poi la vera *HashTable*.
Tale struttura oltre alla *HashTable* vera e propria contine anche informazini a lei relative come ad esempio la sua dimensione.

La hashTable, gestisce le cllisioni con liste ottenute dalle liste generiche sopra citate di trabocchi contenenti,
gli elementi della hash(HashElement) in cui salvo i puntatori ai dati assieme alla loro relativa chiave di salvataggio.


##### Syncronized HashTable & List:
Sono di fatto *"Wrapper"* per i relativi moduli, che assicurano che non vi siano race condition attraverso delle **mutex**.

Va però fatto notare che mentre per le liste la mutex è unica, per le HashTable è presente un array di mutex partizionate,
sul quale vengono effettuate le operazioni di lock in base al indice della chiave.

##### Syncronized Socket:
Questo modulo va inizializzato prima di poter essere utilizzato, ma una volta creato permette di effettuare operazioni sugli FD,
senza preoccuparsi di race condition in scrittura in quanto utilizza un array di mutex per evitare il problema.

#### SettingManager:
Si occupa del caricamento delle delle impostazioni del server, caricando le informazioni contenute in un file,
nel formato specificato caricandole in un apposita struttura.

Nello specifico il modulo e stato reso il più possibile **Fault tollerable**,
facendogli ignorare problemi di case (nel nome dei settaggi), spazi (tra i settaggi e i loro valori) e eventuali caratteri speciali.

### Flusso dell applicativo:


### Possibili miglioramenti:
 * La funzione di **"hashing"** non è ben distribuita, una sostituzione con una più equiprobabile potrebbe migliorare le performance.
 * Ottimizzare la dimensione delle hash.
 * Ottimizzare la dimiensione delle Mutex partizionate, in modo da evitare il più possibile le collisioni, cosa che in questo caso blocca un thread.
 * Alcune strutture possono essere sopostate sullo Stack, rendendo più veloce la loro rimozione*


### Problemi conosciuti:
* **Il programma ignora le connessioni dopo che gli FD raggiungono il 255**: Il problema è intriseco nelle specifiche di FD_SET,
e visto che il riuso degli FD non è supportato nelle socket af_unix, l'unico modo per risolverlo sarebbe quello di ricreare la socket.
Ma visto che il problema non si presenta all inteno dei test, e visto che chiudere la socket con conessioni pendenti non è banale ho lasciato il problema.