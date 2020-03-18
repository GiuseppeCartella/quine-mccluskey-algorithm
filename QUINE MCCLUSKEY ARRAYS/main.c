#include "algoritmo.h"

int main() {
	printf("                                 --------------------------------\n");
	printf("                                 | Algoritmo di Quine McCluskey |\n");
	printf("                                 --------------------------------\n");


	dati **total = malloc(sizeof(dati*)); //vettore in cui inserisco i dati di mintermini e dontcare
	*total = NULL;
	int num_tot = 0;    //tengo conto del numero totale tra mintermini e indifferenze
	int best = -1;     //variabile che mi dice quale e il numero piu grande da rappresentare

	
	int n_mintermini = 0;
	printf("Inserisci gli indici di riga della tab. di verita' con uscita pari a 1 (mintermini). \nSi termina con un numero negativo: \n");
	int **mintermini = malloc(sizeof(int*));  //tengo conto dei valori che hanno la funzione 
	*mintermini = NULL;

	bool riconoscimento_indiff = false;
	getdata(total, &best,mintermini, &n_mintermini,&num_tot,riconoscimento_indiff,mintermini,n_mintermini); //metto i valori letti da input ordinandoli a seconda del numero di 1(rappr. binaria)

	int n_indifferenze = 0;
	int **indifferenze = malloc(sizeof(int*));  //tengo conto dei valori che hanno la funzione come indifferenze, per non inserirli nella lista due volte
	*indifferenze = NULL;
	printf("\nInserisci gli indici di riga della tab.di verita' con uscita pari a - (indifferenze). \nSi termina con un numero negativo: \n");

	riconoscimento_indiff = true;
	getdata(total, &best, indifferenze, &n_indifferenze,&num_tot,riconoscimento_indiff,mintermini,n_mintermini);

	//ordino ora il vettore in base al numero di uni
	ordina_vettore(total,n_indifferenze+n_mintermini);


	if (n_mintermini == 0) {
		printf("ATTENZIONE: Non e' stato inserito alcun mintermine, pertanto, LA FUNZIONE DI USCITA VALE SEMPRE 0.");
		printf("\n\nPREMERE UN TASTO PER USCIRE DAL PROGRAMMA...");
		getch();
		exit(0);
	}

	int n_bit;
	int min_bit = dimensione(best);

	do {   //stabilisco quale e il numero piu grande e quindi quanti bit minimo utilizzare
		printf("Inserire il numero di bit con cui si intende rappresentare i valori (numero di ingressi, minimo %d): ", min_bit);
		scanf("%d", &n_bit);

		if (n_bit < min_bit) {
			printf("ATTENZIONE: IL VALORE INSERITO NON E' SUFFICIENTE A RAPPRESENTARE TUTTI I MINTERMINI, RIPROVARE...\n\n");
		}

	} while (n_bit< min_bit);


	converti_tutto_vettore(total, n_bit, num_tot);

	int n_implicanti_primi = num_tot;

	bool fine = true;
	while (fine == true) {//mi fermo quando non faccio piu semplificazioni
		total=semplifica(total, n_bit, &fine,&n_implicanti_primi);
	}
	
	elimina_indiff(total, *mintermini, n_mintermini, &n_implicanti_primi);
	num_tot = n_implicanti_primi;

	//INIZIO LA COPERTURA
	matrix *copertura = malloc(sizeof(struct matrix));
	inizializza_matrice(copertura, *mintermini, n_mintermini, n_implicanti_primi, total);
	//n_implicanti indicherà il numero delle colonne, lunghezza lista il numero di righe

	int **implicanti_finali = malloc(sizeof(int*));
	*implicanti_finali = NULL;
	int* implicanti_primi = malloc(n_implicanti_primi * sizeof(int));
	implicanti_primi = riempi_in_ordine(implicanti_primi, n_implicanti_primi);

	int n_implicanti_finali = 0;

	copertura = semplifica_matrice(copertura, implicanti_finali, implicanti_primi, &n_implicanti_primi, &n_implicanti_finali);

	if (n_mintermini == (int)pow(2, n_bit))
		printf("( 1 )");
	else
		stampa_soluzione_finale(*implicanti_finali, n_implicanti_finali, total,n_bit,num_tot);


	//dealloco tutto
	if (copertura != NULL) {
		if (copertura->data != NULL)
			free(copertura->data);

		free(copertura);
	}

	dealloca_vettore(mintermini);
	dealloca_vettore(indifferenze);
	dealloca_vettore(implicanti_finali);


	if (n_implicanti_primi != 0 && implicanti_primi != NULL) //controllo di non aver gia svuotato tutto con le realloc nella funzione semplifica_matrice 
		free(implicanti_primi);


	if (total != NULL) {
		if (*total != NULL) {
			for (int m = 0; m < num_tot; ++m) {
				free((*total)[m].value);
				free((*total)[m].rappr_bin);
			}
			free(*total);
		}
		free(total);
	}

	printf("\n\nPREMERE UN TASTO PER USCIRE DAL PROGRAMMA...");
	getch();

	exit(0);	

}