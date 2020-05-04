#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define max 2147483647
#pragma warning(disable: 4996)

typedef struct suradnice { //policka v mape
	int x;
	int y;
	int distance;
	int visited;
	struct suradnice* prev;
} SURADNICE;

typedef struct priority { //struktura priorit
	int distance;
	struct suradnice* sur;
	struct priority* next;
}PRIORITY;

void arrayInit(SURADNICE** array, int dlzkax, int dlzkay) { //inicializuje mapu nacisto
	for (int y = 0; y < dlzkay; y++) {
		for (int x = 0; x < dlzkax; x++) {
			array[x][y].x = x;
			array[x][y].y = y;
			array[x][y].distance = max;
			array[x][y].visited = 0;
			array[x][y].prev = NULL;
		}
	}
}

void vypis(char** mapa, int dlzka) {
	for (int x = 0; x < dlzka; x++) {
		for (int i = 0; i < dlzka; i++) {
			if (mapa[x][i] == 'C')
				printf("1");
			else if (mapa[x][i] == 'H')
				printf("2");
			else
				printf("%c", mapa[x][i]);
		}
		printf("\n");
	}
}

char* vzdialenost(char** mapa, int x, int y) { //vrati pismeno na danych suradniciach
	return  mapa[y][x];
}

void insertpriority(SURADNICE* policko, PRIORITY** first) {
	PRIORITY* new = (struct PRIORITY*) malloc(sizeof(PRIORITY)), * dalsi = *first, * pomocna = NULL, * prev = NULL;

	new->distance = policko->distance;
	new->sur = policko;

	while (dalsi != NULL && dalsi->distance < new->distance) {//ked najde prazdne alebo je mensie ako cislo
		prev = dalsi;
		dalsi = dalsi->next;
	}
	if (*first == NULL) {
		(*first) = new;
		(*first)->next = NULL;
	}
	else if (prev->next == NULL) {
		prev->next = new;
		prev->next->next = NULL;
	}
	else {
		pomocna = dalsi;
		dalsi = new;
		dalsi->next = pomocna;
		prev->next = dalsi;
	}
}

void deletepriority(PRIORITY** first) {
	PRIORITY* delete;
	if ((*first)->next == NULL) {
		*first = NULL;
	}
	else {
		delete = *first;
		*first = (*first)->next;
		//free(delete);
	}
}

void suradnicedraka(SURADNICE** array, int dx, int dy, int* counter1, int* finalarray) {
	SURADNICE* minuly = &array[dx][dy];
	SURADNICE* minuly1 = &array[dx][dy];
	int counter = 0, index;

	//printf("drak je na[%d, %d] a najkratsia vzdialenost %d\n", dx, dy, array[dx][dy].distance);

	while (minuly != NULL) {
		//printf("[%d,%d]\n", minuly->x, minuly->y);
		minuly = minuly->prev;
		counter++;
	}

	int* tempx = (int*)malloc(counter * sizeof(int));
	int* tempy = (int*)malloc(counter * sizeof(int));

	index = counter;
	counter = 0;
	while (minuly1 != NULL) {
		tempx[index - counter - 1] = minuly1->x;
		tempy[index - counter - 1] = minuly1->y;
		minuly1 = minuly1->prev;
		counter++;
	}

	for (int i = 0; i < index; i++) {
		finalarray[2 * (*counter1)] = tempx[i];
		finalarray[2 * (*counter1) + 1] = tempy[i];
		*counter1 += 1;
		//printf("%d %d\n", tempx[i], tempy[i]);
	}
	(*counter1)--;//aby sa neopakovali koncovy a zaciatocny pri zapise do arrayu

	free(tempx);
	free(tempy);
}

int validate(char** mapa, int dlzkax, int dlzkay) { //zisti ci su na mapa spravne pocty draka a princezien
	int d = 0, p = 0;

	for (int y = 0; y < dlzkax; y++) {
		for (int x = 0; x < dlzkay; x++) {
			if (mapa[x][y] == 'D')
				d++;
			if (mapa[x][y] == 'P')
				p++;
		}
	}
	if (d == 1 && p > 0 && p <= 5)
		return p;
	else
		return -1;
}

void pozicieprincezien(int* xova, int* yova, int dlzkax, int dlzkay, char** mapa) { //do arrayu priradi suradnice draka a princezien
	int i = 1;
	for (int y = 0; y < dlzkax; y++) {
		for (int x = 0; x < dlzkay; x++) {
			if (mapa[x][y] == 'D') {
				xova[0] = y;
				yova[0] = x;
			}
			if (mapa[x][y] == 'P') {
				xova[i] = y;
				yova[i] = x;
				i++;
			}
		}
	}
	//vypis poziicia draka a princezien
	/*for (int temp = 0; temp < i; temp++) {
		if (temp == 0)
			printf("drak %d:[%d, %d]\n", temp, xova[temp], yova[temp]);
		else
			printf("princezna %d:[%d, %d]\n", temp, xova[temp], yova[temp]);
	}*/
}

void dijkstra(SURADNICE** array, PRIORITY* first, char** mapa, int dlzkax, int dlzkay) {
	char pismeno;
	int temp = 0;

	while (first != NULL) {
		int x = first->sur->x;
		int y = first->sur->y;

		if (x - 1 >= 0 && array[x - 1][y].visited == 0) {//vlavo
			pismeno = vzdialenost(mapa, x - 1, y);
			if (pismeno == 'N') {
				array[x - 1][y].visited = 1;
			}
			else {
				if (pismeno == 'H')
					temp = 2;
				else temp = 1;
				if (array[x][y].distance + temp < array[x - 1][y].distance) {
					array[x - 1][y].distance = array[x][y].distance + temp;
					insertpriority(&array[x - 1][y], &first);
					array[x - 1][y].prev = &array[x][y];
				}
			}
		}

		if (x + 1 < dlzkax && array[x + 1][y].visited == 0) {//vpravo
			pismeno = vzdialenost(mapa, x + 1, y);
			if (pismeno == 'N') {
				array[x + 1][y].visited = 1;
			}
			else {
				if (pismeno == 'H')
					temp = 2;
				else temp = 1;
				if (array[x][y].distance + temp < array[x + 1][y].distance) {
					array[x + 1][y].distance = array[x][y].distance + temp;
					insertpriority(&array[x + 1][y], &first);
					array[x + 1][y].prev = &array[x][y];
				}
			}
		}

		if (y - 1 >= 0 && array[x][y - 1].visited == 0) {//hore
			pismeno = vzdialenost(mapa, x, y - 1);
			if (pismeno == 'N') {
				array[x][y - 1].visited = 1;
			}
			else {
				if (pismeno == 'H')
					temp = 2;
				else temp = 1;
				if (array[x][y].distance + temp < array[x][y - 1].distance) {
					array[x][y - 1].distance = array[x][y].distance + temp;
					insertpriority(&array[x][y - 1], &first);
					array[x][y - 1].prev = &array[x][y];
				}
			}
		}

		if (y + 1 < dlzkay && array[x][y + 1].visited == 0) {//dole
			pismeno = vzdialenost(mapa, x, y + 1);
			if (pismeno == 'N') {
				array[x][y + 1].visited = 1;
			}
			else {
				if (pismeno == 'H')
					temp = 2;
				else temp = 1;
				if (array[x][y].distance + temp < array[x][y + 1].distance) {
					array[x][y + 1].distance = array[x][y].distance + temp;
					insertpriority(&array[x][y + 1], &first);
					array[x][y + 1].prev = &array[x][y];
				}
			}
		}
		array[x][y].visited = 1;
		deletepriority(&first);
	}
}

int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
	
	int t1 = pow(10, 6);
	if (n >= 1 && n <= 100 && m >= 1 && m <= 100 && t >= 0 && t <= t1) {
		int temp = 0, dx = -2, dy = -2, valid = -1, p = 0, counter = 0;
		//int dlzka = n;
		PRIORITY* first = NULL;
		SURADNICE** array = (SURADNICE*)malloc(m * sizeof(SURADNICE));
		for (int i = 0; i < n; i++)
			array[i] = (SURADNICE*)malloc(n * sizeof(SURADNICE));

		arrayInit(array, n, m);

		valid = validate(mapa, n, m);
		//printf("validate: %d\n", valid);

		if (valid == -1)
			return NULL;
		else {
			int* xova = (int*)malloc(valid * sizeof(int) + 1);
			int* yova = (int*)malloc(valid * sizeof(int) + 1);
			pozicieprincezien(xova, yova, n, m, mapa); //spravi array so suradnicami princezien a draka ulozi na poziciu 0

			char pismeno = vzdialenost(mapa, 0, 0);
			if (pismeno == 'N') {
				//printf("neplatna mapa");
				return NULL;
			}
			else {
				if (pismeno == 'D') {
					dx = 0;
					dy = 0;
				}
				if (pismeno == 'H')
					array[0][0].distance = 2;
				else
					array[0][0].distance = 1;

				insertpriority(&array[0][0], &first);
				dijkstra(array, first, mapa, n, m);

				int* finalarray = (int*)malloc(sizeof(int) * 1000);
				for (int i = 0; i < 1000; i++)
					finalarray[i] = -1;

				if (array[xova[0]][yova[0]].distance != max) { // ak sa da sostat ku drakovi
					for (int i = 1; i < valid + 1; i++) {// ak sa neda dostat ku princeznej
						if (array[xova[i]][yova[i]].distance == max) {
							//printf("neda sa dostat ku princeznej\n");
							return NULL;
						}
					}
					suradnicedraka(array, xova[0], yova[0], &counter, finalarray);

					int x = 0 , tt = 0;
					while(finalarray[2 * x + 1] != -1){//vypocita cas cesty ku drakovi
						if (vzdialenost(mapa, finalarray[2 * x], finalarray[2 * x + 1]) == 'H')
							tt += 2;
						else
							tt++;
						x++;
					}
					//printf("tt: %d\n", tt);
					if (tt > t) {//zisti ci drak stihol zjest princezne
						//printf("Malo casu drak zjedol princezne");
						return NULL;
					}
					//printf("%d %d", xova[1], yova[1]);
				}
				else { //neda sa dostat ku drakovi
					//printf("neda sa dostat ku drakovi\n");
					return NULL;
				}

				//hladanie cesty k princeznam
				int** matica = (int*)malloc(valid * sizeof(int) + 1);
				for (int i = 0; i < valid + 1; i++) {
					matica[i] = (int*)malloc(sizeof(int) + 1);
				}

				for (int i = 0; i < valid + 1; i++) {
					first = NULL;
					arrayInit(array, n, m);
					array[xova[i]][yova[i]].distance = 0;
					insertpriority(&array[xova[i]][yova[i]], &first);
					dijkstra(array, first, mapa, n, m);

					for (int x = 0; x < valid + 1; x++) {
						matica[i][x] = array[xova[x]][yova[x]].distance;
					}
				}

				//printf("\n  D  1  2  3\n"); //vypis matice
				//for (int i = 0; i < valid + 1; i++) {
				//	if (i == 0)
				//		printf("D ");
				//	else
				//		printf("%d ", i);
				//	for (int x = 0; x < valid + 1; x++) {
				//		printf("%d  ", matica[i][x]);
				//	}
				//	printf("\n");
				//}

				//riesenie princezien a ich ciest
				int* pomocna = (int*)malloc(sizeof(int) * (valid + 1));
				int* najkratsi = (int*)malloc(sizeof(int) * (valid + 1));
				int shortest = max, temp1 = 0;

				int* random = (int*)malloc(sizeof(int) * (valid + 1));

				for (int i = 0; i < valid + 1; i++) {
					random[i] = 0;
				}

				switch (valid) {
				case 5:
					for (int i = 1; i < valid + 1; i++) {
						if (random[i] == 1)
							continue;
						random[i] = 1;
						for (int x = 1; x < valid + 1; x++) {
							if (random[x] == 1)
								continue;
							random[x] = 1;
							for (int y = 1; y < valid + 1; y++) {
								if (random[y] == 1)
									continue;
								random[y] = 1;
								for (int a = 1; a < valid + 1; a++) {
									if (random[a] == 1)
										continue;
									random[a] = 1;
									for (int b = 1; b < valid + 1; b++) {
										if (random[b] == 1)
											continue;
										random[b] = 1;

										pomocna[0] = 0;
										pomocna[1] = i;
										pomocna[2] = x;
										pomocna[3] = y;
										pomocna[4] = a;
										pomocna[5] = b;

										for (int z = 0; z < valid; z++) {
											first = NULL;
											arrayInit(array, n, m);
											array[xova[pomocna[z]]][yova[pomocna[z]]].distance = 0;
											insertpriority(&array[xova[pomocna[z]]][yova[pomocna[z]]], &first);
											dijkstra(array, first, mapa, n, m);
											temp1 += array[xova[pomocna[z + 1]]][yova[pomocna[z + 1]]].distance;
											//printf("temp : %d\n", temp1);
										}
										if (temp1 < shortest) {
											najkratsi[0] = 0;
											najkratsi[1] = i;
											najkratsi[2] = x;
											najkratsi[3] = y;
											najkratsi[4] = a;
											najkratsi[5] = b;

											shortest = temp1;
										}
										temp1 = 0;

										//printf("0 %d %d %d %d %d\n", i, x, y, a, b);
										random[b] = 0;
									}
									random[a] = 0;
								}
								random[y] = 0;
							}
							random[x] = 0;
						}
						random[i] = 0;
					}
					break;
				case 4:
					for (int i = 1; i < valid + 1; i++) {
						if (random[i] == 1)
							continue;
						random[i] = 1;
						for (int x = 1; x < valid + 1; x++) {
							if (random[x] == 1)
								continue;
							random[x] = 1;
							for (int y = 1; y < valid + 1; y++) {
								if (random[y] == 1)
									continue;
								random[y] = 1;
								for (int a = 1; a < valid + 1; a++) {
									if (random[a] == 1)
										continue;
									random[a] = 1;

									pomocna[0] = 0;
									pomocna[1] = i;
									pomocna[2] = x;
									pomocna[3] = y;
									pomocna[4] = a;

									for (int z = 0; z < valid; z++) {
										first = NULL;
										arrayInit(array, n, m);
										array[xova[pomocna[z]]][yova[pomocna[z]]].distance = 0;
										insertpriority(&array[xova[pomocna[z]]][yova[pomocna[z]]], &first);
										dijkstra(array, first, mapa, n, m);
										temp1 += array[xova[pomocna[z + 1]]][yova[pomocna[z + 1]]].distance;
										//printf("temp : %d\n", temp1);
									}
									if (temp1 < shortest) {
										najkratsi[0] = 0;
										najkratsi[1] = i;
										najkratsi[2] = x;
										najkratsi[3] = y;
										najkratsi[4] = a;

										shortest = temp1;
									}
									temp1 = 0;

									//printf("0 %d %d %d %d \n", i, x, y, a);
									random[a] = 0;
								}
								random[y] = 0;
							}
							random[x] = 0;
						}
						random[i] = 0;
					}
					break;
				case 3:
					for (int i = 1; i < valid + 1; i++) {
						if (random[i] == 1)
							continue;
						random[i] = 1;
						for (int x = 1; x < valid + 1; x++) {
							if (random[x] == 1)
								continue;
							random[x] = 1;
							for (int y = 1; y < valid + 1; y++) {
								if (random[y] == 1)
									continue;
								random[y] = 1;

								pomocna[0] = 0;
								pomocna[1] = i;
								pomocna[2] = x;
								pomocna[3] = y;

								for (int z = 0; z < valid; z++) {
									first = NULL;
									arrayInit(array, n, m);
									array[xova[pomocna[z]]][yova[pomocna[z]]].distance = 0;
									insertpriority(&array[xova[pomocna[z]]][yova[pomocna[z]]], &first);
									dijkstra(array, first, mapa, n, m);
									temp1 += array[xova[pomocna[z + 1]]][yova[pomocna[z + 1]]].distance;
									//printf("temp : %d\n", temp1);
								}
								if (temp1 < shortest) {
									najkratsi[0] = 0;
									najkratsi[1] = i;
									najkratsi[2] = x;
									najkratsi[3] = y;

									shortest = temp1;
								}
								temp1 = 0;

								//printf("0 %d %d %d\n", i, x, y);
								random[y] = 0;
							}
							random[x] = 0;
						}
						random[i] = 0;
					}
					break;
				case 2:
					for (int i = 1; i < valid + 1; i++) {
						if (random[i] == 1)
							continue;
						random[i] = 1;
						for (int x = 1; x < valid + 1; x++) {
							if (random[x] == 1)
								continue;
							random[x] = 1;

							pomocna[0] = 0;
							pomocna[1] = i;
							pomocna[2] = x;

							for (int z = 0; z < valid; z++) {
								first = NULL;
								arrayInit(array, n, m);
								array[xova[pomocna[z]]][yova[pomocna[z]]].distance = 0;
								insertpriority(&array[xova[pomocna[z]]][yova[pomocna[z]]], &first);
								dijkstra(array, first, mapa, n, m);
								temp1 += array[xova[pomocna[z + 1]]][yova[pomocna[z + 1]]].distance;
								//printf("temp : %d\n", temp1);
							}
							if (temp1 < shortest) {
								najkratsi[0] = 0;
								najkratsi[1] = i;
								najkratsi[2] = x;

								shortest = temp1;
							}
							temp1 = 0;

							//printf("0 %d %d\n", i, x);
							random[x] = 0;
						}
						random[i] = 0;
					}
					break;
				case 1:
					najkratsi[0] = 0;
					najkratsi[1] = 1;
					//printf("0 1");
					break;
				}


				/*printf("najkratsia %d\n", shortest);
				for (int i = 0; i < valid; i++) {
					printf("%d ", najkratsi[i]);
				}
				printf("\n\n");*/

				for (int z = 0; z < valid; z++) { //vypis cesty pre princezne
					first = NULL;
					arrayInit(array, n, m);
					array[xova[najkratsi[z]]][yova[najkratsi[z]]].distance = 0;
					insertpriority(&array[xova[najkratsi[z]]][yova[najkratsi[z]]], &first);
					dijkstra(array, first, mapa, n, m);
					suradnicedraka(array, xova[najkratsi[z + 1]], yova[najkratsi[z + 1]], &counter, finalarray);
					//printf("temp : %d\n", temp1);
				}

				*dlzka_cesty = counter + 1;
				return finalarray;
			}
		}
	}
	else
		return NULL;
}

void test() { //nahodny test mapa 11X10
	int dlzkax = 11, dlzkay = 10, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCNHCCHNC";
	mapa[1] = "NNCCCHHCCCC";
	mapa[2] = "DNCCNNHHHPC";
	mapa[3] = "HNCNNNNNNNN";
	mapa[4] = "CCCCCNHHHHC";
	mapa[5] = "CCHCCCNNNNC";
	mapa[6] = "NNNNNHCCCCC";
	mapa[7] = "CCCCCCCCCCC";
	mapa[8] = "CCCNNHHHHHC";
	mapa[9] = "HHHPCCCPCCC";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test1() {//nahodny test mapa 10X10
	int dlzkax = 20, dlzkay = 10, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CCHCNHCCHNCPHCNHCCHN";
	mapa[1] = "NNCCCHHCCCNNCCCHHCCC";
	mapa[2] = "DNCCNNHHHCNNCCNNHHHC";
	mapa[3] = "HNCNNNNNNNCCCNNHHHHH";
	mapa[4] = "CCCCCNHHHHCCCCCNHHHH";
	mapa[5] = "CCHCCCNNNNCCCCCNHHHH";
	mapa[6] = "NNNNNHCCCCCCCCCNHHHH";
	mapa[7] = "CCCCCCCCCCCCCCCCCCCC";
	mapa[8] = "CCCNNHHHHHCCCCCNHHHH";
	mapa[9] = "HHHPCCCPCCCCHCCCNNNN";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test2() { //mapa 1X1
	int dlzkax = 1, dlzkay = 1, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "C";


	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test3() {//drak zavrety N
	int dlzkax = 10, dlzkay = 10, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCNHCCHN";
	mapa[1] = "NNCCCHHCCC";
	mapa[2] = "DNCCNNHHHC";
	mapa[3] = "NNCNNNNNNN";
	mapa[4] = "CCCCCNHHHH";
	mapa[5] = "CCHCCCNNNN";
	mapa[6] = "NNNNNHCCCC";
	mapa[7] = "CCCCCCCCCC";
	mapa[8] = "CCCNNHHHHH";
	mapa[9] = "HHHPCCCPCC";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test4() {//jedna z princezien zavreta N
	int dlzkax = 10, dlzkay = 10, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCNHCCHN";
	mapa[1] = "NNCCCHHCCC";
	mapa[2] = "DNCCNNHHHC";
	mapa[3] = "CNCNNNNNNN";
	mapa[4] = "CCCCCNHHHH";
	mapa[5] = "CCHCCCNNNN";
	mapa[6] = "NNNNNHCCCC";
	mapa[7] = "CCCCCCCCCC";
	mapa[8] = "CCCNNHHNHH";
	mapa[9] = "HHHPCCNPNC";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test5() {//malo casu na zabitie draka
	int dlzkax = 10, dlzkay = 10, cas = 10, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCNHCCHN";
	mapa[1] = "NNCCCHHCCC";
	mapa[2] = "DNCCNNHHHC";
	mapa[3] = "CNCNNNNNNN";
	mapa[4] = "CCCCCNHHHH";
	mapa[5] = "CCHCCCNNNN";
	mapa[6] = "NNNNNHCCCC";
	mapa[7] = "CCCCCCCCCC";
	mapa[8] = "CCCNNHHHHH";
	mapa[9] = "HHHPCCCPNC";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test6() {//iny pocet drakov ako 1
	int dlzkax = 10, dlzkay = 10, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCNHCCHN";
	mapa[1] = "NNCDCHHCCC";
	mapa[2] = "DNCCNNHHHC";
	mapa[3] = "CNCNNNNNNN";
	mapa[4] = "CCCCCNHHHH";
	mapa[5] = "CCHCCCNNNN";
	mapa[6] = "NNNNNHCCCC";
	mapa[7] = "CCCCCCCCCC";
	mapa[8] = "CCCNNHHHHH";
	mapa[9] = "HHHPCCCPNC";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test7() {//princezien iny pocet ako od 1 do 5
	int dlzkax = 10, dlzkay = 10, cas = 10, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCNHCCHN";
	mapa[1] = "NNCCCHHCCC";
	mapa[2] = "DNCCNNHHHC";
	mapa[3] = "CNCNNNNNNN";
	mapa[4] = "CCCCCNHHHH";
	mapa[5] = "CCHCCCNNNN";
	mapa[6] = "NNNNNHCCCC";
	mapa[7] = "CCCCCCCCCC";
	mapa[8] = "CCCNNHHHHH";
	mapa[9] = "HHHPCCCPNC";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test8() {//iba drak a princezna
	int dlzkax = 2, dlzkay = 1, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "DP";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

void test9() {//mapa ako rad
	int dlzkax = 20, dlzkay = 1, cas = 20, dlzka_cesty;
	int* finalarray;

	char** mapa = (char**)malloc(dlzkay * sizeof(char*));
	mapa[0] = "CPHCHHCCHDCCHCPHCCHN";

	for (int y = 0; y < dlzkay; y++) { //vypis mapy na lahsie kopirovanie do vizualizeru
		for (int x = 0; x < dlzkax; x++) {
			printf("%c", vzdialenost(mapa, x, y));
		}
		printf("\n");
	}
	finalarray = zachran_princezne(mapa, dlzkax, dlzkay, cas, &dlzka_cesty);
	for (int i = 0; i < dlzka_cesty; i++)
		printf("%d %d\n", finalarray[2 * i], finalarray[2 * i + 1]);
	free(mapa);
}

int main() {
	printf("test:\n");
	test();
	
	printf("test1:\n");
	test1();
	
	printf("test2:\n");
	test2();
	
	printf("test3:\n");
	test3();
	
	printf("test4:\n");
	test4();
	
	printf("test5:\n");
	test5();
	
	printf("test6:\n");
	test6();
	
	printf("test7:\n");
	test7();
	
	printf("test8:\n");
	test8();
	
	printf("test9:\n");
	test9();

	return 0;
}