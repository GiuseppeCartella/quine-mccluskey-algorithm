#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//LISTE

typedef int element;

//Definisco tipo elemento della lista
typedef struct list_element {
	element *value;
	char* rappr_bin;
	int num_uni;
	int valori_coperti; //numeri di valori che la stringa copre
	struct list_element *next;
} item;

//Definisco tipo lista (puntatore a primo elemento)
typedef item* list;

//definisco tipo matrix
typedef struct matrix {
	size_t rows, cols;
	char *data;
} matrix;

list tail(list l)
{
	if (l == NULL)
		abort();
	else
		return l->next;
}

void dealloca(list l)
{
	list tmp = NULL;
	while (l != NULL) {
		tmp = l;
		l = l->next;
		free(tmp->rappr_bin);
		free(tmp->value);
		free(tmp);
	}
}

int conta_uni(char *p, int n_bit) {
	int cont = 0;
	for (int i = 0; i < n_bit; ++i) {
		if (p[i] == '1')
			cont++;
	}
	return cont;
}

int conta_uni2(int el) { //conta gli uni prima della conversione binaria.
	int cont = 0;
	bool tmp;
	while (el != 0)
	{
		tmp = el % 2;
		el = el / 2;
		if (tmp == 1)
			cont++;
	}
	return cont;
}

list cons(element e, list l, int g)
{
	list t;
	t = (list)malloc(sizeof(item));
	t->value = malloc(sizeof(element));
	t->value[0] = e;
	t->valori_coperti = 1;
	t->num_uni = g;

	t->next = l;
	return t;
}

list sviluppo_lista_succ(list k, list h, list h2, int dh, int n_bit) {

	//CASO DI LISTA VUOTA
	if (k == NULL) {
		list t = (list)malloc(sizeof(item));
		t->value = malloc((h->valori_coperti + h2->valori_coperti) * sizeof(int));
		int r = 0;
		for (; r < h->valori_coperti; ++r) {
			(t->value)[r] = (h->value)[r];
		}

		for (; r - h->valori_coperti < h2->valori_coperti; ++r) {
			(t->value)[r] = (h2->value)[r - h->valori_coperti];
		}

		t->valori_coperti = h->valori_coperti + h2->valori_coperti;
		//inserisco la nuova rappr binaria
		t->rappr_bin = malloc((n_bit + 1) * sizeof(char));
		for (int i = 0; i < n_bit; ++i) {
			if (i == dh) {
				(t->rappr_bin)[i] = '-';
			}
			else
				(t->rappr_bin)[i] = (h->rappr_bin)[i]; //qui e indifferente usare h oh2 perche sono uguale a meno dell indice dh
		}
		(t->rappr_bin)[n_bit] = 0;
		t->num_uni = conta_uni(t->rappr_bin, n_bit);
		t->next = NULL;
		return t;
	}


	//CASO DI LISTA CHE CONTIENE ALMENO UN ELEMENTO
	list t = k;
	while (t->next != NULL) {
		t = t->next;
	}

	t->next = (list)malloc(sizeof(item));
	t = t->next;
	t->value = malloc((h->valori_coperti + h2->valori_coperti) * sizeof(int));
	int r = 0;
	for (; r < h->valori_coperti; ++r) {
		(t->value)[r] = (h->value)[r];
	}

	for (; r - h->valori_coperti < h2->valori_coperti; ++r) {
		(t->value)[r] = (h2->value)[r - h->valori_coperti];
	}

	t->valori_coperti = h->valori_coperti + h2->valori_coperti;
	//inserisco la nuova rappr binaria
	t->rappr_bin = malloc((n_bit + 1) * sizeof(char));
	for (int i = 0; i < n_bit; ++i) {
		if (i == dh) {
			(t->rappr_bin)[i] = '-';
		}
		else
			(t->rappr_bin)[i] = (h->rappr_bin)[i]; //qui e indifferente usare h o h2 perche sono uguale a meno dell indice dh
		(t->rappr_bin)[n_bit] = 0;
		t->num_uni = conta_uni(t->rappr_bin, n_bit);
		t->next = NULL;
	}

	return k;
}

list insordList(element e, list l) //Data una lista già ordinata in ordine DECRESCENTE aggiunge al posto giusto el
{
	list l1 = NULL, root = l;
	list t;
	int g = conta_uni2(e); //la variab tiene conto di quanti 1 servono per la rappr binaria
	if (l == NULL || g < l->num_uni)
	{
		t = cons(e, l, g);
		return t;
	}
	t = cons(e, NULL, g);
	while (l != NULL && g >= l->num_uni)
	{
		l1 = l;
		l = tail(l);
	}
	l1->next = t;
	t->next = l;
	return root;
}

bool numero_in_vettore(int valore, element* vettore, int lunghezza)
{
	for (int i = 0; i < lunghezza; i++)
	{
		if (vettore[i] == valore)
			return true;
	}
	return false;
}

list getlist(list l, int* best, int **vettore, int *lunghezza, int riconoscimento_indiff, int **mintermini, int num_mintermini)
{
	//La funzione chiede all'utente dei numeri int da aggiungere alla lista fino a quando non inserisce un numero negativo
	element el;
	int cont = 0;
	do
	{
		scanf("%d", &el);
		if (el >= 0) {
			if (el > *best) {
				*best = el;
			}

			if (riconoscimento_indiff == true && numero_in_vettore(el, *mintermini, num_mintermini)) {
				printf("\nATTENZIONE,ERRORE: Il valore inserito rappresenta gia' un mintermine!");
				printf("\n\nPREMERE UN TASTO PER USCIRE DAL PROGRAMMA...");
				getch();
				exit(0);
			}

			if (!(numero_in_vettore(el, *vettore, *lunghezza))) {
				*vettore = realloc(*vettore, (cont + 1) * sizeof(int));
				(*vettore)[cont] = el;
				cont++;
				(*lunghezza) = cont;
				l = insordList(el, l); //inserisco in modo ordinato a seconda del numero di uni
			}

		}

	} while (el >= 0);

	return l;
}

int lunghezza_lista(list l)
{
	int lunghezza = 0;
	while (l != NULL)
	{
		lunghezza++;
		l = tail(l);
	}
	return lunghezza;
}

int dimensione(int best)
{
	int count = 0;

	while (best != 0)
	{
		best = best / 2;
		count++;
	}

	if (count == 0)
		count++;

	return count;
}

char* converti_bin(element el, int n_bit)
{
	char* final = malloc((n_bit + 1) * sizeof(char));

	bool tmp;

	for (int i = (n_bit - 1); i >= 0; i--)
	{
		if (el != 0)
		{
			tmp = el % 2;
			if (tmp == 0)
				final[i] = '0';
			else
				final[i] = '1';
			el = el / 2;
		}
		else
			final[i] = '0';
	}
	final[n_bit] = 0;
	return final;
}

list converti_tutta_lista(list l, int n_bit)
{
	list ris = l;
	while (l != NULL)
	{
		l->rappr_bin = converti_bin(l->value[0], n_bit);
		l = tail(l);
	}

	return ris;
}

bool confronta_indifferenze(char *p1, char *p2, int n_bit) {
	//da come avviene l implementazione sono sicuro che le due stringhe hanno stessa lunghezza
	for (int i = 0; i < n_bit; ++i) {
		if ((p1[i] == '-'&&p2[i] != '-') || (p2[i] == '-'&&p1[i] != '-'))
			return false;
	}
	return true;
}

bool confronta_distanza(char *p1, char *p2, int n_bit, int *indice_errore) {
	//questo risultato viene usato solo se confronta_indifferenze=true.
	//quindi se entro in questa funzione do per scontato che le indifferenze si trovino alle stesse posizioni.
	//la var indice_errore mi dice in quale posiozione ho la disuguaglianza
	int c = 0;

	for (int i = 0; i < n_bit; ++i) {
		if ((p1[i] != p2[i]) && c != 0) //se la distanza diventa maggiore di uno ritorno false
			return false;

		if ((p1[i] != p2[i]) && c == 0) { //se i due bit non coincidono ma ancora non ho trovato altre disuguaglianze incremento e continuo.
			c++;
			*indice_errore = i;
		}

	}
	return true;
}

list cons_in_coda(list dest, list sorg, int n_bit) {
	if (dest == NULL) {
		list t = (list)malloc(sizeof(item));
		t->value = malloc(sizeof(int) * sorg->valori_coperti);

		for (int i = 0; i < sorg->valori_coperti; i++)
			(t->value)[i] = (sorg->value)[i];

		t->rappr_bin = malloc(sizeof(char) * (n_bit + 1));
		t->valori_coperti = sorg->valori_coperti;

		for (int j = 0; j < n_bit; j++)
			(t->rappr_bin)[j] = (sorg->rappr_bin)[j];

		t->rappr_bin[n_bit] = 0;
		t->num_uni = sorg->num_uni;
		t->next = NULL;
		return t;
	}


	list t = dest;
	while (t->next != NULL) {
		t = t->next;
	}

	t->next = (list)malloc(sizeof(item));
	t = t->next;

	t->value = malloc(sizeof(int) * sorg->valori_coperti);

	for (int i = 0; i < sorg->valori_coperti; i++)
		(t->value)[i] = (sorg->value)[i];

	t->rappr_bin = malloc(sizeof(char) * (n_bit + 1));
	t->valori_coperti = sorg->valori_coperti;

	for (int j = 0; j < n_bit; j++)
		(t->rappr_bin)[j] = (sorg->rappr_bin)[j];

	t->rappr_bin[n_bit] = 0;
	t->num_uni = sorg->num_uni;
	t->next = NULL;

	return dest;
}

bool VettoreInLista(int* v1, int dim1, list l) //Restituisce true se tutti gli elementi di v1 sono contenuti in almeno un implicante della lista l (anche in ordine diverso)
{
	int giusti = 0;

	if (l == NULL)
		return false;

	while (l != NULL) {
		int dim2 = l->valori_coperti;
		if (v1 == NULL || l->value == NULL || dim1 > dim2) { //v1 e v2 non saranno mai NULL, questa riga serve per evitare crash improvvisi del programma	
			if (l->next == NULL)
				return false;
			l = l->next;
			continue;
		}

		for (int i = 0; i < dim1; i++)
		{
			for (int j = 0; j < dim2; j++)
			{
				if (v1[i] == (l->value)[j]) {
					giusti++;
				}
			}
		}
		if (giusti == dim1) {
			return true;
		}
		else {
			giusti = 0;
			l = l->next;
		}
	}
	return false;
}

list semplifica(list h, int n_bit, bool* fine) {
	int dh;

	list h2 = h->next;
	list k = NULL;

	*fine = false;
	while (h != NULL) {
		bool inserito = false;
		if (h2 == NULL && VettoreInLista(h->value, h->valori_coperti, k) == false) { //COMMENTA, ULTIMO GRUPPO
			k = cons_in_coda(k, h, n_bit);
			inserito = true;
		}

		while (h2 != NULL) {
			if ((h->num_uni == h2->num_uni - 1) && (confronta_distanza(h->rappr_bin, h2->rappr_bin, n_bit, &dh) == true) && (confronta_indifferenze(h->rappr_bin, h2->rappr_bin, n_bit) == true)) {
				//se due stringhe sono semplificabili allora inizio la costruzione della lista successiva,che quindi conterra indifferenza
				int* vett_tmp = malloc((h->valori_coperti + h2->valori_coperti) * sizeof(int));

				int r = 0;
				for (; r < h->valori_coperti; ++r) {
					vett_tmp[r] = (h->value)[r];
				}

				for (; r - h->valori_coperti < h2->valori_coperti; ++r) {
					vett_tmp[r] = (h2->value)[r - h->valori_coperti];
				}

				if (VettoreInLista(vett_tmp, (h->valori_coperti + h2->valori_coperti), k) == false) {

					k = sviluppo_lista_succ(k, h, h2, dh, n_bit);
					*fine = true; //se unisco qualcosa
					inserito = true;
					free(vett_tmp);
				}
				else
					free(vett_tmp);
			}

			h2 = h2->next;

			if (h2 == NULL && inserito == false && VettoreInLista(h->value, h->valori_coperti, k) == false) { //Penultimo gruppo se non trova condizini																				  
				k = cons_in_coda(k, h, n_bit);
				inserito = true;
			}

			if (h2 != NULL && ((h2->num_uni - h->num_uni) >= 2)) {
				if (inserito == false && VettoreInLista(h->value, h->valori_coperti, k) == false) { //COMMENTA, QUANDO UN VETTORE NON E' STATO COPIATO NE COL GRUPPO PRIMA NE COL GRUPPO DOPO															
					k = cons_in_coda(k, h, n_bit);
					inserito = true;
				}
				break;
			}
		}

		if (h != NULL) {
			h = h->next;
			if (h != NULL)
				h2 = h->next;
		}
	}
	dealloca(h);
	dealloca(h2);
	return k;
}

void stampa_matrice(matrix* m)
{
	for (size_t r = 0; r < m->rows; ++r) {
		for (size_t c = 0; c < m->cols; ++c) {
			printf(" %i ", m->data[m->cols * r + c]);
		}
		printf("\n");
	}
}

void inizializza_matrice(matrix* m, int* mintermini, int n_mintermini, int n_implicanti_primi, list l) {
	m->rows = n_implicanti_primi;
	m->cols = n_mintermini;
	m->data = malloc(m->cols*m->rows * sizeof(char));

	for (size_t r = 0; r < m->rows; ++r) {
		for (size_t c = 0; c < m->cols; ++c) {
			if (numero_in_vettore(mintermini[c], l->value, l->valori_coperti) == 1)
				m->data[m->cols * r + c] = 1;
			else
				m->data[m->cols * r + c] = 0;
		}
		l = l->next;
	}

}

matrix *elimina_riga_colonne(matrix* m, int* indici_colonne, int indice_riga, int n_cols_da_eliminare) {

	matrix *update = malloc(sizeof(struct matrix));
	update->cols = m->cols - n_cols_da_eliminare;
	update->rows = m->rows - 1;
	update->data = malloc(sizeof(char) * update->cols * update->rows);
	int messi = 0;

	for (size_t r = 0; r < m->rows; ++r) {
		for (size_t c = 0; c < m->cols; ++c) {
			if (numero_in_vettore(c, indici_colonne, n_cols_da_eliminare) == 0 && r != indice_riga) {
				update->data[messi] = m->data[m->cols * r + c];
				messi++;
			}
		}
	}

	free(m->data);
	free(m);

	return update;
}

int* riempi_in_ordine(int* vett, int dim)
{
	for (int i = 0; i < dim; i++)
		vett[i] = i;
	return vett;
}

bool tutto_indiff(int* vett, int* mintermini, int vett_dim, int mintermini_dim) { //Controlla se almeno un elemento di vett è contenuto tra i mintermini, nel caso torna false. se no ritorna true
	for (int i = 0; i < vett_dim; i++) {
		for (int j = 0; j < mintermini_dim; j++) {
			if (vett[i] == mintermini[j])
				return false;
		}
	}
	return true;
}

list elimina_indiff(list l, int* mintermini, int n_mintermini, int *n_implicanti_primi) {
	list s, r;

	if (l != NULL) {

		/* eliminazione negativi dal secondo in poi */
		if (l->next != NULL) {
			s = l;
			while (s->next != NULL) {
				if (tutto_indiff(s->next->value, mintermini, s->next->valori_coperti, n_mintermini)) {
					r = s->next;
					s->next = s->next->next;
					(*n_implicanti_primi)--;
					free(r);
				}
				else
					s = s->next;
			}
		}

		/* elimina primo elemento se negativo */
		if (tutto_indiff(l->value, mintermini, l->valori_coperti, n_mintermini)) {
			r = l;
			l = l->next;
			(*n_implicanti_primi)--;
			free(r);
		}
	}
	return l;
}

bool domina_altra_riga(matrix* m, int riga_domina, int riga_dominata) {
	for (size_t c = 0; c < m->cols; c++) {
		if ((m->data[m->cols * riga_domina + c] == 0) && (m->data[m->cols * riga_dominata + c] == 1))
			return false;
	}
	return true;
}

bool domina_altra_colonna(matrix* m, int colonna_domina, int colonna_dominata) {
	for (size_t r = 0; r < m->rows; r++) {
		if ((m->data[m->cols * r + colonna_domina] == 1) && (m->data[m->cols * r + colonna_dominata] == 0))
			return false;
	}
	return true;
}

matrix* elimina_righe_dominate(matrix* m, bool* riuscito2, int** implicanti_primi, int* n_implicanti_primi) {
	matrix* update = malloc(sizeof(struct matrix));
	update->cols = m->cols;
	update->rows = 0;
	update->data = NULL;
	*riuscito2 = false;
	int eliminati = 0;

	for (size_t r = 0; r < m->rows; ++r) {
		if (m->data[m->cols * r] != -1) { //L'elemento non è da togliere. Lo copio nella nuova matrice
			(update->rows)++;
			update->data = realloc(update->data, (update->cols * update->rows) * sizeof(char));
			for (size_t i = 0; i < m->cols; i++)
				update->data[update->cols * (update->rows - 1) + i] = m->data[m->cols * r + i];

		}
		else { //Tengo traccia delle posizioni degli implicanti nella lista originale

			*riuscito2 = true;
			(*n_implicanti_primi)--;

			for (size_t i = r; i < m->rows - 1; i++)
				(*implicanti_primi)[i - eliminati] = (*implicanti_primi)[i + 1 - eliminati];

			*implicanti_primi = realloc(*implicanti_primi, sizeof(int) * (*n_implicanti_primi));
			eliminati++;
		}
	}

	free(m->data);
	free(m);

	return update;
}

matrix* elimina_colonne_dominate(matrix* m, bool* riuscito3) {
	matrix* update = malloc(sizeof(struct matrix));
	update->cols = 0;
	update->rows = m->rows;
	*riuscito3 = false;

	for (size_t c = 0; c < m->cols; c++) {
		if (m->data[c] != -1)
			(update->cols)++;
	}

	update->data = malloc(update->cols * update->rows * sizeof(char));
	size_t c_update = 0;

	for (size_t c = 0; c < m->cols; c++) {
		if (m->data[c] == -1)
			continue;
		for (size_t r = 0; r < m->rows; r++) {
			update->data[update->cols * r + c_update] = m->data[m->cols * r + c];
		}
		c_update++;
	}


	free(m->data);
	free(m);

	return update;
}

matrix *semplifica_matrice(matrix* m, int** implicanti_finali, int* implicanti_primi, int *n_implicanti_primi, int *n_implicanti_finali) {
	//Primo passo: cerco gli implicanti primi ed essenziali e li salvo nel vettore finale
	bool riuscito = true;
	bool riuscito2 = true;
	bool riuscito3 = true;
	bool colonna_valida = true;
	int zeri_in_colonna = 0;
	int inseriti = 0;
	int* indici_colonne_da_eliminare = NULL;
	bool matrice_ridotta = false;


	while (riuscito2 == true || riuscito3 == true) {
		//PRIMO CRITERIO: ESSENZIALITA'

		while (riuscito == true) {

			for (size_t r = 0; r < m->rows; ++r) {
				for (size_t c = 0; c < m->cols; ++c) {

					if (m->data[m->cols * r + c] == 0) //se supero questo controllo, sto analizzando un uno
						continue;

					for (size_t i = c; i < m->rows * m->cols; i = i + m->cols)
					{
						if (i == (m->cols * r + c)) //Se sto provando a confrontare l'elemento con se stesso, vado avanti
							continue;

						if (m->data[i] == 1) //Se uno degli elementi della colonna è uno, vado all'elemento successivo
							break;

						zeri_in_colonna++;
					}

					if (zeri_in_colonna == m->rows - 1) //se tutti gli elementi della colonna a parte se stesso sono zeri, ho trovato un implicante primo
					{
						//Tengo traccia delle posizioni degli implicanti nella lista originale

						*(implicanti_finali) = realloc((*implicanti_finali), (inseriti + 1) * sizeof(int));
						(*implicanti_finali)[inseriti] = implicanti_primi[r];
						inseriti++;
						(*n_implicanti_finali)++;
						(*n_implicanti_primi)--;

						for (size_t i = r; i < m->rows - 1; i++)
							implicanti_primi[i] = implicanti_primi[i + 1];

						implicanti_primi = realloc(implicanti_primi, sizeof(int) * (*n_implicanti_primi));

						int cont = 0;
						for (size_t j = 0; j < m->cols; j++) {
							if (m->data[m->cols * r + j] == 1) {
								indici_colonne_da_eliminare = realloc(indici_colonne_da_eliminare, sizeof(int) * (cont + 1));
								indici_colonne_da_eliminare[cont] = j;
								cont++;
							}

						}
						m = elimina_riga_colonne(m, indici_colonne_da_eliminare, r, cont);
						matrice_ridotta = true;
					}
					else
						zeri_in_colonna = 0;

					if (matrice_ridotta == true)
						break;
				}

				if (matrice_ridotta == true)
					break;

			}

			if (matrice_ridotta == false)
			{
				riuscito = false; //se siamo arrivati a questo punto, abbiamo scorso tutta la matrice senza fare sostituzioni: questa parte di algoritmo può non essere piu ripetuta
			}
			else
			{
				matrice_ridotta = false;
				zeri_in_colonna = 0;
			}
		}

		riuscito = true;

		if (m->data == NULL)
			return m; // m = NULL

		//SECONDO CRITERIO: DOMINANZA DI RIGA
		for (size_t r = 0; r < m->rows; ++r) {
			if (m->data[r * m->cols] == -1)
				continue; //Se il primo elemento della riga è un -1, la riga è tra quelle già eliminate

			for (size_t r2 = 0; r2 < m->rows; ++r2) {
				if (r == r2)
					continue; //Non controllo la riga con se stessa
				if (m->data[r2 * m->cols] == -1)
					continue; //Se il primo elemento della riga è un -1, la riga è tra quelle già eliminate
				if (domina_altra_riga(m, r, r2)) {
					m->data[r2 * m->cols] = -1;
				}
			}
		}

		//Elimino tutte le righe che iniziano per -1
		m = elimina_righe_dominate(m, &riuscito2, &implicanti_primi, n_implicanti_primi); //RIUSCITO2 DEVE ESSERE MODIFICATO ALL'INTERNO DELLA FUNZIONE PRIMA, COSI' E' RIDONDANTE

		//TERZO CRITERIO: DOMINANZA DI COLONNE.
		for (size_t c = 0; c < m->cols; ++c) {
			if (m->data[c] == -1)
				continue; //Se il primo elemento della colonna è un -1, la colonna è tra quelle già eliminate


			for (size_t c2 = 0; c2 < m->cols; ++c2) {
				if (c == c2)
					continue; //Non controllo la colonna con se stessa
				if (m->data[c2] == -1)
					continue; //Se il primo elemento della colonna è un -1, la colonna è tra quelle già eliminate
				if (domina_altra_colonna(m, c, c2)) {
					m->data[c2] = -1; //i -1 li metto nella prima riga
				}
			}
		}


		//Elimino tutte le colonne che iniziano per -1
		m = elimina_colonne_dominate(m, &riuscito3); //RIUSCITO3 DEVE ESSERE MODIFICATO ALL'INTERNO DELLA FUNZIONE PRIMA, COSI' E' RIDONDANTE
	}

	//Nel caso copertura != NULL unisci i vettori implicante_finale con implicanti_primi (cioè la copertura non si è semplificata tutta)
	int cont = *n_implicanti_primi;  //tengo traccia del numero di implicanti primi con cui inizio perche poi dopo tornando al main devo tenere il tutto aggiornato
	if (cont != 0) {
		for (int i = 0; i < cont; i++) {
			n_implicanti_finali++;
			*implicanti_finali = realloc(*implicanti_finali, sizeof(int) * (*n_implicanti_finali));
			(*implicanti_finali)[*n_implicanti_finali - 1] = implicanti_primi[i];
			(*n_implicanti_primi)--;
		}
	}

	return m;
}

void stampa_soluzione_finale(int *implicanti_finali, int n_implicanti_finali, list total, int n_bit) {
	int indice = 0;
	bool messo_primo = false;

	if (total == NULL)
		return;

	while (total != NULL) {
		if (numero_in_vettore(indice, implicanti_finali, n_implicanti_finali)) {
			if (messo_primo == true)
				printf(" + ( ");
			else
				printf("( ");


			messo_primo = true;
			for (int j = 0; j < n_bit; j++) {
				if (((total->rappr_bin)[j] == '0'))
					printf("X%i' ", (n_bit - 1 - j));
				else if (((total->rappr_bin)[j] == '1'))
					printf("X%i ", (n_bit - 1 - j));
			}
			printf(")");
		}
		indice++;
		total = total->next;


	}

	printf("\n");
}