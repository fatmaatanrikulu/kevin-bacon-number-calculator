#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Oyuncu bilgilerini tutan yapý 
struct performer {
	char firstname[50];
	char lastname[50];
	int bacon;
};
// Film bilgilerini tutan yapý 
struct movie {
	char name[100];
	char year[10];
	struct performer performers[100];
	long int check;
	long int level;
};
// Film listesini dosyadan çeken iþlev 
struct movie* getmovies(FILE* f, struct movie* m){
	int ch, i, k=0, j; // Ýterasyon deðiþkenleri 
	ch = fgetc(f);
	// Dosyadaki yazýnýn sonuna gelene kadar çalýþtýrýlacak döngü 
	while (ch != EOF){
		i=0;
		// Film adý bilgisini dosyadan çeken döngü 
		while(ch != '('){
			m[k].name[i] = ch;
			i++;
			ch = fgetc(f);
		}
		m[k].name[i] = '\0';
		ch = fgetc(f);
		i=0;
		// Film yýlý bilgisini dosyadan çeken döngü 
		while(ch != ')'){
			m[k].year[i] = ch;
			i++;
			ch = fgetc(f);
		}
		m[k].year[i] = '\0';
		ch = fgetc(f);
		j=0;
		// Filmde oynayan oyuncularýn bilgilerini dosyadan çeken döngü 
		while(ch != 10){
			ch = fgetc(f);
			i=0;
			// Ýlgili oyuncunun soyismini çeken döngü
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
			// Ýlgili oyuncunun ismini çeken döngü 
			while(ch != '/' && ch != 10){
				m[k].performers[j].firstname[i] = ch;
				i++;
				ch = fgetc(f);
				// Film içindeki oyuncularý döngüye alabilmek için son oyuncudan sonra gelen dizi deðiþkenine NULL atama iþlemi 
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
// Filmlerin Kevin Bacon'a olan uzaklýklarýný saptayan iþlev
struct movie* setbreadthfirstlevels(struct movie* m, int filmcount){
	// Ýlgili filmin iliþkili olduðu bir üst Kevin Bacon seviyesine ait oyuncuyu tutan deðiþkenler
	char rootfirstname[50];
	char rootlastname[50];
	int i, j=0, r, level=1, checked=0, subnetwork, k, l;
	struct movie* breadthfirstlist = (struct movie *)malloc(filmcount*sizeof(struct movie)); //Kevin Bacon sayýlarýna göre sýralanacak film listesi deðiþkeni
	struct movie temp;
	strcpy(rootfirstname, "Kevin");
	strcpy(rootlastname, "Bacon");
	// Kevin Bacon'un oynadýðý filmleri saptayan döngü 
	for(i=0; i<filmcount; i++){ // Listedeki her filmi dönecek döngü
		if(m[i].check==0){ // Ýlgili filmin daha önce Kevin Bacon sayýsýnýn atanýp atanmadýðýný kontrol eden koþul
			r=0; // Oyuncu iterasyonu 
			while(m[i].performers[r].firstname[0]!=0 && m[i].check==0){ // Ýlgili filmin tüm oyuncularýný arayan döngü
				if(strcmp(m[i].performers[r].firstname, rootfirstname) == 0 && strcmp(m[i].performers[r].lastname, rootlastname) == 0){
					breadthfirstlist[j] = m[i]; // Ýlgili oyuncu bulunduysa film Kevin Bacon sýralý listesine atanýr.
					breadthfirstlist[j].level = level; // Filmin Kevin Bacon'a uzaklýðý atanýr. 
					checked++; // Kontrol edilmiþ filmler sayýsýný tutan deðiþken arttýrýlýr. 
					m[i].check=1; // Film kontrol edildi olarak iþaretlenir. 
					j++; // Kevin Bacon sýralý filmler iterasyonu arttýrýlýr. 
				}
				r++; // Oyuncu iterasyonu arttýrýlýr. 
			}
		}
	}
	level++; // Kevin Bacon seviyesi arttýrýlýr
	l=0; // Kevin Bacon sýralý listesini dolaþmak için kullanýlacak iterasyon 
	// Kevin Bacon'un oynamadýðý geriye kalan filmleri kontrol etmek için oluþturulmuþ döngü
	while(checked < filmcount){
		subnetwork=0; // Aranan filmin Kevin Bacon alt aðýnda bulunup bulunmadýðýný kontrol eden yapý 
		// Kevin Bacon sýralý listesininin sadece Bacon'a eþit uzaklýkta bulunan filmleri için döngü
		while(breadthfirstlist[l].level == level-1){
			k=0; // Oyuncu iterasyonu 
			subnetwork=1; // Ýlgili fimin Kevin Bacon alt aðýnda olduðunu belirten satýr
			// Ýlgili filmin oyuncularýný arayan döngü 
			while(breadthfirstlist[l].performers[k].firstname[0] != 0){
				strcpy(rootfirstname, breadthfirstlist[l].performers[k].firstname); // Ýliþkisi aranacak oyuncu yapýlan isim atamasý 
				strcpy(rootlastname, breadthfirstlist[l].performers[k].lastname); // Ýliþkisi aranacak oyuncu yapýlan soyisim atamasý 
				// Ýlgili oyuncunun oynadýðý filmleri arayan döngü 
				for(i=0; i<filmcount; i++){
					if(m[i].check==0){ // Filmin, daha önce kontrol edilip edilmediðini sorgulayan koþul 
						r=0; // Oyuncu iterasyonu 
						// Ýlgili filmin oyuncularýný arayan döngü 
						while(m[i].performers[r].firstname[0]!=0){
							if(strcmp(m[i].performers[r].firstname, rootfirstname) == 0 && strcmp(m[i].performers[r].lastname, rootlastname) == 0){
								breadthfirstlist[j] = m[i]; // Ýlgili oyuncu bulunduysa film Kevin Bacon sýralý listesine atanýr.
								breadthfirstlist[j].level = level; // Filmin Kevin Bacon'a uzaklýðý atanýr. 
								checked++; // Kontrol edilmiþ filmler sayýsýný tutan deðiþken arttýrýlýr. 
								if(checked%200==0)
									printf("bakilan film sayisi : %d\n", checked);
								m[i].check=1;  // Film kontrol edildi olarak iþaretlenir. 
								j++; // Kevin Bacon sýralý filmler iterasyonu arttýrýlýr. 
							}
							r++; // Oyuncu itersayonu arttýrýlýr
						}
					}
				}
				k++; // Oyuncu itersayonu arttýrýlýr
			}
			l++; // Kevin Bacon'a olan uzaklýðý hesaplanmýþ film listesi iterasyonu arttýyyrýlýr.
		}
		if(subnetwork==0) // Eðer Kavin Bacon alt aðýnda bulunmayan filmler döngüde dönmeye baþlamýþsa döngüden çýkmayý saðlayan koþul 
			checked = filmcount;
		level++; // Kevin Bacon'a olan uzaklýðý belirleyen seviyenin arttýrýlmasý 
	}
	level = -1; // Kevin Bacon ile iliþkisi olmayan filmler için kullanýlacak seviye 
	for(i=0; i<filmcount; i++){ // Kevin Bacon ile iliþkisi olmayan filmler için oluþturulan döngü 
		if(m[i].check==0){ // Filmin, daha önce kontrol edilip edilmediðini sorgulayan koþul 
			breadthfirstlist[j] = m[i];  // Ýlgili oyuncu bulunduysa film Kevin Bacon sýralý listesine atanýr.
			breadthfirstlist[j].level = level;  // Filmin Kevin Bacon'a uzaklýðý atanýr. 
			checked++;  // Kontrol edilmiþ filmler sayýsýný tutan deðiþken arttýrýlýr. 
			m[i].check=1; // Film kontrol edildi olarak iþaretlenir. 
			j++; // Kevin Bacon sýralý filmler iterasyonu arttýrýlýr. 
		}
	}
	return breadthfirstlist; // Kevin Bacon'a olan uzaklýðýna göre sýralanmýþ film listesi döndürülür
}
// Filmlerin uzaklýklarýna göre oyuncularýn Kevin Bacon sayýlarýný saptayan iþlev
struct performer* getbaconnumbersofperformers(struct movie* m, int filmcount){
	int i, j, k, exist, show, count;
	long int performercount=0, checked;
	// Girilen film listesi dosyasýnda bulunan oyuncu sayýsý saptayan döngü (bilgi tekrarý dahil)
	for(i=0; i<filmcount; i++){
		j=0;
		while(m[i].performers[j].firstname[0]!=0){
			performercount++;
			j++;
		}
	}
	// Kontrol edilmiþ oyuncular için oluþturulan göstericiyi gösteren gösterici deðiþkeni için yapýlan dinamik bellek yönetimi 
	struct performer* performersforbfs;
	performersforbfs = (struct performer *)malloc(performercount*(sizeof(struct performer)));
	checked = 0; // Kontrol edilmiþ oyuncu sayýsý için oluþturulan deðiþken 
	strcpy(performersforbfs[0].firstname, "Kevin"); // Kontrol edilmiþ oyuncularýn ilk dizinine atanacak isim bilgisi
	strcpy(performersforbfs[0].lastname, "Bacon"); // Kontrol edilmiþ oyuncularýn ilk dizinine atanacak soyisim bilgisi
	performersforbfs[0].bacon = 0; // Kontrol edilmiþ oyuncularýn ilk dizinine atanacak Kevin Bacon sayýsý bilgisi
	checked++; // Kontrol edilmiþ oyuncu sayýsý arttýrýlýr. 
	performersforbfs[checked].firstname[0]=0; // Deðiþkenleri döngüye alabilmek için son oyuncudan sonra gelen dizi deðiþkenine NULL atama iþlemi 
	for(i=0; i<filmcount; i++){ // Bütün filmleri dönecek döngü 
		j=0; // Oyuncu iterasyonu 
		while(m[i].performers[j].firstname[0]!=0){ // Ýlgili filmin oyuncularýný dönecek döngü 
			k=0; // Kontrol edilmiþ oyuncu listesi iterasyonu 
			exist=0; // Ýlgili oyuncunun ilgili filmde içerip içermediði bilgisini tutan deðiþken 
			while(performersforbfs[k].firstname[0]!=0){ // Kontrol edilmiþ oyuncularý dönen döngü 
				// Ýlgili oyuncunun daha önce Kevin Bacon sayýsýnýn saptanýp saptanmadýðýný bulan koþul 
				if(strcmp(m[i].performers[j].firstname, performersforbfs[k].firstname)==0 
					&& strcmp(m[i].performers[j].lastname, performersforbfs[k].lastname)==0){
						k = checked;
						exist=1;
				}
				else{
					k++;
				}
			}
			// Eðer döngü sonunda ilgili oyuncunun Kevin Bacon sayýsý bulunmamýþsa çalýþacak yapý 
			// Kod bloðu içerisinde kontrol edilen oyuncunun isim, soyisim ve bacon sayýsý, kontrol edilen oyuncular listesine atanýr. 
			if(exist==0){
				strcpy(performersforbfs[checked].firstname, m[i].performers[j].firstname);
				strcpy(performersforbfs[checked].lastname, m[i].performers[j].lastname);
				performersforbfs[checked].bacon = m[i].level;
				checked++;
				if(checked%2000==0)
					printf("bakilan oyuncu sayisi : %d\n", checked);
				performersforbfs[checked].firstname[0]=0;
			}
			j++; // Oyuncu iterasyonun arttýrýr. 
		}
	}
	printf("Kevin Bacon sayisi 0 olan oyuncu sayisi: 1\n");
	show=1;
	i=1;
	count=0;
	// Kaç oyuncunun kaç Kevin Bacon sayýsýna sahip olduðunu ekrana yazan yapý
	while(performersforbfs[i].bacon!=-1){ // Tüm Kevin Bacon sayýsý kontrol edilmiþ oyuncu listesi için oluþturulan döngü 
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
	// Kevin Bacon sayýsý sonsuz olan oyuncular için oluþturulan döngü 
	while(performersforbfs[i].firstname[0]!=0){
		if(performersforbfs[i].bacon==-1){
			count++;
		}
		i++;
	}
	printf("Kevin Bacon sayisi sonsuz olan oyuncu sayisi: %d\n", count);
	// Ýþlev sonunda Kevin Bacon sayýsý hesaplanmýþ oyuncular listesi döndürülür. 
	return performersforbfs;
}
// Kullanýcýnýn girdiði oyuncunun Kevin Bacon ile olan iliþkisini ekrana yazdýran iþlev 
void printrelation(struct movie* m, struct performer* p, char* firstname, char* lastname){
	struct performer root; // Kullanýcýnýn girdiði oyuncu bilgisini tutmak için oluþturulan deðiþken 
	int i, include=0, j, exist=0, k;
	strcpy(root.firstname, firstname);
	strcpy(root.lastname, lastname);
	i=0;
	// Ýlgili oyuncunun bulunup bulunmadýðýný kontrol eden döngü
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
		// Ýlgili oyuncunun Kevin Bacon sayýsý sonsuz deðilse çalýþtýrýlacak döngüyü içeren komut
		if(root.bacon!=-1){
			// Ýliþkisi aranan oyuncu Kevin Bacon olmadýðý sürece çalýþtýrýlacak döngü
			while(strcmp(root.firstname, "Kevin")!=0 || strcmp(root.lastname, "Bacon")!=0){
				include=0; // Ýlgili oyuncunun ilgili filmde yer alýp almadýðýný kontrol eden deðiþken 
				i=0; // Film iterasyonu 
				while(include==0){
					j=0; // Oyuncu iterasyonu 
					while(m[i].performers[j].firstname[0]!=0){ // Ýlgili film içerisindeki oyuncularý dolaþacak olan döngü 
						// Ýlgili oyuncunun hangi filmde yer aldýðýný bulan döngü 
						if(strcmp(m[i].performers[j].firstname, root.firstname)==0 && strcmp(m[i].performers[j].lastname, root.lastname)==0){
							include++;
						}
						j++;
					}
					i++;
				}
				i--;
				j=0;
				// Ýliþkisi aranan oyuncunun Kevin Bacon'a daha yakýn olan bir oyuncuyla olan iliþkisini ekrana yazmak için çalýþtýralacak döngü 
				while(p[j].bacon<root.bacon && exist==0){
					k=0; // Ýliþkisi aranan oyuncunun bulunduðu film içerisindeki oyuncular için kullanýlan iterasyon 
					// Ýliþkisi aranan oyuncunun bulunduðu film içerisindeki oyuncular için kullanýlan döngü 
					while(m[i].performers[k].firstname[0]!=0 && exist==0){
						// Ýliþkisi aranan oyuncu ile Kevin Bacon seviyesi daha yüksek bir rol arkadaþýnýn karþýlaþtýrmasýný yapan koþul 
						if(strcmp(p[j].firstname, m[i].performers[k].firstname)==0 && strcmp(p[j].lastname, m[i].performers[k].lastname)==0){
							exist++;
							// Bulunan oyuncu, artýk yeni kök olmuþ ve artýk onun iliþkisi aranýyordur. 
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
	FILE *f; // Baðlantý kurulacak dosya için oluþturulan iþaretçi deðiþkeni
	char file_name[256]; // Baðlantý kurulacak dosya ismini tutacak deðiþken 
	char performer_name[100]; // Kullanýcýn gireceði oyuncu ismini tutacak deðiþken 
	char firstname[50], lastname[50]; // Kullanýcýn gireceði oyuncu isminin, isim ve soyisim olarak ayrý ayrý tutulacaðý deðiþkenler
	char temp, opt; // Kullanýcý giriþini döngüye almak için kullanýlan deðiþkenler 
	int flag=1;
	int ch, i, j; // Ýterasyon deðiþkenleri 
	long int filmcount=0; // Film sayýsýný tutacak deðiþken 
	struct movie* m; // Film listesini tutacak iþaretçi deðiþkeni 
	struct performer* p; // Oyuncu listesini tutacak iþaretçi deðiþkeni 
	// Kullanýcýdan baðlantý kurulacak dosya isminin alýmý 
	printf("Film listesini iceren giris dosyasinin ismi(txt) : ");
	gets(file_name); 
	// Dosya baðlantýsý 
	if ((f = fopen(file_name, "r")) == NULL) {
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	// Dosyadaki satýr sayýsýna göre film sayýsýný hesaplayan döngü 
	while ((ch=fgetc(f)) != EOF)
		if(ch==10)
			filmcount++;
	fclose(f);
	// Film sayýsýna göre film listesi bilgilerini tutan iþaretçi deðiþkenini geniþletme iþlemi 
	m = (struct movie *)malloc(filmcount*sizeof(struct movie));
	// Dosya baðlantýsý 
	if ((f = fopen(file_name, "r")) == NULL) {
		printf("cannot open file...\n");
		exit(EXIT_FAILURE);
	}
	// Film listesini dosyadan çekme iþlemi 
	m = getmovies(f, m);
	printf("Film listesi olusturuldu!\n\n");
	filmcount = m[0].level;
	// Filmlerin Kevin Bacon'a olan uzaklýklarýný belirleme iþlemi 
	m = setbreadthfirstlevels(m, filmcount);
	printf("Film uzakliklari belirlendi!\n\n");
	// Filmlerin uzaklýklarýna göre oyuncularýn Kevin Bacon sayýlarýný saptama iþlemi 
	p = getbaconnumbersofperformers(m, filmcount);
	printf("\nKevin Bacon numaralari olusturuldu!\n\n");
	// Kullanýcýnýn istediði kadar Kevin Bacon sayýsý hesaplamasý için oluþturulan döngü 
	while(flag)
    {
    	// Kullanýcýn girdiði oyuncu bilgisini isim ve soyisime ayrýþtýrma iþlemi 
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
		// Kullanýcýnýn girdiði oyuncunun Kevin Bacon ile olan iliþkisini ekrana yazdýran iþlev 
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
