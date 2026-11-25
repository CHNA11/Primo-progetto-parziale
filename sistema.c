#include <stdio.h>
#include <stdlib.h> //introduco perché almeno mi serve per allocazione dinamica
#include <string.h> //introdotto per operazione sulle stringhe

// definizione delle strutture richieste, obbligatorie secondo richieste
typedef struct
{
    int codice_spettacolo;    // chiave univoca dello spettacolo
    char titolo[101];         // stringa di lunghezza massimo 100 caratteri
    char genere[31];          // stringa di lunghezza massimo 30 caratteri(es. "Prosa", "Musical", ecc...)
    char data_spettacolo[11]; // data di formato "gg/mm/aaaa"
    int posti_totali;         // capienza della sala per lo spettacolo, ovviamente >0 strettamente
    int posti_disponibili;    // Posti disponibili per prenotazione, differenza tra posti totali e quelli occupati
    float prezzo_biglietto;   // prezzo di singolo biglietto, >0
} Spettacolo;

typedef struct
{
    int codice_cliente;       // chiave univoca dello cliente
    char nome[51];            // stringa di massimo 50 caratteri
    char cognome[51];         // stringa di massimo 50 caratteri
    char email[80];           // stringa di massimo 80 caratteri
    char data_iscrizione[11]; // stringa formato "gg/mm/aaaa"
} Cliente;

typedef struct
{
    int codice_prenotazione;    // chiave univoca automatica, incrementale
    int codice_spettacolo;      // Spettacolo prenotato
    int codice_cliente;         // Cliente che ha effettuato la prenotazione
    int numero_posti;           // Numero di biglietti prenotati (massimo dovrebbe essere 10)
    float importo_totale;       // numero_post*prezzo_biglietto
    char data_prenotazione[11]; // stringa formato "gg/mm/aaaa"
} Prenotazione;

typedef struct
{
    int codice_spettacolo; // chiave univoca dello spettacolo
    int codice_cliente;    // chiave univoca dello cliente
    int numero_posti;      // numero di biglietti richiesti, per defaul al massimo 10
    char data_richiesta[11];
} RichiestaAttesa;

// definizione delle liste per prenotazioni e attesa

typedef struct NodoPrenotazione
{
    Prenotazione dati;
    struct NodoPrenotazione *next;
} NodoPrenotazione;

typedef struct NodoAttesa
{
    RichiestaAttesa dati;
    struct NodoAttesa *next;
} NodoAttesa;

// nodo definito a persona per statistica di genere
typedef struct Nodogenere
{
    char genere[51];
    int numero;
    struct Nodogenere *next;
} Nodogenere;

// questa funzione chiede password per entrare nel sistema di operazione
void avvio();

// funzione per allocazione dinamica

// funzione di richiesta per numero massimo di spettacoli
int richiestaMaxspettacoli();
// funzione di richiesta per numero massimo di clienti
int richiestaMaxclienti();
// funzione di allocazione dinamica per spettacoli
Spettacolo *avviospettacoli(int Maxspettacoli);
// funzione di allocazione dinamica per clienti
Cliente *avvioclienti(int Maxclienti);
// funzione di riallocazione per sapzio non sufficiente per spettacoli
int riallocazioneSp(Spettacolo **Spettacoli, int num_spettacoli, int *Maxspettacoli);
// funzione di riallocazione per sapzio non sufficiente per clienti
int riallocazioneCl(Cliente **Clienti, int num_clienti, int *Maxclienti);

// funzioni di operazioni sulle liste o nodo

// funzione di creazione nodo prenotazione
NodoPrenotazione *creaprenotazione(Prenotazione dati);
// funzione di creazione nodo Attesa
NodoAttesa *creaattesa(RichiestaAttesa dati);
// funzione di creazione nodo genere
Nodogenere *creagenere(char genere[]);
// funzione di inserimento in coda un nodo prenotazione
NodoPrenotazione *inserimentocodaprenotazione(Prenotazione dati, NodoPrenotazione *Prenotazioni);
// funzione di inserimento in coda un nodo attesa
NodoAttesa *inserimentocodaattesa(RichiestaAttesa dati, NodoAttesa *Attese);
// funzione di inserimento in coda un nodo genere
Nodogenere *inserimentocodagenere(char genere[], Nodogenere *generi);
// funzione di Eliminazione un prenotazione specifico, per annulazione si guarda annulaprenotazione.
NodoPrenotazione *Eliminaprenotazione(int codice_prenotazione, NodoPrenotazione *Prenotazioni, Spettacolo *Spettacoli, NodoAttesa **Attese, int num_spettacoli);
// funzione che converte tutte le attese possibilmente in prenotazione
NodoPrenotazione *converteprenotazione(Spettacolo *Spettacoli, NodoAttesa **Attese, NodoPrenotazione *Prenotazioni, Prenotazione Nodo, int num_spettacoli);
// funzione che libera lista di prenotazioni
int liberaprenotazioni(NodoPrenotazione *Prenotazioni);
// funzione che libera lista di attesa
int liberaattesa(NodoAttesa *Attese);
// funzione che libera lista di generi
int liberageneri(Nodogenere *generi);

// funzioni di controllo per validità dei dati

// funzione che controlla codice sia univoco per spettacoli
int controllacodiceSp(int codice, Spettacolo *Spettacoli, int num_spettacoli);
// funzione che controlla codice sia univoco per clienti
int controllacodiceCl(int codice, Cliente *Clienti, int num_clienti);
// funzione che controlla data sia valida
int controlladata(char data[]);
// funzione che controlla numero di spettacoli non supera limite
int controllospazio(int numero, int massimo);
// funzione che controlla validità dell'email cercando @
int controllaemail(char email[]);
// funzione che controlla prenotazione
int controllaprenotazione(int codice_annullazione, NodoPrenotazione *Prenotazioni);
// funzione che controlla apertura del file
int controllaapertura(FILE *fp);
// funzione che controlla numero di operazione file
int controllaoperazione(size_t flag, int n, FILE *fp);

// funzioni di inserimento

// funzione di inserimento per dati di uno spettacolo
int inserimentospettacolo(Spettacolo *Spettacoli, Nodogenere **generi, int *ns, int *Maxspettacoli);
// funzione di inserimento per dati cei clienti
int inserimentoclienti(Cliente *Clienti, int *ns, int *Maxspettacoli);
// funzione di inserimento prenotazione per liste
NodoPrenotazione *registraprenotazione(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa **Attese, int num_spettacoli, int num_clienti);

// funzione di annullazione

// funzione di annulazione prenotazioni e operazione su lista di attesa
NodoPrenotazione *annullaprenotazione(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa **Attese, int num_spettacoli, int num_clienti);

// funzioni di stampa

// funzione di stampa menu e scelte temporanea per test
void menu();
// funzione di stampa menu secondaria per Gestione spettacoli
void menuspettacoli(Spettacolo *Spettacoli, Nodogenere **generi, int *ns, int *Maxspettacoli);
// funzione di stampa menu secondaria per Gestione Clienti
void menuclienti(Cliente *Clienti, int *nc, int *Maxclienti);
// funzione di stampa menu secondaria per Gestione Prenotazioni
NodoPrenotazione *menuprenotazioni(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa **Attese, int num_spettacoli, int num_clienti);
// funzione di stampa menu secondaria per Gestione Statistiche e Report
void menustat(int num_spettacoli, Spettacolo *Spettacoli, int num_clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese, Nodogenere *generi);
// funzione di stampa menu secondaria per Gestione Gestione file
void menufile(int *num_spettacoli, Spettacolo *Spettacoli, int *num_clienti, Cliente *Clienti, NodoPrenotazione **Prenotazioni, NodoAttesa **Attese, Nodogenere **generi, int *Maxspettacoli, int *Maxclienti);
// funzione di stampa dati di uno spettacolo
void stampaspettacolo(Spettacolo spettacolo);
// funzione che stampa tutti i spettacoli presenti nell'array
void visualizzaspettacoli(Spettacolo *Spettacoli, int num_spettacoli);
// funzione che stampa dati di uno cliente
void stampacliente(Cliente cliente);
// funzione che stampa tutti i clienti presenti nell'array
void visualizzaclienti(Cliente *Clienti, int num_clienti);
// funzione che cerca per codice e stampa spettacolo trovato
int cercacodicespettacolo(Spettacolo *Spettacoli, int num_spettacoli);
// funzione che cerca per codice e stampa cliente trovato
int cercacodicecliente(Cliente *Clienti, int num_clienti);
// fuznione che cerca per genere e stampa spettacoli trovati
int cercagenerespettacolo(Spettacolo *Spettacoli, int num_spettacoli);
// funzione che stampa spettacoli con posti disponibili >0 e ordinati in modo cronologico crescente.
void spettacolidisponibili(Spettacolo *Spettacoli, int num_spettacoli);
// funzione che stampa una prenotazione;
void stampaprenotazione(Prenotazione dati);
// funzione che stampa tutte le prenotazioni nell'ordine di inserimento
void visualizzaprenotazioni(NodoPrenotazione *Prenotazioni);
// funzione che stampa prenotazioni di un cliente
void visualizzaprenotazionicliente(NodoPrenotazione *Prenotazioni);
// funzione che stampa prenotazioni di uno spettacolo
void visualizzaprenotazionispettacolo(NodoPrenotazione *Prenotazioni);
// funzione che stampa lista d'attesa
void visualizzalistaattesa(NodoAttesa *Attese);
// funzione che stampa statistica generale
void statistichegenerali(int num_spettacoli, Spettacolo *Spettacoli, int num_clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese);
// funzione che stampa numero di spettacoli per genere
void stampaspettacoligenere(Nodogenere *generi);
// funzione che stampa Top 5 spettacoli per incasso
void stampatop(int num_spettacoli, Spettacolo *Spettacoli);

// funzioni di operazioni su file

// funzione che salva dati sul file
int salvadatabase(int num_spettacoli, Spettacolo *Spettacoli, int num_clienti, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese);
// funzione che carica dati dal file
int caricadatabase(int *num_spettacoli, Spettacolo **Spettacoli, int *num_clienti, Cliente **Clienti, NodoPrenotazione **Prenotazioni, NodoAttesa **Attese, Nodogenere **generi, int *Maxspettacoli, int *Maxclienti);
// funzione che esporta catalogo in formato testo
int esportacatalogo(int num_spettacoli, Spettacolo *Spettacoli);
// funzione che esporta report prenotazione
int esportaprenotazione(NodoPrenotazione *Prenotazioni, Cliente *Clienti, int num_clienti);

// funzioni sul input da tastiera

// funzione che assicura di leggere un intero per scanf
int scanf_sicuro(int *indirizzo);
// fuznione che considera genere inserito e modifica contatore per genere o crea nuovo nodo
Nodogenere *valutagenere(char genere[], Nodogenere *generi);
// funzione che conta numero di nodi presenti nella lista di tipo Nodoattesa
int contattesa(NodoAttesa *Attese);
// funzione che genera codice univoco per prenotazione
int generacodice(NodoPrenotazione *Prenotazioni);
// funzione che scansiona una stringa e trasforma in int, siccome per input di data chiede controllo, si riscrive da parte questa funzione
int scanstring(char data[], int lung);
// funzione che trasforma data da stringa a int /in realtà per libreria standard esiste sscanf, ma questo ho scritto prima di saperlo e mi piacerebbe tenerlo
int trasformadata(Spettacolo spettacolo);
// funzione che ripulisce inserimento nella tastiera
void pulizia();

// funzione di liberazione memoria allocata
int liberazione(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese, Nodogenere *generi);

int main()
{
    avvio(); // richiama funzione di avvio per chiedere password in entrata
    menu();  // entra in menu di primo livello
    return 0;
}

void avvio()
{
    int password;                           // dichiarare una variabile per salvare password
    size_t flag = 1;                        // inzializzare una variabile per controllare operazione sul file
    FILE *fp = fopen("password.dat", "rb"); // apertura file di password salvato binario
    if (fp == NULL)
    { // se non si trova questo file setta password come default 123456
        password = 123456;
    }
    else
    {
        flag = fread(&password, sizeof(int), 1, fp); // controllare lettura della file
        if (flag != 1)
        { // controllo generale
            printf("problemi sulla gestione della password, eliminare password.dat dalla cartella\n");
            exit(1);
        }
        password = password - 123456; // decodificazione della password, corrispondente al passo di cambiamento password con codificazione
        fclose(fp);
    }

    int passwordinput;   // dichiarare una variabile per password inserito dal utente
    int riprova = 0;     // indicatore di riprova
    int correttezza = 0; // indicatore di corettezza
    do
    {
        printf("inserisci la password per entrare in accesso al sistema : "); // chiede di inserire password
        scanf_sicuro(&passwordinput);
        if (passwordinput == password)
        {
            correttezza = 1;
        }
        else
        { // si considera caso si sbaglio e non vuole più provare
            printf("password non corrispondente, per riprovare inserisci 1, altrimenti qualsiasi numero : ");
            scanf_sicuro(&riprova);
        }
    } while (correttezza == 0 && riprova == 1);
    if (correttezza == 0)
    { // se non riesce ad inserire password giusto si esce dal sistema
        exit(0);
    }
    int scelta = 0;
    do
    { // si dia pure possibilità di cambiare password
        printf("Le possibile funzioni sono : \n");
        printf("1. Cambiare password.\n");
        printf("2. Entrare in menu.\n");
        printf("3. Uscire dal sistema.\n");
        printf("Inserisci la tua scelta : ");
        scanf_sicuro(&scelta);
        if (scelta == 1)
        {
            int nuovapassword;
            printf("Inserisci la nuova password : ");
            scanf_sicuro(&nuovapassword);
            if (nuovapassword == password)
            { // evita di inserire vecchi password
                printf("la nuova password deve essere diversa da quella vecchia\n");
            }
            else
            {
                nuovapassword = nuovapassword + 123456;            // codificazione password
                FILE *fp = fopen("password.dat", "wb");            // apertura file con password
                flag = fwrite(&nuovapassword, sizeof(int), 1, fp); // sovrascrivere password
                if (flag != 1)
                { // controlla
                    printf("Errore nel salvare nuova passoword\n");
                    fclose(fp);
                }
                else
                { // risultato
                    printf("Nuova password salvato con successo\n");
                    fclose(fp); // chiusura
                }
            }
        }
        else if (scelta == 2)
        { // per entrare in sistema basta non uscire con exit una volta inserito password giusto
            printf("Avvio con successo, benvenuto al sistema di manager per teatro!\n");
        }
        else if (scelta == 3)
        {
            exit(0);
        }
        else
        { // caso rimasto
            printf("scelta non valida, si ritorna al menu principale\n");
        }
    } while (scelta != 2);
}

int richiestaMaxspettacoli()
{
    int Maxspettacoli;
    do
    { // chiedere in input grandezza per allocazione
        printf("inserisci numero massimo di spettacoli da gestire : ");
        scanf_sicuro(&Maxspettacoli);
        if (Maxspettacoli < 1)
        { // informare all'utente per input non valido
            printf("il numero richiesto deve essere un intero positivo non nullo\n");
        }
    } while (Maxspettacoli < 1); // non uscire dal ciclo finché non ha un valore legale
    return Maxspettacoli;
}

int richiestaMaxclienti()
{
    int Maxclienti;
    do
    { // chiede in input grandezza per allocare
        printf("inserisci numero massimo di clienti da gestire : ");
        scanf_sicuro(&Maxclienti);
        if (Maxclienti < 1) // controlla che sia un input valido
        {
            printf("il numero richiesto deve essere un intero positivo non nullo\n");
        }
    } while (Maxclienti < 1); // uscire solo quando numero è valido
    return Maxclienti;
}

Spettacolo *avviospettacoli(int Maxspettacoli)
{
    Spettacolo *Spettacoli = (Spettacolo *)malloc(Maxspettacoli * sizeof(Spettacolo)); // allocazione dinamico
    if (Spettacoli == NULL)
    { // controllo di allocazione
        printf("Allocazione per spettacoli insuccesso\n");
        exit(1);
    }
    return Spettacoli; // ritornare array allocato
}

Cliente *avvioclienti(int Maxclienti)
{
    Cliente *Clienti = (Cliente *)malloc(Maxclienti * sizeof(Cliente));
    if (Clienti == NULL)
    { // controllo allocazione
        printf("Allocazione per Clienti insuccesso\n");
        exit(1);
    }
    return Clienti;
}

int riallocazioneSp(Spettacolo **Spettacoli, int num_spettacoli, int *Maxspettacoli)
{
    int flagra;
    printf("Se desideri riallocare spazio con spazio maggiore automaticamente inserisci 1.\n");
    printf("Se desidere riallocare con uno spazio specifico desiderato inserisci 2.\n");
    printf("Se desideri lasciare array come adesso inserisci qualsiasi altro numero.\n");
    printf("Inserisci la tua scelta : ");
    scanf_sicuro(&flagra); // leggere scelta per riallocazione
    if (flagra == 1)
    {
        while (num_spettacoli > *Maxspettacoli)
        {
            *Maxspettacoli = *Maxspettacoli * 2; // in automatico si raddopia dimensione finché sia sufficiente
        }
        Spettacolo *temp = (Spettacolo *)realloc(*Spettacoli, *Maxspettacoli * sizeof(Spettacolo)); // riallocazione con temp per evitare perdita di dati
        if (temp == NULL)
        { // infatti se uso direttamente Spettacoli si perde nel caso NULL
            printf("riallocazione insuccesso\n");
            return -1;
        }
        *Spettacoli = temp; // caso successo, assegnare questo nuvo spazio
        return 1;
    }
    else if (flagra == 2)
    {
        do
        {
            printf("Nello spazio contiene %d spettacoli, inserisci nuova capacità per spettacoli : ", num_spettacoli);
            scanf_sicuro(Maxspettacoli); // chiedere nuovospazio manualmente
            if (num_spettacoli > *Maxspettacoli)
            { // ma deve essere comunque sufficiente
                printf("Si chiede di inserire una capacità sufficiente per contenere spettacoli\n");
            }
        } while (num_spettacoli > *Maxspettacoli);
        Spettacolo *temp = (Spettacolo *)realloc(*Spettacoli, *Maxspettacoli * sizeof(Spettacolo));
        if (temp == NULL)
        { // come caso automatico
            printf("riallocazione insuccesso\n");
            return -1;
        }
        *Spettacoli = temp;
        return 1;
    }
    else
    {
        return 0;
    }
}

int riallocazioneCl(Cliente **Clienti, int num_clienti, int *Maxclienti)
{
    int flagra;
    printf("Se desideri riallocare spazio con spazio maggiore automaticamente inserisci 1.\n");
    printf("Se desidere riallocare con uno spazio specifico desiderato inserisci 2.\n");
    printf("Se desideri lasciare array come adesso inserisci qualsiasi altro numero.\n");
    printf("Inserisci la tua scelta : ");
    scanf_sicuro(&flagra); // leggere scelta per riallocazione
    if (flagra == 1)
    {
        while (num_clienti > *Maxclienti)
        {
            *Maxclienti = *Maxclienti * 2; // in automatico si raddopia dimensione finché sia sufficiente
        }
        Cliente *temp = (Cliente *)realloc(*Clienti, *Maxclienti * sizeof(Cliente)); // riallocazione con temp per evitare perdita di dati
        if (temp == NULL)
        { // infatti se uso direttamente Spettacoli si perde nel caso NULL
            printf("riallocazione insuccesso\n");
            return -1;
        }
        *Clienti = temp; // caso successo, assegnare questo nuvo spazio
        return 1;
    }
    else if (flagra == 2)
    {
        do
        {
            printf("Nello spazio contiene %d clienti, inserisci nuova capacità per clienti : ", num_clienti);
            scanf_sicuro(Maxclienti); // chiedere nuovospazio manualmente
            if (num_clienti > *Maxclienti)
            { // ma deve essere comunque sufficiente
                printf("Si chiede di inserire una capacità sufficiente per contenere clienti\n");
            }
        } while (num_clienti > *Maxclienti);
        Cliente *temp = (Cliente *)realloc(*Clienti, *Maxclienti * sizeof(Cliente)); // riallocazione con temp per evitare perdita di dati
        if (temp == NULL)
        { // come caso automatico
            printf("riallocazione insuccesso\n");
            return -1;
        }
        *Clienti = temp;
        return 1;
    }
    else
    {
        return 0;
    }
}

NodoPrenotazione *creaprenotazione(Prenotazione dati)
{
    // allocare dinamicamente un nodo
    NodoPrenotazione *prenotazione = (NodoPrenotazione *)malloc(sizeof(NodoPrenotazione));
    if (prenotazione == NULL)
    { // controllo di allocazione
        printf("Errore allocazione prenotazione.\n");
        exit(1);
    }
    // assegnazione dati per nodo
    prenotazione->dati = dati;
    prenotazione->next = NULL;
    // ritornare puntatore del nodo
    return prenotazione;
}

NodoAttesa *creaattesa(RichiestaAttesa dati)
{
    // allocare dinamicamente un nodo
    NodoAttesa *Attesa = (NodoAttesa *)malloc(sizeof(NodoAttesa));
    if (Attesa == NULL)
    { // controllo di allocazione
        printf("Errore allocazione Attesa.\n");
        exit(1);
    }
    // assegnazione valori
    Attesa->dati = dati;
    Attesa->next = NULL;
    // ritorna puntatore
    return Attesa;
}

Nodogenere *creagenere(char genere[])
{ // allocare dinamicamente un nodo
    Nodogenere *nuovagenere = (Nodogenere *)malloc(sizeof(Nodogenere));
    if (nuovagenere == NULL)
    { // controllo allocazione
        printf("Errore allocazione genere.\n");
        exit(1);
    }
    // per assegnare una stringa si usa strcpy
    strcpy(nuovagenere->genere, genere);
    nuovagenere->numero = 1;  // si inizializza il valore per 1
    nuovagenere->next = NULL; // inizializza puntatore verso nulla
    return nuovagenere;       // ritornare puntatore del nodo
}

NodoPrenotazione *inserimentocodaprenotazione(Prenotazione dati, NodoPrenotazione *Prenotazioni)
{
    // creare un nodo nuovo
    NodoPrenotazione *prenotazione = creaprenotazione(dati);
    if (Prenotazioni == NULL)
    { // se lista è vuoto questa è nuova testa
        return prenotazione;
    }
    else
    { // scorrimento lista fino alla fine
        NodoPrenotazione *p = Prenotazioni;
        while (p->next != NULL)
        {
            p = p->next;
        }
        // segnare l'ultimo nodo puntatore verso nuovo nodo
        p->next = prenotazione;
        // ritornare testa
        return Prenotazioni;
    }
}

NodoAttesa *inserimentocodaattesa(RichiestaAttesa dati, NodoAttesa *Attese)
{
    // analogo inserimentocoda
    NodoAttesa *attesa = creaattesa(dati);
    if (Attese == NULL)
    {
        return attesa;
    }
    else
    {
        NodoAttesa *p = Attese;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = attesa;
        return Attese;
    }
}

Nodogenere *inserimentocodagenere(char genere[], Nodogenere *generi)
{ // analogo inserimento coda
    Nodogenere *nuovagenere = creagenere(genere);
    if (generi == NULL)
    {
        return nuovagenere;
    }
    else
    {
        Nodogenere *p = generi;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = nuovagenere;
        return generi;
    }
}

NodoPrenotazione *Eliminaprenotazione(int codice_prenotazione, NodoPrenotazione *Prenotazioni, Spettacolo *Spettacoli, NodoAttesa **Attese, int num_spettacoli)
{
    // dichiarare una variabile per dati del nodo da essere eliminati
    Prenotazione datitemp;
    if (Prenotazioni == NULL)
    {
        // gestire caso lista vuota
        printf("Non ci sono prenotazioni presenti.\n");
        return Prenotazioni;
    }
    else if (Prenotazioni->dati.codice_prenotazione == codice_prenotazione) // caso di eliminazione testa
    {                                                                       // si memorizza primo, secondo diventa testa, libera primo
        datitemp = Prenotazioni->dati;
        NodoPrenotazione *temp = Prenotazioni;
        Prenotazioni = Prenotazioni->next;
        free(temp);
    }
    else
    {
        NodoPrenotazione *p = Prenotazioni;
        NodoPrenotazione *prec = NULL;
        while (p != NULL && p->dati.codice_prenotazione != codice_prenotazione)
        { // scorrimento verso valore
            prec = p;
            p = p->next;
        }
        if (p == NULL)
        {
            // caso fino alla fine e non ho trovato
            printf("Non ci sono prenotazione corrispondente.\n");
            return Prenotazioni;
        }
        else
        { // caso intermedio o ultimo
            datitemp = p->dati;
            prec->next = p->next;
            free(p);
        }
    }
    // Quando si elimina, converte tutte le attesse possibilmente in prenotazioni
    Prenotazioni = converteprenotazione(Spettacoli, Attese, Prenotazioni, datitemp, num_spettacoli);
    printf("Prenotazione annulato correttamente.\n");
    return Prenotazioni;
}

NodoPrenotazione *converteprenotazione(Spettacolo *Spettacoli, NodoAttesa **Attese, NodoPrenotazione *Prenotazioni, Prenotazione Nodo, int num_spettacoli)
{
    // trova indice dello spettacolo che viene annullato prenotazione
    int indicesp = controllacodiceSp(Nodo.codice_spettacolo, Spettacoli, num_spettacoli);
    // se si annulla, incrementa posti disponibili
    Spettacoli[indicesp].posti_disponibili = Spettacoli[indicesp].posti_disponibili + Nodo.numero_posti;
    int codice_spettacolo = Spettacoli[indicesp].codice_spettacolo; // inziializzare una variabile per codice spettacolo per comodità
    // inizializzare puntatori per scorrere lista di attesa
    NodoAttesa *q = *Attese;
    NodoAttesa *prec = NULL;
    while (q != NULL)
    {
        if (q->dati.codice_spettacolo == codice_spettacolo && q->dati.numero_posti <= Spettacoli[indicesp].posti_disponibili)
        // si considera attese corrispondenti allo spettacolo specifico e con posti sufficienti
        {
            // inizializzare un nodo da annulare
            NodoAttesa *da_annulare = q;
            // dichiarare una vairabile per segnare dati del attesa in una struttura di tipok prenotazione
            Prenotazione dati;
            // assegnare dati
            dati.codice_cliente = q->dati.codice_cliente;
            dati.codice_spettacolo = q->dati.codice_spettacolo;
            strcpy(dati.data_prenotazione, q->dati.data_richiesta); // è una stringa
            dati.numero_posti = q->dati.numero_posti;
            dati.importo_totale = dati.numero_posti * Spettacoli[indicesp].prezzo_biglietto;
            dati.codice_prenotazione = generacodice(Prenotazioni);                                               // genera automaticamente e univocamente il codice, da lavorare
            Prenotazioni = inserimentocodaprenotazione(dati, Prenotazioni);                                      // inserisce questa prenotazione convertito d'attesa in lista
            Spettacoli[indicesp].posti_disponibili = Spettacoli[indicesp].posti_disponibili - dati.numero_posti; // calcolare nuovo posto disponibile
            // cancellare questa attesa dalla lista di attesa
            if (prec == NULL)
            {
                *Attese = q->next;
                q = *Attese; 
            }
            else
            {
                prec->next = q->next;
                q = q->next; 
            }
            free(da_annulare);
        }
        else
        {
            // caso generale si scorre e ricorda anche il precedente
            prec = q;
            q = q->next;
        }
    }
    return Prenotazioni;
}

// funzione che libera lista di prenotazioni
int liberaprenotazioni(NodoPrenotazione *Prenotazioni)
{
    NodoPrenotazione *temp = Prenotazioni;
    while (Prenotazioni != NULL)
    {
        Prenotazioni = Prenotazioni->next;
        free(temp);
        temp = Prenotazioni;
    }
    return 1;
}
// funzione che libera lista di attesa
int liberaattesa(NodoAttesa *Attese)
{
    NodoAttesa *temp = Attese;
    while (Attese != NULL)
    {
        Attese = Attese->next;
        free(temp);
        temp = Attese;
    }
    return 1;
}
// funzione che libera lista di generi
int liberageneri(Nodogenere *generi)
{
    Nodogenere *temp = generi;
    while (generi != NULL)
    {
        generi = generi->next;
        free(temp);
        temp = generi;
    }
    return 1;
}

// funzione che controlla codice sia univoco per spettacoli
int controllacodiceSp(int codice, Spettacolo *Spettacoli, int num_spettacoli)
{
    for (int i = 0; i < num_spettacoli; i++)
    {
        // scorre tutti i spettacoli presenti e confronta
        if (Spettacoli[i].codice_spettacolo == codice)
        {
            return i;
        }
    }
    return -1;
}

// funzione che controlla codice sia univoco per clienti
int controllacodiceCl(int codice, Cliente *Clienti, int num_clienti)
{
    for (int i = 0; i < num_clienti; i++)
    {
        // scorre tutti i spettacoli presenti e confronta
        if (Clienti[i].codice_cliente == codice)
        {
            return i;
        }
    }
    return -1;
}

// funzione che controlla data sia valida
int controlladata(char data[])
{
    // controlo il presenza di / nel formato richiesto
    if (data[2] != '/' || data[5] != '/')
    {
        printf("Inserisci una data con formato gg/mm/aaaa\n");
        return -1;
    }
    // definisco una stringa con numeri tra 0 e 9
    char numeri[] = "0123456789";
    int flaggiornouno = 0; // indicatore per vedere se prima cifra del giorno sia un numero
    int flaggiornodue = 0; // indicatore per vedere se seconda cifra del giorno sia un numero
    int flagmeseuno = 0;   // indicatore per vedere se prima cifra del mese sia un numero
    int flagmesedue = 0;   // indicatore per vedere se seconda cifra del mese sia un numero
    int flagannouno = 0;   // indicatore per vedere se prima cifra del anno sia un numero
    int flagannodue = 0;   // indicatore per vedere se seconda cifra del anno sia un numero
    int flagannotre = 0;   // indicatore per vedere se terza cifra del anno sia un numero
    int flagannoqua = 0;   // indicatore per vedere se quarta cifra del anno sia un numero
    int giorno = 0;        // inizializzare una variabile che corrisponde al giorno inserito
    int mese = 0;          // inizializzare una variabile che corrisponde al mese inserito
    int anno = 0;          // inizializzare una variabile che corrisponde al anno inserito
    for (int i = 0; i < 10; i++)
    {
        // considerando le cifre corrispondenti e trasformare in numeri nella variabile
        if (data[0] == numeri[i])
        {
            giorno = giorno + (10) * i;
            flaggiornouno = 1;
        }
        if (data[1] == numeri[i])
        {
            giorno = giorno + i;
            flaggiornodue = 1;
        }
        if (data[3] == numeri[i])
        {
            mese = mese + (10) * i;
            flagmeseuno = 1;
        }
        if (data[4] == numeri[i])
        {
            mese = mese + i;
            flagmesedue = 1;
        }
        if (data[6] == numeri[i])
        {
            anno = anno + (1000) * i;
            flagannouno = 1;
        }
        if (data[7] == numeri[i])
        {
            anno = anno + (100) * i;
            flagannodue = 1;
        }
        if (data[8] == numeri[i])
        {
            anno = anno + (10) * i;
            flagannotre = 1;
        }
        if (data[9] == numeri[i])
        {
            anno = anno + i;
            flagannoqua = 1;
        }
    }
    if (flaggiornouno != 1 || flaggiornodue != 1)
    { // controlla che nel posto del giorno ha inserito un numero
        printf("Inserisci un giorno valido con numeri tra 1 e 31\n");
        return -1;
    }
    if (flagmeseuno != 1 || flagmesedue != 1)
    { // controlla che nel posto del mese ha inserito un numero
        printf("Inserisci un mese valido con numeri tra 1 e 12\n");
        return -1;
    }
    if (flagannouno != 1 || flagannodue != 1 || flagannotre != 1 || flagannoqua != 1)
    { // controlla che nel posto del anno ha inserito un numero
        printf("Inserisci un anno valido con numeri tra 2000 e 2100\n");
        return -1;
    }
    if (giorno < 1 || giorno > 31)
    { // controlla che giorno sia compreso tra 1 e 31
        printf("Inserisci un giorno compreso tra 1 e 31\n");
        return -1;
    }
    if (mese < 1 || mese > 12)
    { // controlla che mese sia compreso tra 1 e 12
        printf("Inserisci un mese compreso tra 1 e 12\n");
        return -1;
    }
    if (anno < 2000 || anno > 2100)
    { // controllo che anno sia compreso tra 2000 e 2100
        printf("Inserisci un anno compreso tra 2000 e 2100\n");
        return -1;
    }
    if ((mese % 2 == 0 && mese <= 8) || (mese % 2 == 1 && mese > 8))
    { // si ricorda che mese pari prima di Settembre e mese dispari dopo Agosto non hanno 31 giorni
        if (giorno == 31)
        {
            printf("Nel mese %d non ha 31 giorni, inserisci una data valida\n", mese);
            return -1;
        }
    }
    if (mese == 2 && giorno > 28)
    { // gestisce pure il caso di anni bisestile
        if (giorno > 29 || anno % 4 != 0)
        {
            printf("Nel febbraio del %d non ha %d giorni\n", anno, giorno);
            return -1;
        }
    }
    return 1; // se nessuno di questi errori sono presenti allora è una data valida
}

int controllospazio(int numero, int massimo)
{
    // basta considerare vari casi
    if (numero > massimo)
    {
        return -1;
    }
    else if (numero == massimo)
    {
        return 0; // in questo caso non si esce dallo spazio ma ha raggiunto al suo massimo
    }
    else
    {
        return 1;
    }
}

int controllaemail(char email[])
{
    int i = 0; // inizializzare una variabile per indice di stringa
    int c;     // dichiarare una variabile per singolo char, che possono anche considerati come interi
    do
    {
        // assegnare c singolo carrattere
        c = email[i];
        i++; // scorre indice
    } while (c != '@' && c != EOF && c != '\0'); // fino alla fine o trova @
    if (c == '@')
    { // se ho trovato, considero come un email valido
        printf("Email valido.\n");
        return 1;
    }
    else
    { // altrimenti non valido
        printf("Email non valido.\n");
        return -1;
    }
}

int controllaprenotazione(int codice_annullazione, NodoPrenotazione *Prenotazioni)
{
    // inziializzare un puntatore di partenza per lista
    NodoPrenotazione *p = Prenotazioni;
    while (p != NULL && p->dati.codice_prenotazione != codice_annullazione)
    { // scorripento
        p = p->next;
    }
    if (p == NULL)
    { // se ho arrivato a null, non ho trovato
        return -1;
    }
    else
    { // se ho trovato, faccio ritornare codice spettacolo
        return p->dati.codice_spettacolo;
    }
}

// funzione che controlla apertura del file
int controllaapertura(FILE *fp)
{
    if (fp == NULL)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
// funzione che controlla numero di operazione file
int controllaoperazione(size_t flag, int n, FILE *fp)
{
    if (flag != n)
    {
        fclose(fp);
        return -1;
    }
    return 1;
}

int inserimentospettacolo(Spettacolo *Spettacoli, Nodogenere **generi, int *ns, int *Maxspettacoli)
{
    int flag = 0;
    // dichiarare una variabile locale per comodità del codice
    int num_spettacoli = *ns;
    int i = num_spettacoli;
    if (controllospazio(num_spettacoli, *Maxspettacoli) == -1)
    {
        printf("Non ci sono spazi disponibili per inserimento\n");
        int flagra = riallocazioneSp(&Spettacoli, num_spettacoli, Maxspettacoli);
        // vari casi di riallocazione
        if (flagra == 1)
        {
            printf("Riallocazione con successo, adesso abbiamo nuova capacità %d\n", Maxspettacoli);
        }
        else
        {
            printf("Riallocazione ha incontrato qualche problema, riprova più tardi\n");
            return 0;
        }
    }; // controllo spazio siponibile
    int flagcodice = 0; // indicatore per validità del codice
    do
    {
        int tempcodice; // variabile temporanea per codice
        printf("Inserisci codice spettacolo corrispondente : ");
        scanf_sicuro(&tempcodice);
        flagcodice = 1;
        if (controllacodiceSp(tempcodice, Spettacoli, num_spettacoli) != -1)
        { // controlla che sia univoca
            printf("codice già esistente e deve essere univoco, riprova.\n");
            flagcodice = 0;
        }
        else
        { // assegnare se non ci sono problemi
            Spettacoli[i].codice_spettacolo = tempcodice;
        }
    } while (flagcodice != 1);
    printf("Inserisci titolo dello spettacolo con massimo 100 caratteri : ");
    fgets(Spettacoli[i].titolo, 100, stdin);                          // un titolo può avere qualsiasi contenuto
    Spettacoli[i].titolo[strcspn(Spettacoli[i].titolo, "\n")] = '\0'; // rimuovi \n
    printf("Inserisci genere dello spettacolo con massimo 30 caratteri : ");
    fgets(Spettacoli[i].genere, 30, stdin); // analogamente per genere
    Spettacoli[i].genere[strcspn(Spettacoli[i].genere, "\n")] = '\0';
    int flagdata = 0; // indicatore per validità di data
    do
    {
        printf("Inserisci data dello spettacolo con formato gg/mm/aaaa : ");
        char tempdata[11];       // dichiarare una variabile temporanea per data
        scanf("%10s", tempdata); // controllo di lunghezza, quelli oltre viene ignorata
        pulizia();
        flagdata = 1;
        if (controlladata(tempdata) == -1)
        { // controlla che data sia valido
            printf("Si chiede di inserire una data valida con formato corrispondente segnato\n");
            flagdata = 0;
        }
        else
        { // assegnazione nel caso corretto
            printf("Data valida e inserito correttamente\n");
            strcpy(Spettacoli[i].data_spettacolo, tempdata);
        }
    } while (flagdata != 1);
    int flagposti = 0; // indicatore per numero di posti
    do
    {
        int temppos;
        printf("Inserisci posti totali dello spettacolo : ");
        scanf_sicuro(&temppos);
        flagposti = 1;
        if (temppos < 1)
        { // basta che sia maggiore di 0
            printf("Il numero di posti totali deve essere un numero intero strettamente maggiore di 0\n");
            flagposti = 0;
        }
        else
        {
            Spettacoli[i].posti_totali = temppos;
        }
    } while (flagposti != 1);
    Spettacoli[i].posti_disponibili = Spettacoli[i].posti_totali;
    int flagprezzo = 0; // indicatore per prezzo
    do
    {
        float tempprezzo;
        printf("Inserisci prezzo di un singolo biglietto dello spettacolo : ");
        scanf("%f", &tempprezzo);
        flagprezzo = 1;
        if (tempprezzo <= 0) // basta che sia positivo e non zero
        {
            printf("Prezzo di un singolo biglietto deve essere strettamente maggiore di 0\n");
            flagprezzo = 0;
        }
        else
        {
            Spettacoli[i].prezzo_biglietto = tempprezzo;
        }
    } while (flagprezzo != 1);
    printf("I dati dello spettacolo che hai inserito sono le seguenti : \n");
    printf("%-20s %-20s %-20s %-20s %-20s %-20s %-20s\n", "Codice", "Titolo", "Genere", "data", "posti totali", "posti disponibili", "Prezzo biglietto");
    printf("%-20d %-20s %-20s %-20s %-20d %-20d %-20.2f\n", Spettacoli[i].codice_spettacolo, Spettacoli[i].titolo, Spettacoli[i].genere, Spettacoli[i].data_spettacolo, Spettacoli[i].posti_totali, Spettacoli[i].posti_disponibili, Spettacoli[i].prezzo_biglietto); // stampare dati inseriti
    printf("Per dare conferma di inserimento inserisci 1, altrimenti 0\n");
    scanf_sicuro(&flag); // in realtà vale per tutti diversi da 1 per riinserimento, sicuramente bene per 0
    return flag;
}

int inserimentoclienti(Cliente *Clienti, int *nc, int *Maxclienti)
{
    int flag = 0; // indicatore per conferma di inserimento dati
    int num_clienti = *nc;
    int i = num_clienti;
    if (controllospazio(num_clienti, *Maxclienti) == -1)
    {
        printf("Non ci sono spazi disponibili per inserimento\n");
        int flagra = riallocazioneCl(&Clienti, num_clienti, Maxclienti);
        // vari casi di riallocazione
        if (flagra == 1)
        {
            printf("Riallocazione con successo, adesso abbiamo nuova capacità %d\n", Maxclienti);
        }
        else
        {
            printf("Riallocazione ha incontrato qualche problema, riprova più tardi\n");
            return -1;
        }
    }; // controllo spazio siponibile
    int flagcodice = 0; // indicatore per validità del codice
    do
    {
        int tempcodice; // variabile temporanea per codice
        printf("Inserisci codice cliente corrispondente : ");
        scanf_sicuro(&tempcodice);
        flagcodice = 1;
        if (controllacodiceCl(tempcodice, Clienti, num_clienti) != -1)
        { // controlla che sia univoca
            printf("codice già esistente e deve essere univoco, riprova.\n");
            flagcodice = 0;
        }
        else
        { // assegnare se non ci sono problemi
            Clienti[i].codice_cliente = tempcodice;
        }
    } while (flagcodice != 1);
    printf("Inserisci nome del cliente con massimo 50 caratteri : ");
    fgets(Clienti[i].nome, 50, stdin);                      // registra nome
    Clienti[i].nome[strcspn(Clienti[i].nome, "\n")] = '\0'; // rimuovi \n
    printf("Inserisci cognome del cliente con massimo 50 caratteri : ");
    fgets(Clienti[i].cognome, 50, stdin);                         // registra cognome
    Clienti[i].cognome[strcspn(Clienti[i].cognome, "\n")] = '\0'; // rimuovi \n
    int flagemail = 0;
    char tempemail[81];
    do
    {
        printf("Inserisci email del cliente con massimo 80 caratteri : ");
        scanf("%80s", tempemail);
        pulizia();
        flagemail = 1;
        if (controllaemail(tempemail) == -1)
        { // controlla che email sia valido
            printf("Si chiede di inserire un email valida contenente @\n");
            flagemail = 0;
        }
        else
        { // assegnazione nel caso corretto
            printf("email valida e inserito correttamente\n");
            strcpy(Clienti[i].email, tempemail);
        }
    } while (flagemail != 1);
    int flagdata = 0; // indicatore per validità di data
    do
    {
        printf("Inserisci data del iscrizione con formato gg/mm/aaaa : ");
        char tempdata[11];       // dichiarare una variabile temporanea per data
        scanf("%10s", tempdata); // controllo di lunghezza, quelli oltre viene ignorata
        pulizia();
        flagdata = 1;
        if (controlladata(tempdata) == -1)
        { // controlla che data sia valido
            printf("Si chiede di inserire una data valida con formato corrispondente segnato\n");
            flagdata = 0;
        }
        else
        { // assegnazione nel caso corretto
            printf("Data valida e inserito correttamente\n");
            strcpy(Clienti[i].data_iscrizione, tempdata);
        }
    } while (flagdata != 1);
    // chiedere conferma per dati inseriti
    printf("I dati del cliente che hai inserito sono le seguenti : \n");
    printf("%-20s %-20s %-20s %-40s %-20s\n", "Codice", "nome", "cognome", "email", "data iscrizione");
    printf("%-20d %-20s %-20s %-40s %-20s\n", Clienti[i].codice_cliente, Clienti[i].nome, Clienti[i].cognome, Clienti[i].email, Clienti[i].data_iscrizione); // stampare dati inseriti
    printf("Per dare conferma di inserimento inserisci 1, altrimenti 0\n");
    scanf_sicuro(&flag); // in realtà vale per tutti diversi da 1 per riinserimento, sicuramente bene per 0
    return flag;
}

NodoPrenotazione *registraprenotazione(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa **Attese, int num_spettacoli, int num_clienti)
{
    int flag = 0;      // inizializzare variabile per conferma di prenotazione
    Prenotazione dati; // dichairare un variabile per contenere dati inseriti
    do
    {
        int flagcodicesp = 0;
        int indicesp; // dichiaro fuori dal cilco sotto perché mi serve anche dopo
        do
        {
            printf("Inserisci codice spettacole della prenotazione corrispondente : ");
            scanf_sicuro(&dati.codice_spettacolo);
            flagcodicesp = 1;
            // controlla che spettacolo esiste
            indicesp = controllacodiceSp(dati.codice_spettacolo, Spettacoli, num_spettacoli);
            if (indicesp == -1)
            {
                printf("codice spettacolo non esistente.\n");
                flagcodicesp = 0;
            }
            else
            {
                printf("codice inserito corrisponde allo spettaco %s nella data %s con %d posti disponibili.\n", Spettacoli[indicesp].titolo, Spettacoli[indicesp].data_spettacolo, Spettacoli[indicesp].posti_disponibili);
            }
        } while (flagcodicesp == 0);
        int flagcodicecl = 0;
        int indicecl;
        do
        {
            printf("Inserisci codice cliente della prenotazione corrispondente : ");
            scanf_sicuro(&dati.codice_cliente);
            flagcodicecl = 1;
            // controlla che cliente esiste
            indicecl = controllacodiceCl(dati.codice_cliente, Clienti, num_clienti);
            if (indicecl == -1)
            {
                printf("codice cliente non esistente.\n");
                flagcodicecl = 0;
            }
            else
            {
                printf("codice cliente corrisponde al cliente %s %s.\n", Clienti[indicecl].nome, Clienti[indicecl].cognome);
            }
        } while (flagcodicecl == 0);
        do
        {
            printf("Inserisci numero di posti desiderati : ");
            scanf_sicuro(&dati.numero_posti);
            if (dati.numero_posti < 1 || dati.numero_posti > 10)
            { // avvisare per dati non validi
                printf("numero di posti deve essere compreso tra 1 e 10.\n");
            }
        } while (dati.numero_posti < 1 || dati.numero_posti > 10);
        int flagdata = 0;
        do
        {
            printf("Inserisci data della prenotazione in formato gg/mm/aaaa : ");
            scanf("%10s", dati.data_prenotazione);
            flagdata = 1;
            if (controlladata(dati.data_prenotazione) == -1)
            { // controlla che data sia valido
                printf("Si chiede di inserire una data valida con formato corrispondente segnato\n");
                flagdata = 0;
            }
            else
            {
                printf("Data valida e inserito correttamente\n");
            }
        } while (flagdata == 0);
        dati.importo_totale = dati.numero_posti * Spettacoli[indicesp].prezzo_biglietto; // calcola automaticamente importo totale
        printf("I costi totali della prenotazione è : %.2f.\n", dati.importo_totale);
        if (Spettacoli[indicesp].posti_disponibili < dati.numero_posti)
        { // gestire caso senza posti disponibili
            printf("Nello spettacolo %s non ci sono posti sufficienti.\n", Spettacoli[indicesp].titolo);
            printf("Se desideri aggiungere nella lista d'attesa inserisci 1, altrimenti 0 : ");
            scanf_sicuro(&flag);
            if (flag == 1)
            {
                RichiestaAttesa datiattesa;
                datiattesa.codice_cliente = dati.codice_cliente;
                datiattesa.codice_spettacolo = dati.codice_spettacolo;
                strcpy(datiattesa.data_richiesta, dati.data_prenotazione);
                datiattesa.numero_posti = dati.numero_posti;
                *Attese = inserimentocodaattesa(datiattesa, *Attese);
                printf("dati salvati nella lista di attesa.\n");
            }
            else
            {
                flag = 0;
            }
        }
        else
        {
            // gestire caso posti sufficiente
            printf("Per confermare prenotazione inserisci 1, altrimenti 0 : ");
            scanf_sicuro(&flag);
            if (flag == 1)
            {
                // controlla se cliente è già in attesa dello stesso spettacolo
                NodoAttesa *controllo = *Attese; // scorro la lista
                int presenza = 0;                // inizializzare una variabile per segnare la presenza di ripetizione
                while (controllo != NULL)
                {
                    if (controllo->dati.codice_cliente == dati.codice_cliente && controllo->dati.codice_spettacolo == dati.codice_spettacolo)
                    {
                        presenza = 1;
                        break;
                    }
                    controllo = controllo->next;
                }
                if (presenza == 1)
                {
                    printf("il cliente ha già un attesa per spettacolo inserito\n");
                }
                else
                {
                    dati.codice_prenotazione = generacodice(Prenotazioni);
                    Spettacoli[indicesp].posti_disponibili = Spettacoli[indicesp].posti_disponibili - dati.numero_posti;
                    Prenotazioni = inserimentocodaprenotazione(dati, Prenotazioni);
                    printf("Prenotazione effettuato correttamente.\n");
                    printf("Ecco gli dati della prenotazione : \n");
                    printf("%-20s %-20s %-20s %-20s %-20s %-20s\n", "codice prenotazione", "codice spettacolo", "codice cliente", "numero posti", "importo totale", "data prenotazione");
                    stampaprenotazione(dati); // stampa anche dati di prenotazione
                }
            }
        }
    } while (flag != 1);
    return Prenotazioni;
}

NodoPrenotazione *annullaprenotazione(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa **Attese, int num_spettacoli, int num_clienti)
{
    // chiede codice di prenotazione da annullare
    printf("Insersci codice di prenotazione per annullazione : ");
    int codice_annullazione;
    scanf_sicuro(&codice_annullazione);
    int codice_spettacolo = controllaprenotazione(codice_annullazione, Prenotazioni);
    if (codice_spettacolo == -1)
    { // controlla che esiste prenotazione corrispondente
        printf("Non esiste prenotazione corrispondente al codice.\n");
    }
    else
    { // annulla prenotazione e converte attese dentro funzione
        Prenotazioni = Eliminaprenotazione(codice_annullazione, Prenotazioni, Spettacoli, Attese, num_spettacoli);
    }
    return Prenotazioni;
}

void menu()
{
    // inzializzazione delle array per database
    int Maxspettacoli = richiestaMaxspettacoli();
    int Maxclienti = richiestaMaxclienti();
    int num_spettacoli = 0; // numero di spettacoli presenti nello spazio
    int num_clienti = 0;    // numero di cienti presenti nello spazio
    int *ns = &num_spettacoli;
    int *nc = &num_clienti;
    Spettacolo *Spettacoli = avviospettacoli(Maxspettacoli);
    Cliente *Clienti = avvioclienti(Maxclienti);
    printf("Database inzializzato con Massima capacità di spettacoli : %d\n",Maxspettacoli);
    printf("Database inzializzato con Massima capacità di clienti : %d\n",Maxclienti);
    NodoPrenotazione *Prenotazioni = NULL;
    NodoAttesa *Attese = NULL;
    Nodogenere *generi = NULL;
    char primascelta; // inzializzare prima scelta per menu primo livello, diverso da z per entrare in menu, posso fare anche con do while
    do
    {
        // stampa funzioni possibili per categorie
        printf("Le funzionalità disponibili divisi per categorie sono : \n");
        printf("A. Gestione Spettacoli.\n");
        printf("B. Gestione Clienti.\n");
        printf("C. Gestione Prenotazioni.\n");
        printf("D. Statistiche e Report.\n");
        printf("E. Gestioone File.\n");
        printf("F. Esci.\n");
        printf("Inserisci la tua scelta : ");
        scanf(" %c", &primascelta);
        pulizia(); // pulizia buffer
        // distinguere casi di scelte, richiamando funzioni corrispondenti, per lettere si usa if, e si distingue casi maiuscola o minuscola
        if (primascelta == 'a' || primascelta == 'A')
        {
            menuspettacoli(Spettacoli, &generi, ns, &Maxspettacoli);
        }
        else if (primascelta == 'b' || primascelta == 'B')
        {
            menuclienti(Clienti, nc, &Maxclienti);
        }
        else if (primascelta == 'c' || primascelta == 'C')
        {
            Prenotazioni = menuprenotazioni(Spettacoli, Clienti, Prenotazioni, &Attese, num_spettacoli, num_clienti);
        }
        else if (primascelta == 'd' || primascelta == 'D')
        {
            menustat(num_spettacoli, Spettacoli, num_clienti, Prenotazioni, Attese, generi);
        }
        else if (primascelta == 'e' || primascelta == 'E')
        {
            menufile(&num_spettacoli, Spettacoli, &num_clienti, Clienti, &Prenotazioni, &Attese, &generi, &Maxspettacoli, &Maxclienti);
        }
        else if (primascelta != 'f' && primascelta != 'F')
        { // considerare casi diversi da f o F per validità
            printf("Scelta non valida.\n");
        }
        else
        {
            int uscita = 0;
            do
            {
                printf("Se si esce dal programma, tutti i dati non salvati sul file vengono perduti\n");
                printf("Sei sicuro di uscire? Opzioni possibili sono : ");
                printf("1. Uscire direttamente.\n");
                printf("2. Uscire con salvataggio automatico sui file\n");
                printf("3. ritornare al menu principale\n");
                printf("Inserisci la tua scelta : ");
                scanf_sicuro(&uscita);
                if (uscita == 1 || uscita == 3)
                { // in questi casi non c'è bisogno di fare nulla, basta seguire istruzione di do while
                }
                else if (uscita == 2)
                { // salvare dati
                    if (salvadatabase(num_spettacoli, Spettacoli, num_clienti, Clienti, Prenotazioni, Attese) == -1)
                    {
                        printf("Si è verificato un errore per salvataggio di database\n");
                        uscita = 3;
                    };
                }
                else
                { // in questo caso avvisare che scelta non vale
                    printf("scelta non valida\n");
                }
                if (uscita == 3)
                {
                    // se desidera ritornare, bisogna modificare primascelta da f a qualcos'altro.
                    primascelta = 'z';
                }
            } while (uscita != 1 && uscita != 2 && uscita != 3);
        }
    } while (primascelta != 'f' && primascelta != 'F');
    if (liberazione(Spettacoli, Clienti, Prenotazioni, Attese, generi) != 1)
    {
        printf("Si esce dal programma con liberazione insuccesso\n");
    };
}

// funzione di stampa menu secondaria per Gestione spettacoli
void menuspettacoli(Spettacolo *Spettacoli, Nodogenere **generi, int *ns, int *Maxspettacoli)
{
    int secondascelta; // dichiarare variabile per scelta, questa volta intero
    do
    {
        int num_spettacoli = *ns;
        // stampare le funzioni disponibili
        printf("Le funzionalità per gestione dei spettacoli sono : \n");
        printf("1. Inserisci nuovo spettacolo.\n");
        printf("2. VIsualizza tutti gli spettacoli.\n");
        printf("3. Cerca spettacolo per codice.\n");
        printf("4. Cerca spettacoli per genere.\n");
        printf("5. Spettacoli con posti disponibili.\n");
        printf("6. Esci dal menu secondario.\n");
        printf("Inserisci la tua scelta : ");
        scanf_sicuro(&secondascelta);
        switch (secondascelta) // switch case per scelte
        {
        case 1:
        {
            int flag;
            do
            {
                flag = inserimentospettacolo(Spettacoli, generi, ns, Maxspettacoli);
                printf("dati dello spettacolo salvato correttamente\n");
                // valutare genere inserito
                *generi = valutagenere(Spettacoli[*ns].genere, *generi);
                (*ns)++; // incrementare solo quando tutti i dati sono registrati correttamente in modo valido
                if (controllospazio(num_spettacoli, *Maxspettacoli) == 0)
                { // controlla spazio disponibile
                    printf("Spazio allocato per spattacoli ha raggiunto la sua massima capacità\n");
                    riallocazioneSp(&Spettacoli, *ns, Maxspettacoli);
                }
            } while (flag != 1);
        }
        break;
        case 2:
            visualizzaspettacoli(Spettacoli, num_spettacoli);
            break;
        case 3:
        {
            int codice = cercacodicespettacolo(Spettacoli, num_spettacoli);
            if (codice == -1)
            {
                printf("il codice %d non corrisponde ad alcun spettacolo presenti.\n", codice);
            }
        }
        break;
        case 4:
            if (cercagenerespettacolo(Spettacoli, num_spettacoli) == 0)
            { // se non è mai entrato in if, allora non ho trovato
                printf("Non ci sono spettacoli corrispondente al genere inserito.\n");
            }
            break;
        case 5:
            spettacolidisponibili(Spettacoli, num_spettacoli);
            break;
        case 6:
            break; // per uscita non si fa alcun azione e si esce dal ciclo while
        default:
            printf("scelta non valida.\n"); // avvisa la non validità della scelta e rientra in while
            break;
        }
    } while (secondascelta != 6); // entra in menu per defaul
}
// funzione di stampa menu secondaria per Gestione Clienti
void menuclienti(Cliente *Clienti, int *nc, int *Maxclienti)
{
    int secondascelta;
    do
    { // Analogamente si stampa funzioni dei clienti
        int num_clienti = *nc;
        printf("Le funzionalità per gestione dei clienti sono : \n");
        printf("1. Inserisci nuovo clientie.\n");
        printf("2. Visualizza tutti i clienti.\n");
        printf("3. Cerca cliente per codice.\n");
        printf("4. Esci dal menu secondario.\n");
        printf("Inserisci la tua scelta : ");
        scanf_sicuro(&secondascelta);
        switch (secondascelta)
        {
        case 1:
        {
            int flag;
            do
            {
                flag = inserimentoclienti(Clienti, nc, Maxclienti);
            } while (flag != 1);
            printf("dati dello cliente salvato correttamente\n");
            (*nc)++; // incrementare solo quando tutti i dati sono registrati correttamente in modo valido
            if (controllospazio(num_clienti, *Maxclienti) == 0)
            { // controlla spazio disponibile
                printf("Spazio allocato per clienti ha raggiunto la sua massima capacità\n");
                riallocazioneCl(&Clienti, *nc, Maxclienti);
            }
        }
        break;
        case 2:
            visualizzaclienti(Clienti, num_clienti);
            break;
        case 3:
        {
            int codice = cercacodicecliente(Clienti, num_clienti);
            if (codice == -1)
            {
                printf("il codice %d non corrisponde ad alcun cliente presenti.\n", codice);
            };
        }
        break;
        case 4:
            break;
        default:
            printf("scelta non valida.\n");
            break;
        }
    } while (secondascelta != 4);
}
// funzione di stampa menu secondaria per Gestione Prenotazioni
NodoPrenotazione *menuprenotazioni(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa **Attese, int num_spettacoli, int num_clienti)
{
    int secondascelta;
    do
    { // analogamente funzioni di prenotazioni
        printf("Le funzionalità per gestione dei prenotazioni sono : \n");
        printf("1. Registra nuova prenotazione.\n");
        printf("2. Annulla prenotazione.\n");
        printf("3. Visualizza tutte le prenotazioni.\n");
        printf("4. Visualizza prenotazioni di un cliente.\n");
        printf("5. Visualizza prenotazioni di uno spettacolo.\n");
        printf("6. Visualizza lista d'attesa.\n");
        printf("7. Esci dal menu secondario.\n");
        printf("Inserisci la tua scelta : ");
        scanf_sicuro(&secondascelta);
        switch (secondascelta)
        {
        case 1:
            Prenotazioni = registraprenotazione(Spettacoli, Clienti, Prenotazioni, Attese, num_spettacoli, num_clienti);
            break;
        case 2:
            Prenotazioni = annullaprenotazione(Spettacoli, Clienti, Prenotazioni, Attese, num_spettacoli, num_clienti);
            break;
        case 3:
            visualizzaprenotazioni(Prenotazioni);
            break;
        case 4:
            visualizzaprenotazionicliente(Prenotazioni);
            break;
        case 5:
            visualizzaprenotazionispettacolo(Prenotazioni);
            break;
        case 6:
            visualizzalistaattesa(*Attese);
            break;
        case 7:
            break;
        default:
            printf("scelta non valida.\n");
            break;
        }
    } while (secondascelta != 7);
    return Prenotazioni;
}
// funzione di stampa menu secondaria per Gestione Statistiche e Report
void menustat(int num_spettacoli, Spettacolo *Spettacoli, int num_clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese, Nodogenere *generi)
{
    int secondascelta;
    do
    { // analogamente funzioni di statistiche e report
        printf("Le funzionalità per statistiche e report sono : \n");
        printf("1. Statistiche generali.\n");
        printf("2. Spettacoli per genere.\n");
        printf("3. Top 5 spettacoli per incasso.\n");
        printf("4. Esci dal menu secondario.\n");
        printf("Inserisci la tua scelta : ");
        scanf_sicuro(&secondascelta);
        switch (secondascelta)
        {
        case 1:
            statistichegenerali(num_spettacoli, Spettacoli, num_clienti, Prenotazioni, Attese);
            break;
        case 2:
            stampaspettacoligenere(generi);
            break;
        case 3:
            stampatop(num_spettacoli, Spettacoli);
            break;
        case 4:
            break;
        default:
            printf("scelta non valida.\n");
            break;
        }
    } while (secondascelta != 4);
}
// funzione di stampa menu secondaria per Gestione Gestione file
void menufile(int *num_spettacoli, Spettacolo *Spettacoli, int *num_clienti, Cliente *Clienti, NodoPrenotazione **Prenotazioni, NodoAttesa **Attese, Nodogenere **generi, int *Maxspettacoli, int *Maxclienti)
{
    int secondascelta;
    do
    { // analogamente funzioni su file
        printf("Le funzionalità per gestione dei file sono : \n");
        printf("1. Salva database su file binario.\n");
        printf("2. Carica database da file binario.\n");
        printf("3. Esporta catalogo in formato testo.\n");
        printf("4. Esporta report prenotazioni in formato testo.\n");
        printf("5. Esci dal menu secondario.\n");
        printf("Inserisci la tua scelta : ");
        scanf_sicuro(&secondascelta);
        switch (secondascelta)
        {
        case 1:
            // funzione che salva dati sul file
            if (salvadatabase(*num_spettacoli, Spettacoli, *num_clienti, Clienti, *Prenotazioni, *Attese) == -1)
            {
                printf("Si è verificato un errore per salvataggio di database\n");
            };
            break;
        case 2:
        {
            // Siccome tutti i dati nella lettura vengono sovrascritti, si chiede di dare conferma
            int conferma = 0;
            printf("Caricando dati dal file, si perde tutti i dati presenti nell'array, sei sicura di continuare?\n");
            printf("Per conferma inserisci 1, altrimenti un qualsiasi altro numero intero : ");
            scanf_sicuro(&conferma);
            if (conferma != 1)
            { // uscire da questo caso switch
                break;
            }
            // funzione che carica dati dal file
            if (caricadatabase(num_spettacoli, &Spettacoli, num_clienti, &Clienti, Prenotazioni, Attese, generi, Maxspettacoli, Maxclienti) == -1)
            {
                printf("Si è verificato un errore per caricare dati dal file\n");
            };
        }
        break;
        case 3:
            // funzione che esporta catalogo in formato testo
            if (esportacatalogo(*num_spettacoli, Spettacoli) == -1)
            {
                printf("Si è verificato un errore per esportazione catalogo.txt\n");
            };
            break;
        case 4:
            // funzione che esporta report prenotazione
            if (esportaprenotazione(*Prenotazioni, Clienti, *num_clienti) == -1)
            {
                printf("Si è verificato un errore per esportazione prenotazioni\n");
            };
            break;
        case 5:
            break;
        default:
            printf("scelta non valida.\n");
            break;
        }
    } while (secondascelta != 5);
}

void stampaspettacolo(Spettacolo spettacolo)
{ // stampa dati
    printf("%-20d %-20s %-20s %-20s %17d/%-20d %20.2f\n", spettacolo.codice_spettacolo, spettacolo.titolo, spettacolo.genere, spettacolo.data_spettacolo, spettacolo.posti_disponibili, spettacolo.posti_totali, spettacolo.prezzo_biglietto);
    return;
}

void stampacliente(Cliente cliente)
{ // stampa dati
    printf("%-20d %-20s %-20s %-40s %-20s\n", cliente.codice_cliente, cliente.nome, cliente.cognome, cliente.email, cliente.data_iscrizione);
    return;
}

void visualizzaspettacoli(Spettacolo *Spettacoli, int num_spettacoli)
{
    printf("Elenco completo dei spettacoli presenti sono : \n");
    printf("%-20s %-20s %-20s %-20s %-41s %20s\n", "Codice", "Titolo", "Genere", "data", "posti disponibili/posti totali", "Prezzo biglietto");
    for (int i = 0; i < num_spettacoli; i++)
    { // usare ciclo per stampare  tutti i spettacoli presenti
        stampaspettacolo(Spettacoli[i]);
    }
    return;
}

void visualizzaclienti(Cliente *Clienti, int num_clienti)
{
    printf("Elenco completo dei clienti presenti sono : \n");
    printf("%-20s %-20s %-20s %-40s %-20s\n", "Codice", "Nome", "Cognome", "Email", "Data iscrizione");
    for (int i = 0; i < num_clienti; i++)
    { // usare ciclo per stampare  tutti i clienti presenti
        stampacliente(Clienti[i]);
    }
    return;
}

void stampaprenotazione(Prenotazione dati)
{
    printf("%-20d %-20d %-20d %-20d %-20.2f %-20s\n", dati.codice_prenotazione, dati.codice_spettacolo, dati.codice_cliente, dati.numero_posti, dati.importo_totale, dati.data_prenotazione);
    return;
}

void visualizzaprenotazioni(NodoPrenotazione *Prenotazioni)
{
    // inzializzare un puntatore per scorrimento
    NodoPrenotazione *p = Prenotazioni;
    if (p == NULL)
    { // gestire caso lista vuota
        printf("Non ci sono prenotazione registrai.\n");
        return;
    }
    // stampa tutte le prenotazioni presenti
    printf("Tutte le prenotazione registrate sono :\n");
    printf("%-20s %-20s %-20s %-20s %-20s %-20s\n", "codice prenotazione", "codice spettacolo", "codice cliente", "numero posti", "importo totale", "data prenotazione");
    while (p != NULL)
    { // scorrimento
        stampaprenotazione(p->dati);
        p = p->next;
    }
    return;
}

void visualizzaprenotazionicliente(NodoPrenotazione *Prenotazioni)
{ // scorrimento
    NodoPrenotazione *p = Prenotazioni;
    printf("Inserisci codice di cliente per visualizzare prenotazioni : ");
    int codice_cliente;
    scanf_sicuro(&codice_cliente);
    int presenza = 0;
    float sum = 0;
    while (p != NULL)
    {
        if (p->dati.codice_cliente == codice_cliente)
        { // stampare solo caso con stesso codice cliente
            if (presenza == 0)
            { // stampare solo per primo volta che si trova
                printf("Tutte le prenotazione registrate del utente con codice %d :\n", codice_cliente);
                printf("%-20s %-20s %-20s %-20s %-20s %-20s\n", "codice prenotazione", "codice spettacolo", "codice cliente", "numero posti", "importo totale", "data prenotazione");
                presenza = 1;
            }
            stampaprenotazione(p->dati);
            sum = sum + p->dati.importo_totale;
        }
        p = p->next;
    }
    if (presenza != 0)
    {
        printf("L'importo totale spese è : %.2f\n", sum);
    }
    else
    { // caso non ci sono prenotazioni corrispondenti
        printf("Non ci sono prenotazioni corrispondente\n");
    }
    return;
}

void visualizzaprenotazionispettacolo(NodoPrenotazione *Prenotazioni)
{
    // scorrimento
    printf("Inserisci il codice di spettacolo di ricerca : ");
    int codice_spettacolo;
    scanf_sicuro(&codice_spettacolo);
    NodoPrenotazione *p = Prenotazioni;
    int presenza = 0;
    float sum = 0;
    int posprenotati = 0;
    while (p != NULL)
    {
        if (p->dati.codice_spettacolo == codice_spettacolo)
        {
            if (presenza == 0)
            { // stampa solo per primo caso che si trova
                printf("Tutte le prenotazione registrate dello spettacolo con codice %d è :\n", codice_spettacolo);
                printf("%-20s %-20s %-20s %-20s %-20s %-20s\n", "codice prenotazione", "codice spettacolo", "codice cliente", "numero posti", "importo totale", "data prenotazione");
                presenza = 1;
            }
            stampaprenotazione(p->dati);
            sum = sum + p->dati.importo_totale;
            posprenotati = posprenotati + p->dati.numero_posti;
        }
        p = p->next;
    }
    if (presenza != 0)
    {
        printf("L'importo totale spese è : %.2f\n", sum);
        printf("Numero di posti prenotati sono : %d\n", posprenotati);
    }
    else
    { // caso senza prenotazione corrispondente
        printf("Non ci sono prenotazioni per spettacolo corrispondente\n");
    }
    return;
}

void visualizzalistaattesa(NodoAttesa *Attese)
{ // scorrimento
    NodoAttesa *p = Attese;
    if (p == NULL)
    { // caso lista vuota
        printf("Non ci sono attese presenti.\n");
        return;
    } // stampa
    printf("I dati di lista d'attesa è seguente : \n");
    printf("%-20s %-20s %-20s %-20s\n", "codice spettacolo", "codice_cliente", "numero posti", "data richiesta");
    while (p != NULL)
    {
        printf("%-20d %-20d %-20d %-20s\n", p->dati.codice_spettacolo, p->dati.codice_cliente, p->dati.numero_posti, p->dati.data_richiesta);
        p = p->next;
    }
    return;
}

// funzione che stampa statistica generale
void statistichegenerali(int num_spettacoli, Spettacolo *Spettacoli, int num_clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese)
{
    int posdisp = 0;                                                                           // inizializzare un contatore per posti disponibili
    int numprenotazioni = 0;                                                                   // inizializzare numero di prentazione richiamando funzione che conta prenotazioni
    int numattesa = contattesa(Attese);                                                        // inizializzazione numero di attese, anche in questo caso si richiama una funzione
    float incasso = 0;                                                                         // inizializzare un variabile per incasso totale
    float percoccup = 0;                                                                       // inzializzare un variabile per percentuali di occupazioni
    int indmax = 0;                                                                            // inizializzare una variabile per indice di spettacolo con incasso massimo
    int occupati_max = Spettacoli[indmax].posti_totali - Spettacoli[indmax].posti_disponibili; // inizializzare occupazione massimo
    for (int i = 0; i < num_spettacoli; i++)
    {
        // usare un ciclo per scorrere i dati dei spettacoli
        posdisp = posdisp + Spettacoli[i].posti_disponibili;                                  // incrementare posti disponibili
        int occupati_correnti = Spettacoli[i].posti_totali - Spettacoli[i].posti_disponibili; // inizializzare una variabile locale per ogni ciclo
        if (occupati_max < occupati_correnti)
        {
            // numero di posti prenotati è somma - rimasto
            indmax = i;
            occupati_max = occupati_correnti;
        }
        // sommare percentuali di occupazione di singolo spettacolo
        percoccup = percoccup + occupati_correnti * 100.0 / Spettacoli[i].posti_totali;
    }
    NodoPrenotazione *p = Prenotazioni;
    while (p != NULL)
    {
        // scorrere lista di prenotazione per sommare incasso di ogni prenotazione
        // incrementa anche numero di prenotazione come contatore
        incasso = incasso + p->dati.importo_totale;
        numprenotazioni++;
        p = p->next;
    }
    // stampa i risultati
    printf("In cartellone contiene %d spettacoli\n", num_spettacoli);
    printf("In tutto rimane ancora %d posti disponibili\n", posdisp);
    printf("In cartellone contiene %d clienti registrati\n", num_clienti);
    printf("In cartellone contiene %d prenotazione attiva\n", numprenotazioni);
    printf("In cartellone contiene %d richieste in attesa\n", numattesa);
    printf("Incasso totale è : %.2f \n", incasso);
    printf("Lo spettacolo %s ha più posti prenotati\n", Spettacoli[indmax].titolo);
    if (num_spettacoli != 0)
    {
        // distinguere due casi per evitare divisione per zero
        percoccup = percoccup / num_spettacoli; // dividendo al numero di spettacoli si trova media di occupazione in percentuale
        printf("La media di occupazione è : %.f %%\n", percoccup);
    }
    else
    {
        printf("Non ci sono spettacoli presenti\n");
    }
    return;
}

// funzione che stampa numero di spettacoli per genere
void stampaspettacoligenere(Nodogenere *generi)
{
    if (generi == NULL)
    {
        // gestire caso lista vuota
        printf("non ci sono spettacoli presenti\n");
        return;
    }
    // stampa dati scorrendo lista
    printf("Numero di spettacoli presenti divise per genere sono :\n");
    Nodogenere *p = generi;
    while (p != NULL)
    {
        printf("%-15s : %-3d spettacoli\n", p->genere, p->numero);
        p = p->next;
    }
    return;
}

// funzione che stampa Top 5 spettacoli per incasso
void stampatop(int num_spettacoli, Spettacolo *Spettacoli)
{
    if (num_spettacoli == 0)
    {
        // distinguere caso senza spettacoli
        printf("Non ci sono spettacolipresenti\n");
    }
    else
    {
        // allocazione dinamica per incassi e indici
        float *incassi = (float *)malloc(num_spettacoli * sizeof(float));
        if (incassi == NULL)
        { // controllo
            printf("Errore allocazione dinamica\n");
            exit(1);
        }
        // dichiara un array di indici corrispondenti ad ogni spettacolo, evitando fare operazioni su Array di spettacolo
        int *indici = (int *)malloc(num_spettacoli * sizeof(int));
        if (indici == NULL)
        { // controllo
            printf("Errore allocazione dinamica\n");
            exit(1);
        }
        // usare un ciclo per assegnare valori in indici
        for (int i = 0; i < num_spettacoli; i++)
        {
            indici[i] = i;
            incassi[i] = (Spettacoli[i].posti_totali - Spettacoli[i].posti_disponibili) * Spettacoli[i].prezzo_biglietto;
        }
        // dichiarare variabili per scambio
        float tempincassi;
        int tempindici;
        // bubble sort
        for (int i = 0; i < 5; i++)
        { // bastano primi 5
            for (int j = 0; j < num_spettacoli - 1 - i; j++)
            {
                if (incassi[j] < incassi[j + 1])
                { // in modo decrescente
                    // scambio quantità di incasso
                    tempincassi = incassi[j];
                    incassi[j] = incassi[j + 1];
                    incassi[j + 1] = tempincassi;
                    // scambio contemporaneamente indici corrispondenti
                    tempindici = indici[j];
                    indici[j] = indici[j + 1];
                    indici[j + 1] = tempindici;
                }
            }
        }
        if (num_spettacoli <= 5)
        {
            // avvisare ne sia nel caso minore di 5
            printf("Gli spettacoli sono minori o uguali di 5 e sono :\n");
        }
        else
        {
            // si distingue solo per messaggio di inizio
            printf("Gli top 5 spettacoli con maggiore incasso sono :\n");
        }
        printf("%-10s %-20s %-20s %20s\n", "Posizione", "Titolo", "Genere", "Incasso");
        for (int i = 0; i < 5; i++)
        { // stampa 5 volte
            // modo tabulato
            printf("%-10d %-20s %-20s %20.2f\n", indici[i], Spettacoli[indici[i]].titolo, Spettacoli[indici[i]].genere, incassi[i]);
        }
        // liberazione array dinamici
        free(incassi);
        free(indici);
    }
    return;
}

int salvadatabase(int num_spettacoli, Spettacolo *Spettacoli, int num_clienti, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese)
{
    // Apertura file binario in scrittura
    FILE *fp = fopen("spettacoli.dat", "wb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file spettacoli.dat\n");
        return -1;
    } // controllo
    size_t flag; // dichiarare indicatore per corettezza di scrittura
    if (num_spettacoli == 0)
    { // evita azione per array vuoto
        printf("Non ci sono spettacoli presenti, file spettacoli.dat salvato come vuoto\n");
        fclose(fp);
    }
    else
    {
        flag = fwrite(&num_spettacoli, sizeof(int), 1, fp); // scrive prima numero di spettacolio
        if (controllaoperazione(flag, 1, fp) != 1)
        {
            printf("Errore scrittura file spettacoli.dat\n");
            return -1;
        } // controllo
        flag = fwrite(Spettacoli, sizeof(Spettacolo), num_spettacoli, fp); // poi scrivere dati presenti
        if (controllaoperazione(flag, num_spettacoli, fp) != 1)
        {
            printf("Errore scrittura file spettacoli.dat");
            return -1;
        } // controllo
        fclose(fp);                                                                   // chiusura file
        printf("Dati di spettacoli salvati correttamente nel file spettacoli.dat\n"); // stampa risultato
    }
    // Analogo a spettacoli
    fp = fopen("clienti.dat", "wb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file clienti.dat\n");
        return -1;
    } // controllo
    if (num_clienti == 0)
    {
        printf("Non ci sono clienti presenti, file clienti.dat salvato come vuoto\n");
        fclose(fp);
    }
    else
    {
        flag = fwrite(&num_clienti, sizeof(int), 1, fp);
        if (controllaoperazione(flag, 1, fp) != 1)
        {
            printf("Errore scrittura file clienti.dat");
            return -1;
        }
        flag = fwrite(Clienti, sizeof(Cliente), num_clienti, fp);
        if (controllaoperazione(flag, num_clienti, fp) != 1)
        {
            printf("Errore scrittura file clienti.dat\n");
            return -1;
        }
        fclose(fp);
        printf("Dati di Clienti salvati correttamente nel file clienti.dat\n");
    }
    // Apertura file per Prenotazioni in scrittura binario
    fp = fopen("prenotazioni.dat", "wb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file prenotazioni.dat\n");
        return -1;
    } // controllo            // controllo apertura
    NodoPrenotazione *p = Prenotazioni; // inzializzare puntatore per scorrimento
    if (p == NULL)
    { // caso lista vuota
        fclose(fp);
        printf("Lista vuota, file creato in modo vuoto\n");
    }
    else
    {
        while (p != NULL)
        {
            flag = fwrite(&(p->dati), sizeof(Prenotazione), 1, fp); // scrive solo dati del nodo, una per volta
            if (controllaoperazione(flag, 1, fp) != 1)
            {
                printf("Errore scrittura file prenotazioni.dat\n");
                return -1;
            } // controllo
            p = p->next; // scorrimento
        }
        printf("Dati nella lista di prenotazioni salvati correttamente nel file prenotazioni.dat\n"); // stampa risultato
        fclose(fp);                                                                                   // in qualsiasi caso, chiude file
    }
    // Analogo a Prenotazioni
    fp = fopen("attesa.dat", "wb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file attesa.dat\n");
        return -1;
    } // controllo
    NodoAttesa *q = Attese;
    if (q == NULL)
    {
        fclose(fp);
        printf("Lista vuota, file creato in modo vuoto\n");
    }
    else
    {
        while (q != NULL)
        {
            flag = fwrite(&(q->dati), sizeof(RichiestaAttesa), 1, fp);
            if (controllaoperazione(flag, 1, fp) != 1)
            {
                printf("Errore scrittura file attesa.dat\n");
                return -1;
            };
            q = q->next;
        }
        printf("Dati nella lista di attesa salvati correttamente nel file attesa.dat\n");
        fclose(fp);
    }
    return 1;
}

int caricadatabase(int *num_spettacoli, Spettacolo **Spettacoli, int *num_clienti, Cliente **Clienti, NodoPrenotazione **Prenotazioni, NodoAttesa **Attese, Nodogenere **generi, int *Maxspettacoli, int *Maxclienti)
{
    // apertura file
    FILE *fp = fopen("spettacoli.dat", "rb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file spettacoli.dat\n");
        return -1;
    } // controllo
    int temp_ns;                                       // controllo
    size_t flag = fread(&temp_ns, sizeof(int), 1, fp); // leggere numero di spettacoli
    if (controllaoperazione(flag, 1, fp) != 1)
    {
        printf("Errore lettura file spettacoli.dat\n");
        return -1;
    } // controlla lettura
    *num_spettacoli = temp_ns;  //intanto si assegna numero di spettacoli
    // vari casi per disponibilità dello spazio allocato
    if (controllospazio(*num_spettacoli, *Maxspettacoli) == -1)
    {
        printf("Non ci sono spazi disponibili per inserimento\n");
        int flagra = riallocazioneSp(Spettacoli, *num_spettacoli, Maxspettacoli);
        // vari casi di riallocazione
        if (flagra == 1)
        {
            printf("Riallocazione con successo, adesso abbiamo nuova capacità %d\n", *Maxspettacoli);
        }
        else
        {
            printf("Riallocazione ha incontrato qualche problema, riprova più tardi\n");
            return -1;
        }
    } // controllo spazio siponibile
    else if (controllospazio(*num_spettacoli, *Maxspettacoli) == 0)
    { // controlla spazio disponibile secondo caso
        printf("Spazio allocato per spattacoli raggiungerà la sua massima capacità\n");
        riallocazioneSp(Spettacoli, *num_spettacoli, Maxspettacoli); // riallocare se è neccessario
    };
    // Liberare tutte le memorie se si riesce ad aprire il file e leggere il numero di spettacoli
    *num_clienti = 0;
    liberaprenotazioni(*Prenotazioni);
    *Prenotazioni = NULL;
    liberaattesa(*Attese);
    *Attese = NULL;
    liberageneri(*generi);
    *generi = NULL;
    // controllato che spazio siano sufficienti si legge dati dal file
    flag = fread(*Spettacoli, sizeof(Spettacolo), *num_spettacoli, fp);
    if (controllaoperazione(flag, *num_spettacoli, fp) != 1)
    {
        printf("Errore lettura file spettacoli.dat\n");
        return -1;
    }; // controllo lettura
    fclose(fp); // chiusura file
    for (int i = 0; i < *num_spettacoli; i++)
    { // ciclo per generare lista di generi corrispondenti a nuovi spettacoli
        *generi = valutagenere((*Spettacoli)[i].genere, *generi);
    }
    printf("File spettacoli.dat caricato correttamente\n");
    // Analogamente a Spettacoli
    fp = fopen("clienti.dat", "rb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file clienti.dat\n");
        return -1;
    } // controllo
    flag = fread(num_clienti, sizeof(int), 1, fp);
    if (controllaoperazione(flag, 1, fp) != 1)
    {
        printf("Errore lettura file clienti.dat\n");
        return -1;
    }
    if (controllospazio(*num_clienti, *Maxclienti) == -1)
    {
        printf("Non ci sono spazi disponibili per inserimento\n");
        int flagra = riallocazioneCl(Clienti, *num_clienti, Maxclienti);
        // vari casi di riallocazione
        if (flagra == 1)
        {
            printf("Riallocazione con successo, adesso abbiamo nuova capacità %d\n", Maxspettacoli);
        }
        else // Non si può leggere se non ha spazio disponibile
        {
            printf("Riallocazione ha incontrato qualche problema, riprova più tardi\n");
            return -1;
        }
    } // controllo spazio siponibile
    else if (controllospazio(*num_clienti, *Maxclienti) == 0)
    { // controlla spazio disponibile secondo caso
        printf("Spazio allocato per clienti raggiungerà la sua massima capacità\n");
        riallocazioneCl(Clienti, *num_clienti, Maxclienti);
    };
    // dopo aver controllato che spazio siano sufficienti leggere dati dal file
    flag = fread(*Clienti, sizeof(Cliente), *num_clienti, fp);
    if (controllaoperazione(flag, *num_clienti, fp) != 1)
    {
        printf("Errore lettura file clienti.dat\n");
        return -1;
    }; // controlla lettura dati
    fclose(fp); // chiusura file
    // stampa risultato
    printf("File clienti.dat caricato correttamente\n");
    // Analoga a prima
    fp = fopen("prenotazioni.dat", "rb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file prenotazioni.dat\n");
        return -1;
    } // controllo
    Prenotazione tempprenotazione; // dichiarare una variabile per appoggiare temporaneramente dati letti dal file
    NodoPrenotazione *p = *Prenotazioni;
    while (fread(&tempprenotazione, sizeof(Prenotazione), 1, fp) == 1) // finché si legge
    {                                                                  // legge un dato di un nodo
        NodoPrenotazione *nuovo_nodo = creaprenotazione(tempprenotazione);
        if (*Prenotazioni == NULL)
        { // caso lista vuota
            *Prenotazioni = nuovo_nodo;
            p = nuovo_nodo;
        }
        else
        { // aagiungere nuovo nodo in coda senza usare funzione di inserimento in coda, perché conosco sia primo che ultimo
            p->next = nuovo_nodo;
            p = nuovo_nodo;
        } // inserire nella lista come nuovo nodo
    }
    fclose(fp); // chiusura alla fine
    printf("File prenotazioni.dat caricato correttamente\n");
    // Analogamente a prenotazioni
    fp = fopen("attesa.dat", "rb");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file attesa.dat\n");
        return -1;
    } // controllo
    RichiestaAttesa tempattesa;
    NodoAttesa *q = *Attese;
    while (fread(&tempattesa, sizeof(RichiestaAttesa), 1, fp) == 1)
    {
        NodoAttesa *nuova_attesa = creaattesa(tempattesa);
        if (*Attese == NULL)
        {
            *Attese = nuova_attesa;
            q = nuova_attesa;
        }
        else
        {
            q->next = nuova_attesa;
            q = nuova_attesa;
        }
    }
    fclose(fp);
    printf("File attesa.dat caricato correttamente\n");
    return 1;
}

// funzione che esporta catalogo in formato testo
int esportacatalogo(int num_spettacoli, Spettacolo *Spettacoli)
{
    FILE *fp = fopen("catalogo.txt", "w"); // apertura file txt in scrittura
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file catalogo.txt\n");
        return -1;
    } // controllo            // controllo
    for (int i = 0; i < num_spettacoli; i++)
    { // usare un ciclo per scrivere dati per riga
        fprintf(fp, "Codice: %-10d | Titolo: %-20s | Genere: %-20s | Data: %-20s | Posti %17d/%-20d | Prezzo: %-20.2f\n", Spettacoli[i].codice_spettacolo, Spettacoli[i].titolo, Spettacoli[i].genere, Spettacoli[i].data_spettacolo, Spettacoli[i].posti_disponibili, Spettacoli[i].posti_totali, Spettacoli[i].prezzo_biglietto);
    }
    fclose(fp);                                                            // chiusura file alla fine
    printf("Dati dei spettacoli scritti correttamente in catalogo.txt\n"); // stampa risultato di scrittura
    return 1;
}

int esportaprenotazione(NodoPrenotazione *Prenotazioni, Cliente *Clienti, int num_clienti)
{
    // apertura file
    FILE *fp = fopen("report_prenotazioni.txt", "w");
    if (controllaapertura(fp) != 1)
    {
        printf("Errore apertura file report_prenotazioni.txt\n");
        return -1;
    } // controllo
    NodoPrenotazione *p = Prenotazioni; // Inizializzare variabile per scorrimento
    if (p == NULL)
    {
        printf("Non ci sono prenotazioni presenti, file salvato come vuoto\n");
        fclose(fp);
        return 1;
    }
    int cont = 0;      // Inizializzare contatore per numero di prenotazione
    float incasso = 0; // Inizializzare incasso totale
    int i;             // dichiarare indice per cliente corrispondente
    while (p != NULL)
    {
        cont++;                                                              // incremento contatore
        incasso = incasso + p->dati.importo_totale;                          // aumentare incasso corrispondente
        i = controllacodiceCl(p->dati.codice_cliente, Clienti, num_clienti); // trovare indice di cliente corrispondente
        // stampa i dati sul file
        fprintf(fp, "Codice prenotazione : %-20d | Codice spettacolo : %-20d | Codice Cliente : %-20d Nome : %-20s Cognome : %-20s | Numero posti : %-20d | Importo totale %-20.2f | Data prenotazione : %-20s\n", p->dati.codice_prenotazione, p->dati.codice_spettacolo, p->dati.codice_cliente, Clienti[i].nome, Clienti[i].cognome, p->dati.numero_posti, p->dati.importo_totale, p->dati.data_prenotazione);
        p = p->next; // scorre
    }
    // stampa dati finale
    fprintf(fp, "In tutto contiene un %d prenotazioni con l'incasso totale %-10.2f\n", cont, incasso);
    fclose(fp);                                                                          // chiusura file
    printf("Dati delle prenotazione scritte correttamene in report_prenotazioni.txt\n"); // stampa risultato
    return 1;
}

int cercacodicespettacolo(Spettacolo *Spettacoli, int num_spettacoli)
{
    int codice; // dichiarare codice di ricerca
    printf("Insersci il codice dello spettacolo da cercare : ");
    scanf_sicuro(&codice);
    for (int i = 0; i < num_spettacoli; i++)
    {
        if (Spettacoli[i].codice_spettacolo == codice) // fare confronto con tutti precedenti
        {
            printf("Lo spettacolo corrispondente al codice %d è : \n", codice);
            printf("%-20s %-20s %-20s %-20s %-40s %-20s\n", "Codice", "Titolo", "Genere", "data", "posti disponibili/posti totali", "Prezzo biglietto");
            stampaspettacolo(Spettacoli[i]);
            return codice; // basta un caso per uscire, codice è univoco
        }
    }
    // siccome non ha uscito quando ha trovato qualche spettacolo corrispondente, si avvisa l'errore
    return -1;
}

int cercacodicecliente(Cliente *Clienti, int num_clienti)
{
    int codice; // dichiarare codice di ricerca
    printf("Insersci il codice dello cliente da cercare : ");
    scanf_sicuro(&codice);
    for (int i = 0; i < num_clienti; i++)
    {
        if (Clienti[i].codice_cliente == codice) // fare confronto con tutti precedenti
        {
            printf("Lo cliente corrispondente al codice %d è : \n", codice);
            printf("%-20s %-20s %-20s %-40s %-20s\n", "Codice", "Nome", "Cognome", "Email", "Data iscrizione");
            stampacliente(Clienti[i]);
            return codice; // basta un caso per uscire, codice è univoco
        }
    }
    // siccome non ha uscito quando ha trovato qualche cliente corrispondente, si avvisa l'errore
    return -1;
}

int cercagenerespettacolo(Spettacolo *Spettacoli, int num_spettacoli)
{
    char genere[31]; // dichiarare una variabile per genere
    int flag = 0;    // indicatore per aver trovato o no
    printf("Inserisci il genere degli spettacoli da cercare(attenzione a lettera maiuscola) : ");
    fgets(genere, 30, stdin);
    genere[strcspn(genere, "\n")] = '\0';
    pulizia(); // pulizia di buffer
    for (int i = 0; i < num_spettacoli; i++)
    {
        if (strcmp(Spettacoli[i].genere, genere) == 0)
        { // fare comparison tra tutti vecchi, per richiesta si usa strcmp, altrimenti userò strcasecmp che evita errori per casesentity
            if (flag == 0)
            {             // solo per prima volta che si trova una di genere corrispondente
                flag = 1; // segna di aver trovato per evitare stampo ripetuto
                printf("I spettacoli corrispondenta a genere %s sono :\n", genere);
                printf("%-20s %-20s %-20s %-20s %-40s %-20s\n", "Codice", "Titolo", "Genere", "data", "posti disponibili/posti totali", "Prezzo biglietto");
            }
            // richiama funzione di stampa
            stampaspettacolo(Spettacoli[i]);
        }
    }
    return flag;
}

void spettacolidisponibili(Spettacolo *Spettacoli, int num_spettacoli)
{
    int *indici = (int *)malloc(num_spettacoli * (sizeof(int))); // alloco dinamicamente un array di indici per spettacoli con posti disponibili
    int ind = 0;                                                 // inzializzo un contatore per questi spettacoli
    for (int i = 0; i < num_spettacoli; i++)
    {
        if (Spettacoli[i].posti_disponibili > 0)
        {                    // per ogni spettacoli disponibili
            indici[ind] = i; // segno l'indice corrispondente
            ind++;           // incremento contatore
        }
    }
    int temp; // dichiara variabile temporanea per scambio
    for (int i = 0; i < ind - 1; i++)
    {
        for (int j = 0; j < ind - 1 - i; j++)
        { // bubble sort
            if (trasformadata(Spettacoli[indici[j]]) > trasformadata(Spettacoli[indici[j + 1]]))
            {
                temp = indici[j];
                indici[j] = indici[j + 1];
                indici[j + 1] = temp;
            }
        }
    }
    // stampa risultati
    printf("Gli spettacoli con ancora posti disponibili, ordinati in data crescente sono : \n");
    printf("%-20s %-20s %-20s %-20s %-40s %-20s\n", "Codice", "Titolo", "Genere", "data", "posti disponibili/posti totali", "Prezzo biglietto");
    for (int i = 0; i < ind; i++)
    {
        stampaspettacolo(Spettacoli[indici[i]]);
    }
    free(indici);
    return;
}

// funzione che assicura di leggere un intero per scanf
int scanf_sicuro(int *indirizzo)
{
    int risultato;
    // continua a chiedere di inserire un numero intero
    while ((risultato = scanf("%d", indirizzo)) != 1)
    {
        if (risultato == EOF)
        {
            exit(1);
        }
        printf("Inserisci un numero intero. Riprova : ");
        pulizia();
    }
    pulizia();
    return 1;
}

Nodogenere *valutagenere(char genere[], Nodogenere *generi)
{
    Nodogenere *p = generi;
    if (p == NULL)
    { // caso lista vuoto
        generi = inserimentocodagenere(genere, generi);
        return generi;
    }
    else
    {
        while (p != NULL && strcasecmp(p->genere, genere) != 0)
        { // scorre fino a fine o trova stesso genere
            p = p->next;
        }
        if (p == NULL)
        { // se non ho trovato, inserisco in coda nella lista
            generi = inserimentocodagenere(genere, generi);
            return generi;
        }
        else
        { // se ho trovato, incrementa numero del generi
            p->numero++;
        }
    }
    return generi;
}

// funzione che conta numero di nodi presenti nella lista di tipo Nodoattesa
int contattesa(NodoAttesa *Attese)
{
    // Scorrtimento
    NodoAttesa *p = Attese;
    int cont = 0; // Inizializzare un contatore
    while (p != NULL)
    {
        p = p->next;
        cont++; // Incrementa contatore
    }
    return cont;
}

int generacodice(NodoPrenotazione *Prenotazioni)
{ // restituisce sempre il codice del massimo nella lista +1
    // inzializzare una codice
    int codice = 0;
    if (Prenotazioni == NULL)
    {
        return codice;
    }
    // scorrimento fino al ultimo
    NodoPrenotazione *p = Prenotazioni;
    while (p->next != NULL)
    {
        p = p->next;
    }
    // siccome codice è generato in ordine crescente, si trova il massimo sempre all'ultimo
    // basta aumenta di 1 per codice nuovo
    codice = p->dati.codice_prenotazione + 1;
    return codice;
}

int scanstring(char data[], int lung)
{
    int risultato = 0;                // inizializzare un risultato in int della data
    char numeri[11] = {"0123456789"}; // inzializzare una stringa con numeri 0-9 dove posizione i corrisponde a i
    for (int j = 0; j < lung; j++)
    { // considero ogni char nella posizione j fino ad lunghezza di char
        for (int i = 0; i < 10; i++)
        {                // confrontare con ogni numero naturale da 0 a 9
            int esp = 1; // inizializzare un esponenziale per posizione della cifra
            for (int k = 0; k < lung - j - 1; k++)
            {
                esp = esp * 10; // trasformare in esponente di 10, per mettere cifra i nella posizione giusta
            }
            if (data[j] == numeri[i])
            {
                risultato = risultato + esp * i; // porre cifra i nella posizione j del risultato
            }
        }
    }
    return risultato; // ritornare cifra di tipo int
}

int trasformadata(Spettacolo spettacolo)
{
    // estraggo dati dalla data
    char giornot[3] = {spettacolo.data_spettacolo[0], spettacolo.data_spettacolo[1]};
    char meset[3] = {spettacolo.data_spettacolo[3], spettacolo.data_spettacolo[4]};
    char annot[5] = {spettacolo.data_spettacolo[6], spettacolo.data_spettacolo[7], spettacolo.data_spettacolo[8], spettacolo.data_spettacolo[9]};
    // inizializzare variabili richiamando funzione di scansione
    int giorno = scanstring(giornot, 2);
    int mese = scanstring(meset, 2);
    int anno = scanstring(annot, 4);
    // trasformo di data di forma aaaammgg
    int data = anno * 10000 + mese * 100 + giorno;
    return data;
}

void pulizia()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // pulire tutti le input rimasti nel terminal
    return;
}

// funzione di liberazione memoria allocata
int liberazione(Spettacolo *Spettacoli, Cliente *Clienti, NodoPrenotazione *Prenotazioni, NodoAttesa *Attese, Nodogenere *generi)
{
    // liberazione allocazione dinamica
    free(Spettacoli);
    free(Clienti);
    // liberazione liste con i nodi
    if (liberaprenotazioni(Prenotazioni) == 1)
    {
        printf("Lista di prenotazioni liberata correttamente\n");
    }
    else
    {
        printf("Errore liberazione lista di prenotazioni\n");
        return -1;
    }
    if (liberaattesa(Attese) == 1)
    {
        printf("Lista di attesa liberata correttamente\n");
    }
    else
    {
        printf("Errore liberazione lista di attesa\n");
        return -1;
    }
    if (liberageneri(generi) == 1)
    {
        printf("Lista di generi liberata correttamente\n");
    }
    else
    {
        printf("Errore liberazione lista di generi\n");
        return -1;
    }
    return 1;
}