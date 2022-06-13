#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Oyuncu bilgilerini tutan yap� 
struct performer {
	char firstname[50];
	char lastname[50];
	int bacon;
};
// Film bilgilerini tutan yap� 
struct movie {
	char name[100];
	char year[10];
	struct performer performers[100];
	long int check;
	long int level;
};
// Film listesini dosyadan �eken i�lev 
struct movie* getmovies(FILE* f, struct movie* m){
	int ch, i, k=0, j; // �terasyon de�i�kenleri 
	ch = fgetc(f);
	// Dosyadaki yaz�n�n sonuna gelene kadar �al��t�r�lacak d�ng� 
	while (ch != EOF){
		i=0;
		// Film ad� bilgisini dosyadan �eken d�ng� 
		while(ch != '('){
			m[k].name[i] = ch;
			i++;
			ch = fgetc(f);
		}
		m[k].name[i] = '\0';
		ch = fgetc(f);
		i=0;
		// Film y�l� bilgisini dosyadan �eken d�ng� 
		while(ch != ')'){
			m[k].year[i] = ch;
			i++;
			ch = fgetc(f);
		}
		m[k].year[i] = '\0';
		ch = fgetc(f);
		j=0;
		// Filmde oynayan oyuncular�n bilgilerini dosyadan �eken d�ng� 
		while(ch != 10){
			ch = fgetc(f);
			i=0;
			// �lgili oyuncunun soyismini �eken d�ng�
			while(ch != ','){
				m[k].check = 0;
				m[k].level = 0;
				m[k].performers[j].lastname[i] = ch;
				i++;
				ch = fgetc(f);
			}
			m[k].performers[j].lastname[i] = '\0';
			ch = fgetc(f);
			ch = fgetc(f);
			i=0;
			// �lgili oyuncunun ismini �eken d�ng� 
			while(ch != '/' && ch != 10){
				m[k].performers[j].firstname[i] = ch;
				i++;
				ch = fgetc(f);
				// Film i�indeki oyuncular� d�ng�ye alabilmek i�in son oyuncudan sonra gelen dizi de�i�kenine NULL atama i�lemi 
				if(ch==10){
					m[k].performers[j+1].firstname[0] = 0;
				}
			}
			m[k].performers[j].firstname[i] = '\0';
			m[k].performers[j].bacon = -1; 
			j++;
		}
		ch = fgetc(f);
		k++;
	}
	m[0].level=k;
	return m;
}
// Filmlerin Kevin Bacon'a olan uzakl�klar�n� saptayan i�lev
struct movie* setbreadthfirstlevels(struct movie* m, int filmcount){
	// �lgili filmin ili�kili oldu�u bir �st Kevin Bacon seviyesine ait oyuncuyu tutan de�i�kenler
	char rootfirstname[50];
	char rootlastname[50];
	int i, j=0, r, level=1, checked=0, subnetwork, k, l;
	struct movie* breadthfirstlist = (struct movie *)malloc(filmcount*sizeof(struct movie)); //Kevin Bacon say�lar�na g�re s�ralanacak film listesi de�i�keni
	struct movie temp;
	strcpy(rootfirstname, "Kevin");
	strcpy(rootlastname, "Bacon");
	// Kevin Bacon'un oynad��� filmleri saptayan d�ng� 
	for(i=0; i<filmcount; i++){ // Listedeki her filmi d�necek d�ng�
		if(m[i].check==0){ // �lgili filmin daha �nce Kevin Bacon say�s�n�n atan�p atanmad���n� kontrol eden ko�ul
			r=0; // Oyuncu iterasyonu 
			while(m[i].performers[r].firstname[0]!=0 && m[i].check==0){ // �lgili filmin t�m oyuncular�n� arayan d�ng�
				if(strcmp(m[i].performers[r].firstname, rootfirstname) == 0 && strcmp(m[i].performers[r].lastname, rootlastname) == 0){
					breadthfirstlist[j] = m[i]; // �lgili oyuncu bulunduysa film Kevin Bacon s�ral� listesine atan�r.
					breadthfirstlist[j].level = level; // Filmin Kevin Bacon'a uzakl��� atan�r. 
					checked++; // Kontrol edilmi� filmler say�s�n� tutan de�i�ken artt�r�l�r. 
					m[i].check=1; // Film kontrol edildi olarak i�aretlenir. 
					j++; // Kevin Bacon s�ral� filmler iterasyonu artt�r�l�r. 
				}
				r++; // Oyuncu iterasyonu artt�r�l�r. 
			}
		}
	}
	level++; // Kevin Bacon seviyesi artt�r�l�r
	l=0; // Kevin Bacon s�ral� listesini dola�mak i�in kullan�lacak iterasyon 
	// Kevin Bacon'un oynamad��� geriye kalan filmleri kontrol etmek i�in olu�turulmu� d�ng�
	while(checked < filmcount){
		subnetwork=0; // Aranan filmin Kevin Bacon alt a��nda bulunup bulunmad���n� kontrol eden yap� 
		// Kevin Bacon s�ral� listesininin sadece Bacon'a e�it uzakl�kta bulunan filmleri i�in d�ng�
		while(breadthfirstlist[l].level == level-1){
			k=0; // Oyuncu iterasyonu 
			subnetwork=1; // �lgili fimin Kevin Bacon alt a��nda oldu�unu belirten sat�r
			// �lgili filmin oyuncular�n� arayan d�ng� 
			while(breadthfirstlist[l].performers[k].firstname[0] != 0){
				strcpy(rootfirstname, breadthfirstlist[l].performers[k].firstname); // �li�kisi aranacak oyuncu yap�lan isim atamas� 
				strcpy(rootlastname, breadthfirstlist[l].performers[k].lastname); // �li�kisi aranacak oyuncu yap�lan soyisim atamas� 
				// �lgili oyuncunun oynad��� filmleri arayan d�ng� 
				for(i=0; i<filmcount; i++){
					if(m[i].check==0){ // Filmin, daha �nce kontrol edilip edilmedi�ini sorgulayan ko�ul 
						r=0; // Oyuncu iterasyonu 
						// �lgili filmin oyuncular�n� arayan d�ng� 
						while(m[i].performers[r].firstname[0]!=0){
							if(strcmp(m[i].performers[r].firstname, rootfirstname) == 0 && strcmp(m[i].performers[r].lastname, rootlastname) == 0){
								breadthfirstlist[j] = m[i]; // �lgili oyuncu bulunduysa film Kevin Bacon s�ral� listesine atan�r.
								breadthfirstlist[j].level = level; // Filmin Kevin Bacon'a uzakl��� atan�r. 
								checked++; // Kontrol edilmi� filmler say�s�n� tutan de�i�ken artt�r�l�r. 
								if(checked%200==0)
									printf("bakilan film sayisi : %d\n", checked);
								m[i].check=1;  // Film kontrol edildi olarak i�aretlenir. 
								j++; // Kevin Bacon s�ral� filmler iterasyonu artt�r�l�r. 
							}
							r++; // Oyuncu itersayonu artt�r�l�r
						}
					}
				}
				k++; // Oyuncu itersayonu artt�r�l�r
			}
			l++; // Kevin Bacon'a olan uzakl��� hesaplanm�� film listesi iterasyonu artt�yyr�l�r.
		}
		if(subnetwork==0) // E�er Kavin Bacon alt a��nda bulunmayan filmler d�ng�de d�nmeye ba�lam��sa d�ng�den ��kmay� sa�layan ko�ul 
			checked = filmcount;
		level++; // Kevin Bacon'a olan uzakl��� belirleyen seviyenin artt�r�lmas� 
	}
	level = -1; // Kevin Bacon ile ili�kisi olmayan filmler i�in kullan�lacak seviye 
	for(i=0; i<filmcount; i++){ // Kevin Bacon ile ili�kisi olmayan filmler i�in olu�turulan d�ng� 
		if(m[i].check==0){ // Filmin, daha �nce kontrol edilip edilmedi�ini sorgulayan ko�ul 
			breadthfirstlist[j] = m[i];  // �lgili oyuncu bulunduysa film Kevin Bacon s�ral� listesine atan�r.
			breadthfirstlist[j].level = level;  // Filmin Kevin Bacon'a uzakl��� atan�r. 
			checked++;  // Kontrol edilmi� filmler say�s�n� tutan de�i�ken artt�r�l�r. 
			m[i].check=1; // Film kontrol edildi olarak i�aretlenir. 
			j++; // Kevin Bacon s�ral� filmler iterasyonu artt�r�l�r. 
		}
	}
	return breadthfirstlist; // Kevin Bacon'a olan uzakl���na g�re s�ralanm�� film listesi d�nd�r�l�r
}
// Filmlerin uzakl�klar�na g�re oyuncular�n Kevin Bacon say�lar�n� saptayan i�lev
struct performer* getbaconnumbersofperformers(struct movie* m, int filmcount){
	int i, j, k, exist, show, count;
	long int performercount=0, checked;
	// Girilen film listesi dosyas�nda bulunan oyuncu say�s� saptayan d�ng� (bilgi tekrar� dahil)
	for(i=0; i<filmcount; i++){
		j=0;
		while(m[i].performers[j].firstname[0]!=0){
			performercount++;
			j++;
		}
	}
	// Kontrol edilmi� oyuncular i�in olu�turulan g�stericiyi g�steren g�sterici de�i�keni i�in yap�lan dinamik bellek y�netimi 
	struct performer* performersforbfs;
	performersforbfs = (struct performer *)malloc(performercount*(sizeof(struct performer)));
	checked = 0; // Kontrol edilmi� oyuncu say�s� i�in olu�turulan de�i�ken 
	strcpy(performersforbfs[0].firstname, "Kevin"); // Kontrol edilmi� oyuncular�n ilk dizinine atanacak isim bilgisi
	strcpy(performersforbfs[0].lastname, "Bacon"); // Kontrol edilmi� oyuncular�n ilk dizinine atanacak soyisim bilgisi
	performersforbfs[0].bacon = 0; // Kontrol edilmi� oyuncular�n ilk dizinine atanacak Kevin Bacon say�s� bilgisi
	checked++; // Kontrol edilmi� oyuncu say�s� artt�r�l�r. 
	performersforbfs[checked].firstname[0]=0; // De�i�kenleri d�ng�ye alabilmek i�in son oyuncudan sonra gelen dizi de�i�kenine NULL atama i�lemi 
	for(i=0; i<filmcount; i++){ // B�t�n filmleri d�necek d�ng� 
		j=0; // Oyuncu iterasyonu 
		while(m[i].performers[j].firstname[0]!=0){ // �lgili filmin oyuncular�n� d�necek d�ng� 
			k=0; // Kontrol edilmi� oyuncu listesi iterasyonu 
			exist=0; // �lgili oyuncunun ilgili filmde i�erip i�ermedi�i bilgisini tutan de�i�ken 
			while(performersforbfs[k].firstname[0]!=0){ // Kontrol edilmi� oyuncular� d�nen d�ng� 
				// �lgili oyuncunun daha �nce Kevin Bacon say�s�n�n saptan�p saptanmad���n� bulan ko�ul 
				if(strcmp(m[i].performers[j].firstname, performersforbfs[k].firstname)==0 
					&& strcmp(m[i].performers[j].lastname, performersforbfs[k].lastname)==0){
						k = checked;
						exist=1;
				}
				else{
					k++;
				}
			}
			// E�er d�ng� sonunda ilgili oyuncunun Kevin Bacon say�s� bulunmam��sa �al��acak yap� 
			// Kod blo�u i�erisinde kontrol edilen oyuncunun isim, soyisim ve bacon say�s�, kontrol edilen oyuncular listesine atan�r. 
			if(exist==0){
				strcpy(performersforbfs[checked].firstname, m[i].performers[j].firstname);
				strcpy(performersforbfs[checked].lastname, m[i].performers[j].lastname);
				performersforbfs[checked].bacon = m[i].level;
				checked++;
				if(checked%2000==0)
					printf("bakilan oyuncu sayisi : %d\n", checked);
				performersforbfs[checked].firstname[0]=0;
			}
			j++; // Oyuncu iterasyonun artt�r�r. 
		}
	}
	printf("Kevin Bacon sayisi 0 olan oyuncu sayisi: 1\n");
	show=1;
	i=1;
	count=0;
	// Ka� oyuncunun ka� Kevin Bacon say�s�na sahip oldu�unu ekrana yazan yap�
	while(performersforbfs[i].bacon!=-1){ // T�m Kevin Bacon say�s� kontrol edilmi� oyuncu listesi i�in olu�turulan d�ng� 
		if(performersforbfs[i].bacon!=show){
			printf("Kevin Bacon sayisi %d olan oyuncu sayisi: %d\n", show, count);
			count=0;
			show++;
		}
		count++;
		i++;
	}
	printf("Kevin Bacon sayisi %d olan oyuncu sayisi: %d\n", show, count);
	count=0;
	// Kevin Bacon say�s� sonsuz olan oyuncular i�in olu�turulan d�ng� 
	while(performersforbfs[i].firstname[0]!=0){
		if(performersforbfs[i].bacon==-1){
			count++;
		}
		i++;
	}
	printf("Kevin Bacon sayisi sonsuz olan oyuncu sayisi: %d\n", count);
	// ��lev sonunda Kevin Bacon say�s� hesaplanm�� oyuncular listesi d�nd�r�l�r. 
	return performersforbfs;
}
// Kullan�c�n�n girdi�i oyuncunun Kevin Bacon ile olan ili�kisini ekrana yazd�ran i�lev 
void printrelation(struct movie* m, struct performer* p, char* firstname, char* lastname){
	struct performer root; // Kullan�c�n�n girdi�i oyuncu bilgisini tutmak i�in olu�turulan de�i�ken 
	int i, include=0, j, exist=0, k;
	strcpy(root.firstname, firstname);
	strcpy(root.lastname, lastname);
	i=0;
	// �lgili oyuncunun bulunup bulunmad���n� kontrol eden d�ng�
	while((strcmp(p[i].firstname, root.firstname)!=0 || strcmp(p[i].lastname, root.lastname)!=0) && p[i].firstname[0]!=0){
		i++;
	}
	if(p[i].firstname[0]==0){
		printf("Oyuncu, giris dosyasinda bulunamadi!\n");
	}
	else{
		root.bacon=p[i].bacon;
		if(root.bacon==-1)
			printf("\n%s %s, Kevin Bacon sayisi : sonsuz\n", root.firstname, root.lastname);
		else
			printf("\n%s %s, Kevin Bacon sayisi : %d\n\n", root.firstname, root.lastname, root.bacon);
		j=0;
		// �lgili oyuncunun Kevin Bacon say�s� sonsuz de�ilse �al��t�r�lacak d�ng�y� i�eren komut
		if(root.bacon!=-1){
			// �li�kisi aranan oyuncu Kevin Bacon olmad��� s�rece �al��t�r�lacak d�ng�
			while(strcmp(root.firstname, "Kevin")!=0 || strcmp(root.lastname, "Bacon")!=0){
				include=0; // �lgili oyuncunun ilgili filmde yer al�p almad���n� kontrol eden de�i�ken 
				i=0; // Film iterasyonu 
				while(include==0){
					j=0; // Oyuncu iterasyonu 
					while(m[i].performers[j].firstname[0]!=0){ // �lgili film i�erisindeki oyuncular� dola�acak olan d�ng� 
						// �lgili oyuncunun hangi filmde yer ald���n� bulan d�ng� 
						if(strcmp(m[i].performers[j].firstname, root.firstname)==0 && strcmp(m[i].performers[j].lastname, root.lastname)==0){
							include++;
						}
						j++;
					}
					i++;
				}
				i--;
				j=0;
				// �li�kisi aranan oyuncunun Kevin Bacon'a daha yak�n olan bir oyuncuyla olan ili�kisini ekrana yazmak i�in �al��t�ralacak d�ng� 
				while(p[j].bacon<root.bacon && exist==0){
					k=0; // �li�kisi aranan oyuncunun bulundu�u film i�erisindeki oyuncular i�in kullan�lan iterasyon 
					// �li�kisi aranan oyuncunun bulundu�u film i�erisindeki oyuncular i�in kullan�lan d�ng� 
					while(m[i].performers[k].firstname[0]!=0 && exist==0){
						// �li�kisi aranan oyuncu ile Kevin Bacon seviyesi daha y�ksek bir rol arkada��n�n kar��la�t�rmas�n� yapan ko�ul 
						if(strcmp(p[j].firstname, m[i].performers[k].firstname)==0 && strcmp(p[j].lastname, m[i].performers[k].lastname)==0){
							exist++;
							// Bulunan oyuncu, art�k yeni k�k olmu� ve art�k onun ili�kisi aran�yordur. 
							printf("%s %s-%s %s : %s\n",root.firstname, root.lastname, p[j].firstname, p[j].lastname, m[i].name);
							strcpy(root.firstname, p[j].firstname);
							strcpy(root.lastname, p[j].lastname);
							root.bacon = p[j].bacon;
						}
						k++;
					}
					j++;
				}
				exist=0;
			}
		}
	}
}

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	FILE *f; // Ba�lant� kurulacak dosya i�in olu�turulan i�aret�i de�i�keni
	char file_name[256]; // Ba�lant� kurulacak dosya ismini tutacak de�i�ken 
	char performer_name[100]; // Kullan�c�n girece�i oyuncu ismini tutacak de�i�ken 
	char firstname[50], lastname[50]; // Kullan�c�n girece�i oyuncu isminin, isim ve soyisim olarak ayr� ayr� tutulaca�� de�i�kenler
	char temp, opt; // Kullan�c� giri�ini d�ng�ye almak i�in kullan�lan de�i�kenler 
	int flag=1;
	int ch, i, j; // �terasyon de�i�kenleri 
	long int filmcount=0; // Film say�s�n� tutacak de�i�ken 
	struct movie* m; // Film listesini tutacak i�aret�i de�i�keni 
	struct performer* p; // Oyuncu listesini tutacak i�aret�i de�i�keni 
	// Kullan�c�dan ba�lant� kurulacak dosya isminin al�m� 
	printf("Film listesini iceren giris dosyasinin ismi(txt) : ");
	gets(file_name); 
	// Dosya ba�lant�s� 
	if ((f = fopen(file_name, "r")) == NULL) {
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	// Dosyadaki sat�r say�s�na g�re film say�s�n� hesaplayan d�ng� 
	while ((ch=fgetc(f)) != EOF)
		if(ch==10)
			filmcount++;
	fclose(f);
	// Film say�s�na g�re film listesi bilgilerini tutan i�aret�i de�i�kenini geni�letme i�lemi 
	m = (struct movie *)malloc(filmcount*sizeof(struct movie));
	// Dosya ba�lant�s� 
	if ((f = fopen(file_name, "r")) == NULL) {
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	// Film listesini dosyadan �ekme i�lemi 
	m = getmovies(f, m);
	printf("Film listesi olusturuldu!\n\n");
	filmcount = m[0].level;
	// Filmlerin Kevin Bacon'a olan uzakl�klar�n� belirleme i�lemi 
	m = setbreadthfirstlevels(m, filmcount);
	printf("Film uzakliklari belirlendi!\n\n");
	// Filmlerin uzakl�klar�na g�re oyuncular�n Kevin Bacon say�lar�n� saptama i�lemi 
	p = getbaconnumbersofperformers(m, filmcount);
	printf("\nKevin Bacon numaralari olusturuldu!\n\n");
	// Kullan�c�n�n istedi�i kadar Kevin Bacon say�s� hesaplamas� i�in olu�turulan d�ng� 
	while(flag)
    {
    	// Kullan�c�n girdi�i oyuncu bilgisini isim ve soyisime ayr��t�rma i�lemi 
    	printf("Kevin Bacon sayisi ve baglantisi istenen oyuncu ismi : ");
		gets(performer_name);
		i=0;
        while(performer_name[i]!=' '){
			firstname[i] = performer_name[i];
			i++;
		}
		firstname[i] = '\0';
		j=0;
		i++;
		while(performer_name[i]!='\0'){
			lastname[j] = performer_name[i];
			i++;
			j++;
		}
		lastname[j] = '\0';
		// Kullan�c�n�n girdi�i oyuncunun Kevin Bacon ile olan ili�kisini ekrana yazd�ran i�lev 
		printrelation(m, p, firstname, lastname);
		printf("\nDevam etmek icin enter'a; cikmak icin x'e basin\n");
    	while((temp=getchar()) != '\n') {
    		if(temp == 'x' | temp == 'X')
    		{
    			flag = 0;
    			opt = temp;
    		}
    	}
    }
	fclose(f);
	
	return 0;
}
