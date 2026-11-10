#include <stdio.h>

typedef struct
{
    char data_spettacolo[11];
} Spettacolo;

int scanstring(char data[], int lung)
{
    int risultato = 0;
    char numeri[11] = {"0123456789"};
    for (int j = 0; j < lung; j++)
    {
        for (int i = 0; i < 10; i++)

        {
            int esp=1;
            for(int k=0 ; k<lung-j-1; k++)
            {
                esp=esp*10;
            }
            if (data[j] == numeri[i])
            {
                risultato = risultato + esp * i;
            }
        }
    }
    return risultato;
}

int trasformadata(Spettacolo spettacolo)
{
    char giornot[3] = {spettacolo.data_spettacolo[0], spettacolo.data_spettacolo[1]};
    char meset[3] = {spettacolo.data_spettacolo[3], spettacolo.data_spettacolo[4]};
    char annot[5] = {spettacolo.data_spettacolo[6], spettacolo.data_spettacolo[7], spettacolo.data_spettacolo[8], spettacolo.data_spettacolo[9]};
    int giorno = scanstring(giornot, 2);
    int mese = scanstring(meset, 2);
    int anno = scanstring(annot, 4);
    printf("%s\n",giornot);
    printf("%s\n",meset);
    printf("%s\n",annot);
    printf("%d\n",giorno);
    printf("%d\n",mese);
    printf("%d\n",anno);
    int data = anno * 10000 + mese * 100 + giorno;
    return data;
}
int main()
{
    Spettacolo spettacolo;
    printf(": ");
    scanf("%10s", spettacolo.data_spettacolo);
    printf("%s\n", spettacolo.data_spettacolo);
    printf("%d\n", trasformadata(spettacolo));
    return 0;
}