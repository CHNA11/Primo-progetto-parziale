#include <stdio.h>
#include <stdlib.h> //introduco perché almeno mi serve per allocazione dinamica
#include <string.h> //introdotto per operazione sulle stringhe
#include <ctype.h>

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
// funzione di riallocazione per sapzio non sufficiente
int riallocazione(Spettacolo *Spettacoli);

// funzioni di controllo per validità dei dati

// funzione che controlla codice sia univoco
int controllacodice(int codice, Spettacolo* Spettacoli);
// funzione che controlla data sia valida
int controlladata(char *data);
// funzione che controlla numero di spettacoli non supera limite
int controllonumerospettacolo();

// funzioni di inserimento

// funzione di inserimento per dati di uno spettacolo
void inserimentospettacolo(Spettacolo *Spettacoli);

// funzioni di stampa

// funzione di stampa menu e scelte temporanea per test
void menu();
// funzione di stampa dati di uno spettacolo
void stampaspettacolo(Spettacolo spettacolo);

// variabile globale per controllo di dimensione, infatti deve essere ripresa in modo comodo da altri funzioni

int Maxspettacoli;      // dichiarare variabile per allocazione dinamica dei spettacoli
int Maxclienti;         // dichiarare variabile per allocazione dinamica dei clienti
int num_spettacoli = 0; // numero di spettacoli presenti nello spazio

int main()
{
    avvio(); //richiama funzione di avvio per chiedere password in entrata
    menu();
}

void avvio()
{
    int password; //dichiarare una variabile per salvare password
    size_t flag = 1; //inzializzare una variabile per controllare operazione sul file
    FILE *fp = fopen("password.dat", "rb"); //apertura file di password salvato binario
    if (fp == NULL)
    { //se non si trova questo file setta password come default 123456
        password = 123456;
    }
    else
    {
        flag = fread(&password, sizeof(int), 1, fp); //controllare lettura della file
        if (flag != 1)
        { //controllo
            printf("problemi sulla gestione della password, eliminare password.dat dalla cartella\n");
            exit(1);
        }
        password = password - 123456; //decodificazione della password, corrispondente al passo di cambiamento password con codificazione
        fclose(fp);
    }

    int passwordinput; //dichiarare una variabile per password inserito dal utente
    int riprova = 0; //indicatore di riprova
    int correttezza = 0; //indicatore di corettezza
    do
    {
        printf("inserisci la password per entrare in accesso al sistema : "); //chiede di inserire password
        scanf("%d", &passwordinput);
        if (passwordinput == password)
        {
            correttezza = 1;
        }
        else
        { //si considera caso si sbaglio e non vuole più provare
            printf("password non corrispondente, per riprovare inserisci 1, altrimenti qualsiasi numero : ");
            scanf("%d", &riprova);
        }
    } while (correttezza == 0 && riprova == 1);
    if (correttezza == 0)
    { //se non riesce ad inserire password giusto si esce dal sistema
        exit(0);
    }
    int scelta = 0;
    do
    { //si dia pure possibilità di cambiare password
        printf("Le possibile funzioni sono : \n");
        printf("1. Cambiare password.\n");
        printf("2. Entrare in menu.\n");
        printf("3. Uscire dal sistema.\n");
        printf("Inserisci la tua scelta : ");
        scanf("%d", &scelta);
        if (scelta == 1)
        {
            int nuovapassword;
            printf("Inserisci la nuova password : ");
            scanf("%d", &nuovapassword);
            if (nuovapassword == password)
            { //evita di inserire vecchi password
                printf("la nuova password deve essere diversa da quella vecchia\n");
            }
            else
            {
                nuovapassword = nuovapassword + 123456; //codificazione password
                FILE *fp = fopen("password.dat", "wb"); //apertura file con password
                flag = fwrite(&nuovapassword, sizeof(int), 1, fp); //sovrascrivere password
                if (flag != 1)
                { //controlla
                    printf("Errore nel salvare nuova passoword\n");
                    fclose(fp);
                }
                else
                { //risultato
                    printf("Nuova password salvato con successo\n");
                    fclose(fp); //chiusura
                }
            }
        }
        else if (scelta == 2)
        { //per entrare in sistema basta non uscire con exit una volta inserito password giusto
            printf("Avvio con successo, benvenuto al sistema di manager per teatro!\n");
        }
        else if (scelta == 3)
        {
            exit(0);
        }
        else
        { //caso rimasto
            printf("scelta non valida, si ritorna al menu principale\n");
        }
    } while (scelta != 2);
}

int richiestaMaxspettacoli()
{
    do
    { // chiedere in input grandezza per allocazione
        printf("inserisci numero massimo di spettacoli da gestire : ");
        scanf("%d", &Maxspettacoli);
        if (Maxspettacoli < 1)
        { // informare all'utente per input non valido
            printf("il numero richiesto deve essere un intero positivo non nullo\n");
        }
    } while (Maxspettacoli < 1); // non uscire dal ciclo finché non ha un valore legale
    return Maxspettacoli;
}

int richiestaMaxclienti()
{
    do
    { //chiede in input grandezza per allocare
        printf("inserisci numero massimo di clienti da gestire : ");
        scanf("%d", &Maxclienti);
        if (Maxclienti < 1) //controlla che sia un input valido
        {
            printf("il numero richiesto deve essere un intero positivo non nullo\n");
        }
    } while (Maxclienti < 1); //uscire solo quando numero è valido
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
    { //controllo allocazione
        printf("Allocazione per Clienti insuccesso\n");
        exit(1);
    }
    return Clienti;
}

int riallocazione(Spettacolo *Spettacoli)
{
    int flagra;
    printf("Se desideri riallocare spazio con spazio maggiore automaticamente inserisci 1.\n");
    printf("Se desidere riallocare con uno spazio specifico desiderato inserisci 2.\n");
    printf("Se desideri lasciare array come adesso inserisci qualsiasi altro numero.\n");
    printf("Inserisci la tua scelta : ");
    scanf("%d", &flagra); //leggere scelta per riallocazione
    if (flagra == 1)
    {
        while (num_spettacoli > Maxspettacoli)
        {
            Maxspettacoli = Maxspettacoli * 2; //in automatico si raddopia dimensione finché sia sufficiente
        }
        Spettacolo *temp = (Spettacolo *)realloc(Spettacoli, Maxspettacoli * sizeof(Spettacolo)); //riallocazione con temp per evitare perdita di dati
        if (temp == NULL)
        { //infatti se uso direttamente Spettacoli si perde nel caso NULL
            printf("riallocazione insuccesso\n");
            return -1;
        }
        Spettacoli = temp; //caso successo, assegnare questo nuvo spazio
        return 1;
    }
    else if (flagra == 2)
    {
        do
        {
            printf("Nello spazio contiene %d spettacoli, inserisci nuova capacità per spettacoli : ", num_spettacoli);
            scanf("%d", &Maxspettacoli); //chiedere nuovospazio manualmente
            if (num_spettacoli > Maxspettacoli)
            { //ma deve essere comunque sufficiente
                printf("Si chiede di inserire una capacità sufficiente per contenere spettacoli\n");
            }
        } while (num_spettacoli > Maxclienti);
        Spettacolo *temp = (Spettacolo *)realloc(Spettacoli, Maxspettacoli * sizeof(Spettacolo));
        if (temp == NULL)
        { //come caso automatico
            printf("riallocazione insuccesso\n");
            return -1;
        }
        Spettacoli = temp;
        return 1;
    }
    else
    {
        return 0;
    }
}

// funzione che controlla codice sia univoco
int controllacodice(int codice, Spettacolo* Spettacoli){
    for(int i=0; i<num_spettacoli; i++){
        //scorre tutti i spettacoli presenti e confronta
        if(Spettacoli[i].codice_spettacolo==codice){
            return -1;
        }
    }
    return 1;
}

// funzione che controlla data sia valida
int controlladata(char data[]){
    //controlo il presenza di / nel formato richiesto
    if(data[2] != '/' || data[5] != '/'){
        printf("Inserisci una data con formato gg/mm/aaaa\n");
        return -1;
    }
    //definisco una stringa con numeri tra 0 e 9
    char numeri[]="0123456789"; 
    int flaggiornouno=0; //indicatore per vedere se prima cifra del giorno sia un numero
    int flaggiornodue=0; //indicatore per vedere se seconda cifra del giorno sia un numero
    int flagmeseuno=0; //indicatore per vedere se prima cifra del mese sia un numero
    int flagmesedue=0; //indicatore per vedere se seconda cifra del mese sia un numero
    int flagannouno=0; //indicatore per vedere se prima cifra del anno sia un numero
    int flagannodue=0; //indicatore per vedere se seconda cifra del anno sia un numero
    int flagannotre=0; //indicatore per vedere se terza cifra del anno sia un numero
    int flagannoqua=0; //indicatore per vedere se quarta cifra del anno sia un numero
    int giorno =0; //inizializzare una variabile che corrisponde al giorno inserito
    int mese=0; //inizializzare una variabile che corrisponde al mese inserito
    int anno=0; //inizializzare una variabile che corrisponde al anno inserito
    for(int i=0; i<10; i++){
        //considerando le cifre corrispondenti e trasformare in numeri nella variabile
        if(data[0] == numeri[i]){
            giorno=giorno+(10)*i;
            flaggiornouno=1;
        }
        if(data[1] == numeri[i]){
            giorno=giorno+i;
            flaggiornodue=1;
        }
        if(data[3] == numeri[i]){
            mese=mese+(10)*i;
            flagmeseuno=1;
        }
        if(data[4] == numeri[i]){
            mese=mese+i;
            flagmesedue=1;
        }
        if(data[6] == numeri[i]){
            anno=anno+(1000)*i;
            flagannouno=1;
        }
        if(data[7] == numeri[i]){
            anno=anno+(100)*i;
            flagannodue=1;
        }
        if(data[8] == numeri[i]){
            anno=anno+(10)*i;
            flagannotre=1;
        }
        if(data[9] == numeri[i]){
            anno=anno+i;
            flagannoqua=1;
        }
    }
    if(flaggiornouno!=1 || flaggiornodue!=1){ //controlla che nel posto del giorno ha inserito un numero
        printf("Inserisci un giorno valido con numeri tra 1 e 31\n");
        return -1;
    }
    if(flagmeseuno!=1 || flagmesedue!=1){ //controlla che nel posto del mese ha inserito un numero
        printf("Inserisci un mese valido con numeri tra 1 e 12\n");
        return -1;
    }
    if(flagannouno!=1 || flagannodue!=1 || flagannotre!=1 || flagannoqua!=1){ //controlla che nel posto del anno ha inserito un numero
        printf("Inserisci un anno valido con numeri tra 2000 e 2100\n");
        return -1;
    }
    if(giorno<1 || giorno >31){ //controlla che giorno sia compreso tra 1 e 31
        printf("Inserisci un giorno compreso tra 1 e 31\n");
        return -1;
    }
    if(mese<1 || mese>12){ //controlla che mese sia compreso tra 1 e 12
        printf("Inserisci un mese compreso tra 1 e 12\n");
        return -1;
    }
    if(anno<2000 || anno>2100){ //controllo che anno sia compreso tra 2000 e 2100
        printf("Inserisci un anno compreso tra 2000 e 2100\n");
        return -1;
    }
    if((mese %2==0 && mese<=8) || (mese%2==1 && mese>8)){ //si ricorda che mese pari prima di Settembre e mese dispari dopo Agosto non hanno 31 giorni
        if(giorno==31){
            printf("Nel mese %d non ha 31 giorni, inserisci una data valida\n",mese);
            return -1;
        }
    }
    if(mese==2 && giorno>28){ //gestisce pure il caso di anni bisestile
        if(giorno>29 || anno%4!=0){
            printf("Nel febbraio del %d non ha %d giorni\n",anno,giorno);
            return -1;
        }
    }
    return 1; //se nessuno di questi errori sono presenti allora è una data valida
}

int controllonumerospettacolo(){
    //basta considerare vari casi
    if(num_spettacoli>Maxspettacoli){
        return -1;
    }
    else if(num_spettacoli==Maxspettacoli){
        return 0; //in questo caso non si esce dallo spazio ma ha raggiunto al suo massimo
    }
    else{
        return 1;
    }
}

void inserimentospettacolo(Spettacolo *Spettacoli)
{
    // dichiarare una variabile locale per comodità del codice
    int i = num_spettacoli;
    if (controllonumerospettacolo() == -1)
    {
        printf("Non ci sono spazi disponibili per inserimento\n");
        int flagra = riallocazione(Spettacoli);
        //vari casi di riallocazione
        if (flagra == 1)
        {
            printf("Riallocazione con successo, adesso abbiamo nuova capacità %d\n", Maxspettacoli);
        }
        else if (flagra == -1)
        {
            printf("Riallocazione ha incontrato qualche problema, riprova più tardi\n");
        }
        else
        {
            return;
        }
    }; // controllo spazio siponibile
    int flag = 0; // indicatore per conferma di inserimento dati
    do
    {
        int flagcodice = 0; //indicatore per validità del codice
        do
        {
            int tempcodice; //variabile temporanea per codice
            printf("Inserisci codie spettacolo corrispondente : ");
            scanf("%d", tempcodice);
            flagcodice = 1;
            if (controllacodice(tempcodice,Spettacoli) == -1)
            { //controlla che sia univoca
                printf("codice già esistente e deve essere univoco, riprova.\n");
                flagcodice = 0;
            }
            else
            { //assegnare se non ci sono problemi
                Spettacoli[i].codice_spettacolo = tempcodice;
            }
        } while (flagcodice != 1);
        printf("Inserisci titolo dello spettacolo con massimo 100 caratteri : ");
        scanf("%100s", &Spettacoli[i].titolo); //un titolo può avere qualsiasi contenuto
        printf("Inserisci genere dello spettacolo con massimo 30 caratteri : ");
        scanf("%30s", &Spettacoli[i].genere); //analogamente per genere
        int flagdata = 0; //indicatore per validità di data
        do
        {
            printf("Inserisci data dello spettacolo con formato gg/mm/aaaa : ");
            char tempdata[11]; //dichiarare una variabile temporanea per data
            scanf("%10s", &tempdata); //controllo di lunghezza, quelli oltre viene ignorata
            flagdata = 1;
            if (controlladata(tempdata) == -1)
            { //controlla che data sia valido
                printf("Si chiede di inserire una data valida con formato corrispondente segnato\n");
                flagdata = 0;
            }
            else
            {//assegnazione nel caso corretto
                printf("Data valida e inserito correttamente\n");
                strcpy(Spettacoli[i].data_spettacolo, tempdata);
            }
        } while (flagdata != 1);
        int flagposti = 0; //indicatore per numero di posti
        do
        {
            int temppos;
            printf("Inserisci posti totali dello spettacolo : ");
            scanf("%d", &temppos);
            flagposti = 1;
            if (temppos < 1)
            { //basta che sia maggiore di 0
                printf("Il numero di posti totali deve essere un numero intero strettamente maggiore di 0\n");
                flagposti = 0;
            }
            else
            {
                Spettacoli[i].posti_totali = temppos;
            }
        } while (flagposti != 1);
        Spettacoli[i].posti_disponibili = Spettacoli[i].posti_totali;
        int flagprezzo = 0; //indicatore per prezzo
        do
        {
            float tempprezzo;
            printf("Inserisci prezzo di un singolo biglietto dello spettacolo : ");
            scanf("%f", &tempprezzo);
            flagprezzo = 1;
            if (tempprezzo <= 0) //basta che sia positivo e non zero
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
        printf("%-20s %-20s %-20s %-20s %20s %-20s %-20s\n","Codice","Titolo","Genere","data","posti totali","posti disponibili","Prezzo biglietto");
        stampaspettacolo(Spettacoli[i]); //stampare dati inseriti
        printf("Per dare conferma di inserimento inserisci 1, altrimenti 0\n");
        scanf("%d", &flag); //in realtà vale per tutti diversi da 1 per riinserimento, sicuramente bene per 0
    } while (flag != 1);
    printf("dati dello spettacolo salvato correttamente\n");
    num_spettacoli++; //incrementare solo quando tutti i dati sono registrati correttamente in modo valido
    if (controllonumerospettacolo() == 0)
    { //controlla spazio disponibile
        printf("Spazio allocato per spattacoli ha raggiunto la sua massima capacità\n");
        riallocazione(Spettacoli);
    }
    return;
}

void menu(){
    Maxspettacoli=richiestaMaxspettacoli();
    Maxclienti=richiestaMaxclienti();
    Spettacolo * Spettacoli=avviospettacoli(Maxspettacoli);
    Cliente * Clienti=avvioclienti(Maxclienti);
    int scelta=0;
    do{
        printf("insersci la tua scelta : ");
        scanf("%d",&scelta);
        inserimentospettacolo(Spettacoli);
    } while(scelta!=0);
}
void stampaspettacolo(Spettacolo spettacolo){ //stampa dati
    printf("%-20d %-20s %-20s %-20s %-20d %-20d %20.2f\n",spettacolo.codice_spettacolo,spettacolo.titolo,spettacolo.genere,spettacolo.data_spettacolo,spettacolo.posti_totali,spettacolo.posti_disponibili,spettacolo.prezzo_biglietto);
    return;
}