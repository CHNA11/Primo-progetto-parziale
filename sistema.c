#include <stdio.h>
#include <stdlib.h> //introduco perché almeno mi serve per allocazione dinamica

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

void avvio(); // questa funzione chiede password per entrare nel sistema di operazione
int richiestaMaxspettacoli();
int richiestaMaxclienti();
Spettacolo *avviospettacoli(int Maxspettacoli);
Cliente *avvioclienti(int Maxclienti);

int main()
{
    avvio();
}

void avvio()
{
    int password;
    size_t flag = 1;
    FILE *fp = fopen("password.dat", "rb");
    if (fp == NULL)
    {
        password = 123456;
    }
    else
    {
        flag = fread(&password, sizeof(int),1 , fp);
        if (flag != 1)
        {
            printf("problemi sulla gestione della password, eliminare password.dat dalla cartella\n");
            exit(1);
        }
        password = password - 123456;
        fclose(fp);
    }

    int passwordinput;
    int riprova = 0;
    int correttezza = 0;
    do
    {
        printf("inserisci la password per entrare in accesso al sistema : ");
        scanf("%d", &passwordinput);
        if (passwordinput == password)
        {
            correttezza = 1;
        }
        else
        {
            printf("password non corrispondente, per riprovare inserisci 1, altrimenti qualsiasi numero : ");
            scanf("%d", &riprova);
        }
    } while (correttezza == 0 && riprova == 1);
    if (correttezza == 0)
    {
        exit(0);
    }
    int scelta = 0;
    do
    {
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
            {
                printf("la nuova password deve essere diversa da quella vecchia\n");
            }
            else
            {
                nuovapassword = nuovapassword + 123456;
                FILE *fp = fopen("password.dat", "wb");
                flag = fwrite(&nuovapassword, sizeof(int), 1, fp);
                if(flag!=1)
                {
                    printf("Errore nel salvare nuova passoword\n");
                    fclose(fp);
                }
                else
                {
                    printf("Nuova password salvato con successo\n");
                    fclose(fp);
                }
            }
        }
        else if (scelta == 2)
        {
            printf("Avvio con successo, benvenuto al sistema di manager per teatro!\n");
        }
        else if (scelta == 3)
        {
            exit(0);
        }
        else
        {
            printf("scelta non valida, si ritorna al menu principale\n");
        }
    } while (scelta != 2);
}

int richiestaMaxspettacoli()
{
    int Maxspettacoli; // dichiarare variabile per allocazione dinamica dei spettacoli
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
    int Maxclienti; // dichiarare variabile per
    do
    {
        printf("inserisci numero massimo di clienti da gestire : ");
        scanf("%d", &Maxclienti);
        if (Maxclienti < 1)
        {
            printf("il numero richiesto deve essere un intero positivo non nullo\n");
        }
    } while (Maxclienti < 1);
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
    {
        printf("Allocazione per Clienti insuccesso\n");
        exit(1);
    }
    return Clienti;
}
