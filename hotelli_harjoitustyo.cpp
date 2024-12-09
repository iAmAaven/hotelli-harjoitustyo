// Aapo Harjunpää
// Harjoitustyö - Hotellihuoneen varausohjelma (5p)


#define NOMINMAX

// Lisätään tarvittavat kirjastot
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <array>
#include <limits>
#include <locale>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>

using namespace std; // Mahdollistaa std-komentojen käytön ilman std::-etuliitettä
using namespace this_thread;	// Mahdollistavat ajastuskomennot,
using namespace chrono;			// kuten sleep_for() -funktion

// Luodaan globaalit vakiomuuttujat
const int MAKS_HUONEET = 300, MIN_HUONEET = 40;

// Etunimipankin tiedot ovat top 18 nimipalvelu.dvv.fi -sivuston suosituimpien etunimien listalta 2020-vuosikymmenen ajalta
	// Sukunimet ovat saman sivuston "Yleisimmät sukunimet" -listalta
// Näitä vektoreita käytetään satunnaisesti luotujen huonevarausten varaajien nimen luomisessa
// Etunimiä ja sukunimiä on kumpiakin 18, koska maksimi huoneiden määrä on 300.
	// 300:n neliöjuuri on 17,32, jolloin 18 on seuraava kokonaisluku.
	// Eri nimiyhdistelmiä saadaan siis enintään 324 kpl (18*18),
	// jolloin varaajan nimi ei voi olla kahdessa eri huoneessa sama.
const vector<string> etunimipankki = {
	"Olavi", "Juhani", "Johannes", "Mikael", "Oliver", "Ilmari", "Onni", "Elias", "Eino",
	"Maria", "Sofia", "Aurora", "Olivia", "Emilia", "Aino", "Matilda", "Linnea", "Ellen",
};
const vector<string> sukunimipankki = {
	"Korhonen", "Virtanen", "Mäkinen", "Nieminen", "Mäkelä", "Hämäläinen", "Laine", "Heikkinen", "Koskinen",
	"Järvinen", "Lehtonen", "Lehtinen", "Saarinen", "Niemi", "Salminen", "Heinonen", "Heikkilä", "Kinnunen",
};

// Tehdään struct hotellihuoneelle, joka pitää sisällään...
struct Hotellihuone {
	int huoneen_numero = 0; //...huoneen numeron
	int huonetyyppi = 0; //...huonetyypin, eli kuinka monen hengen huone (1 tai 2)
	bool varaustila = false; //...varaustilan: true = varattu   ,   flase = vapaa
	int varausnumero = 1; //...varausnumeron (satunnaisesti valittu 10000:n ja 99999:n välillä
	string varaajan_nimi = ""; //...varaajan nimen
	int oidenMaara = 0; //...öiden määrän
	int hintaPerYo = 0; //...hinnan per yö (100€ 1hh ja 150€ 2hh)
	double varauksenHinta = 0; //...sekä varauksen lopullisen hinnan
};

// DEKLAROIDAAN FUNKTIOT
void paavalikko(vector<Hotellihuone>& huoneet);
void varaaHuone(vector<Hotellihuone>& hotellihuoneet);
int valitseHuoneItse(vector<Hotellihuone>& hotellihuoneet, int huonetyyppi);
int valitseViimeinenVapaaHuone(vector<Hotellihuone>& hotellihuoneet, int huoneenKoko);
void tutkiVarausta(Hotellihuone& huone, int rivinLisays);
void luoHotellihuoneet(vector<Hotellihuone>& huoneet);
int satunnainenVarausnumero();
double varauksenHinta(int oidenMaara, int hintaPerYo);
double kayttajanVarauksenHinta(int oidenMaara, int hintaPerYo);
int vapaatHuonetyypit(vector<Hotellihuone>& huoneet, int tyyppi);
void tulostaVapaatHuoneet(vector<Hotellihuone>& hotellihuoneet, int huoneenKoko, vector<int>& vapaatHuonenumerot);
void puhekupla();
void vastaanottoIloinen();
void vastaanottoVihainen();
void dialogi(vector<string> rivit);
void puhdistaDialogi();
void odotaVastausta();
int kaksiVaihtoehtoa(string vaihtoehto1, string vaihtoehto2);
int kolmeVaihtoehtoa(string vaihtoehto1, string vaihtoehto2, string vaihtoehto3);
void puhdistaVastaus();
void alustaTerminaali();
void sijainti(int x, int y);
void piilotaKursori();
void naytaKursori();
string kayttajanNimi();

int main() // Ohjelman pääfunktio, josta suoritus alkaa
{
	setlocale(LC_ALL, "FI_fi"); // Mahdollistaa ääkköset
	srand(time(0)); // Mahdollistaa satunnaislukujen arpomisen

	piilotaKursori(); // Piilottaa vilkkuvan tekstikursorin terminaalista

	// Luodaan kokonaislukumuuttuja, joka määrittää satunnaisesti huoneiden parillisen määrän
	int huoneiden_maara = 0;
	do {
		huoneiden_maara = rand() % (MAKS_HUONEET - MIN_HUONEET + 1) + MIN_HUONEET;
	} while (huoneiden_maara % 2 != 0);

	vector<Hotellihuone> hotellihuoneet(huoneiden_maara); // Luodaan vektori hotellihuoneille ja annetaan huoneiden arvottu määrä
	luoHotellihuoneet(hotellihuoneet); // Luodaan satunnaisesti hotellihuoneille asiakastiedot
	
	paavalikko(hotellihuoneet); // Aloitetaan varsinainen ohjelma kutsumalla päävalikko-funktio
	
	dialogi({ "Turvallista kotimatkaa!" }); // Kun päävalikosta (eli ohjelmasta) poistutaan, tulostetaan vastaanottajalle viimeinen dialogi

	return EXIT_SUCCESS; // Palauttaa arvon osoittaen onnistuneen suorituksen
}

// Aliohjelma, joka sisältää päävalikon ja johon aliohjelmista palataan
void paavalikko(vector<Hotellihuone>& huoneet)
{
	for (int i = 0;;) // Aloitetaan "loputon" silmukka, joka loppuu vasta sitten, kun käyttäjä päättää poistua ohjelmasta.
	{
		// Tavoiteltu ulkoasu päävalikolle:
		// 
		//		Moro! Haluan...
		// 
		//		>  varata huoneen
		//		   tutkia varausta
		//		   kotiin
		//
		
		alustaTerminaali();
		dialogi({ "Tervetuloa aaventen hotelliin!", "" ,"Miten voin olla avuksi?" });

		sijainti(5, 14); cout << "Moro! Haluan...";

		int valinta = kolmeVaihtoehtoa("varata huoneen", "tutkia varausta", "kotiin");

		puhdistaVastaus();

		if (valinta == 1) // Jos käyttäjä haluaa varata huoneen...
		{
			varaaHuone(huoneet); //...kutsutaan siihen tarkoitettu aliohjelma
		}
		else if (valinta == 2)
		{
			dialogi({ "Voitte antaa minulle varausnumeron", "tai varaajan nimen, jos haluat", "tutkia jokaista sillä nimellä tehtyä", "varausta." });
			int tutkintaValinta = 0;
			tutkintaValinta = kaksiVaihtoehtoa("Minulla on varausnumero", "Haluan tutkia nimelläni tekemiä varauksia");
			puhdistaVastaus();

			if (tutkintaValinta == 1)
			{
				int varausnroSyote = 0;

				while (true)
				{
					dialogi({ "Selvä!", "Anna varausnumero." });
					naytaKursori();
					sijainti(5, 14); cout << "Varausnumeroni on...";
					sijainti(8, 16); cin >> varausnroSyote;
					puhdistaVastaus();
					piilotaKursori();

					bool varausLoydetty = false;

					if (cin.fail())
					{
						cin.clear(); // Tyhjennä virhetila
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						puhdistaVastaus();
					}
					else
					{
						for (Hotellihuone huone : huoneet)
						{
							if (huone.varaustila == true && huone.varausnumero == varausnroSyote)
							{
								varausLoydetty = true;
								int kiitosValinta = 0;
								dialogi({ "Aah! Tässä sinun varauksesi." });
								tutkiVarausta(huone, 0);
								kiitosValinta = kaksiVaihtoehtoa("Kiitos!", "Kiitos paljon!");
								puhdistaVastaus();
								break;
							}
						}
						if (varausLoydetty == true)
							break;
						else
						{
							dialogi({ "En löytänyt varausta tuolla", "numerolla. Haluatko yrittää", "uudelleen?" });
							int uudelleenValinta = 0;
							uudelleenValinta = kaksiVaihtoehtoa("Kyllä", "En");
							puhdistaVastaus();

							if (uudelleenValinta == 2)
								break;
						}
					}
				}
			}
			else // Käyttäjä haluaa tutkia varauksia nimen perusteella
			{
				string nimiSyote = "";

				while (true)
				{
					dialogi({ "Antakaa minulle varaajan nimi,", "kiitos."});
					bool varausLoydetty = false;
					naytaKursori();
					sijainti(5, 14); cout << "Varaajan nimi on...";
					sijainti(8, 16); getline(cin, nimiSyote); // Yritä ottaa käyttäjän syöte
					puhdistaVastaus();

					if (cin.fail() || nimiSyote.empty()) // Jos syötteessä on virhe tai se on tyhjä
					{
						cin.clear(); // Tyhjennä virhetila
						cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennä puskurin sisältö
						puhdistaVastaus(); // Tyhjennä käyttäjän näkymä
					}
					else
					{
						piilotaKursori();
						int riviLisays = 0;
						for (Hotellihuone huone : huoneet)
						{
							if (huone.varaustila == true && huone.varaajan_nimi == nimiSyote)
							{
								if (riviLisays == 0)
								{
									dialogi({ "Aah! Tässä teidän varauksenne." });
									sleep_for(seconds(1));
								}

								varausLoydetty = true;
								tutkiVarausta(huone, riviLisays);
								riviLisays += 10;
							}
						}
						
						sleep_for(seconds(1));

						if (varausLoydetty == true)
						{
							int kiitosValinta = 0;
							kiitosValinta = kaksiVaihtoehtoa("Kiitos!", "Kiitos paljon!");
							break;
						}
						else
						{
							dialogi({ "En löytänyt varausta tuolla", "nimellä. Haluatko yrittää", "uudelleen?" });
							int uudelleenValinta = 0;
							uudelleenValinta = kaksiVaihtoehtoa("Kyllä", "En");
							puhdistaVastaus();

							if (uudelleenValinta == 2)
								break;
						}
					}
				}
			}
		}
		else
			break;
	}
}

void varaaHuone(vector<Hotellihuone>& hotellihuoneet)
{
	if (vapaatHuonetyypit(hotellihuoneet, 1) == 0 && vapaatHuonetyypit(hotellihuoneet, 2) == 0)
	{
		dialogi({"Meillä ei ole enää vapaita", "huoneita. Pahoittelut."});
		int selva = 0;
		selva = kaksiVaihtoehtoa("Selvä", "Asia selvä");
		return;
	}

	int huoneenKoko = 1;

	dialogi({"Kuinka monen hengen huonetta etsitte?"});
	
	sijainti(5, 14); cout << "Etsin...";
	huoneenKoko = kaksiVaihtoehtoa("yhden hengen huonetta", "kahden hengen huonetta");

	if (vapaatHuonetyypit(hotellihuoneet, huoneenKoko) == 0)
	{
		dialogi({ "Valitettavasti meillä ei ole", "enää vapaita " + to_string(huoneenKoko) + ":n hengen huoneita." });
		int selva = 0;
		selva = kaksiVaihtoehtoa("Selvä", "Asia selvä");
		return;
	}

	puhdistaVastaus();

	dialogi({ "Meillä on " + to_string(vapaatHuonetyypit(hotellihuoneet, huoneenKoko)) + " "
		+ to_string(huoneenKoko) + ":n hengen huonetta vapaa-", "na.", "", "Haluatteko valita huoneen itse?", "", "" });

	int valinta = kaksiVaihtoehtoa("Kyllä", "En");
	puhdistaVastaus();

	int huoneNroSyote = 0;

	//

	if (valinta == 1)
	{
		huoneNroSyote = valitseHuoneItse(hotellihuoneet, huoneenKoko);
	}
	else
	{
		huoneNroSyote = valitseViimeinenVapaaHuone(hotellihuoneet, huoneenKoko);
		int asiaSelv = 0;
		asiaSelv = kaksiVaihtoehtoa("Selvä", "Asia selvä");
		puhdistaVastaus();
	}

	//

	int olikoOikeaNimi = 0;
	string varaajanNimiSyote = "";

	do {
		alustaTerminaali();
		varaajanNimiSyote = kayttajanNimi();
		dialogi({ varaajanNimiSyote + "?", "Sainko nimen oikein?" });
		olikoOikeaNimi = kaksiVaihtoehtoa("Kyllä", "Et, nimi on...");
	} while (olikoOikeaNimi != 1);


	puhdistaVastaus();
	dialogi({"Selvä!", "Kuinka monta yötä haluatte viettää", "hotellissamme?", "Varauksen enimmäispituus on", "365 päivää."});

	int oidenMaaraSyote;

	while (true)
	{
		naytaKursori();
		sijainti(5, 14); cout << "Kuinka monta yötäkö?";
		sijainti(8, 16); cin >> oidenMaaraSyote;
		piilotaKursori();

		if (cin.fail() || oidenMaaraSyote < 1 || oidenMaaraSyote > 365)
		{
			cin.clear(); // Tyhjennä virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			puhdistaVastaus();
		}
		else break;
	}

	alustaTerminaali();
	dialogi({to_string(oidenMaaraSyote) + " yötä siis!", "Odottakaa, kun lasken varauksenne hinnan.", "...", "...", "..."});

	double hinta = kayttajanVarauksenHinta(oidenMaaraSyote, hotellihuoneet[huoneNroSyote-1].hintaPerYo);

	ostringstream hintaStream;
	hintaStream.precision(2); // Määritetään tarkkuus kahteen desimaaliin
	hintaStream << fixed << hinta;

	dialogi({ "Varauksen lopullinen hinta on","", hintaStream.str() + " euroa." });

	sleep_for(seconds(2));

	dialogi({ "Oletteko tyytyväinen varaukseenne", "vai haluatteko peruuttaa sen?" });
	int tyytyvaisyys = kaksiVaihtoehtoa("Olen tyytyväinen", "Haluan perua varauksen");
	puhdistaVastaus();

	if (tyytyvaisyys == 1)
	{
		dialogi({ "Hienoa!", "Maksakaa " + hintaStream.str() + " euroa,", "niin saatte avaimen."});
		
		int maksatko = 0;
		maksatko = kaksiVaihtoehtoa("Tässä " + hintaStream.str() + " euroa, olkaa hyvä", "En halua maksaa");
		puhdistaVastaus();

		if (maksatko == 1)
		{
			while (true)
			{
				bool varausnumeroKaytossa = false;
				int varausnro = 0;
				varausnro = satunnainenVarausnumero();
			
				for (Hotellihuone huone : hotellihuoneet)
				{
					if (huone.varaustila == true && huone.varausnumero == varausnro)
					{
						varausnumeroKaytossa = true;
						break;
					}
				}
				if (varausnumeroKaytossa == false)
				{
					hotellihuoneet[huoneNroSyote - 1].varaustila = true;
					hotellihuoneet[huoneNroSyote - 1].varausnumero = varausnro;
					hotellihuoneet[huoneNroSyote - 1].varaajan_nimi = varaajanNimiSyote;
					hotellihuoneet[huoneNroSyote - 1].oidenMaara = oidenMaaraSyote;
					hotellihuoneet[huoneNroSyote - 1].varauksenHinta = hinta;

					break;
				}
			}

			dialogi({ "Kiitos varauksestanne!", "", "Tässä vielä varauksesi tiedot."});
			tutkiVarausta(hotellihuoneet[huoneNroSyote - 1], 0);
			int varmistus;
			varmistus = kaksiVaihtoehtoa("Kiitos!", "Kiitos paljon!");
			alustaTerminaali();
		}
		else
		{
			vastaanottoVihainen();
			dialogi({"Ei sitten."});
			int poistut = 0;
			poistut = kaksiVaihtoehtoa("Ei niin", "Hmm...");
			puhdistaVastaus();
		}
	}
	else
	{
		vastaanottoVihainen();
		dialogi({ "Asia harvinaisen selvä.", "Haluatteko tehdä eri varauksen?" });
		int paatos = kaksiVaihtoehtoa("Kyllä", "En");
		puhdistaVastaus();

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (paatos == 1)
			varaaHuone(hotellihuoneet);
		else
			return;
	}
	
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int valitseHuoneItse(vector<Hotellihuone>& hotellihuoneet, int huoneenKoko)
{
	dialogi({ "Selvä!", "", "Tässä lista vapaista huoneistamme.", "Kerro vain haluamasi huoneen numero ja", "minä hoidan loput!"});

	vector<int> vapaatHuonenumerot;

	int huoneNroSyote;

	while(true)
	{
		naytaKursori();
		tulostaVapaatHuoneet(hotellihuoneet, huoneenKoko, vapaatHuonenumerot);

		sijainti(5, 14); cout << "Haluan huoneen nro...";
		sijainti(8, 16); cin >> huoneNroSyote;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		piilotaKursori();

		if (cin.fail() || find(vapaatHuonenumerot.begin(), vapaatHuonenumerot.end(), huoneNroSyote) == vapaatHuonenumerot.end())
		{
			cin.clear(); // Tyhjennä virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			puhdistaVastaus();
		}
		else break;
	}

	dialogi({"Asia selvä! Huone " + to_string(huoneNroSyote) + " siis!"});
	sleep_for(seconds(2));
	return huoneNroSyote;
}

string kayttajanNimi()
{
	dialogi({ "Millä nimellä varaus tulee,", "arvon ihminen?", });

	string nimiSyote = "";

	while (true)
	{
		naytaKursori();
		sijainti(5, 14); cout << "Varaus tulee nimellä...";
		sijainti(8, 16);

		getline(cin, nimiSyote); // Yritä ottaa käyttäjän syöte

		if (cin.fail() || nimiSyote.empty()) // Jos syötteessä on virhe tai se on tyhjä
		{
			cin.clear(); // Tyhjennä virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennä puskurin sisältö
			puhdistaVastaus(); // Tyhjennä käyttäjän näkymä
		}
		else break; // Syöte on kunnossa, lopeta silmukka
	}
	piilotaKursori();
	alustaTerminaali();

	return nimiSyote;
}

void tulostaVapaatHuoneet(vector<Hotellihuone>& hotellihuoneet, int huoneenKoko, vector<int>& vapaatHuonenumerot)
{
	int kuinkaMonesHuone = 0, kuinkaMonesRivi = 0, valiLisays = 0;

	for (Hotellihuone huone : hotellihuoneet)
	{
		if (huone.huonetyyppi == huoneenKoko && huone.varaustila == false)
		{
			vapaatHuonenumerot.push_back(huone.huoneen_numero);

			kuinkaMonesHuone++;
			sijainti(38 + valiLisays, 12 + kuinkaMonesRivi); cout << huone.huoneen_numero;

			valiLisays += 4;

			if (kuinkaMonesHuone % 10 == 0)
			{
				kuinkaMonesRivi++;
				valiLisays = 0;
			}
		}
	}
}

int valitseViimeinenVapaaHuone(vector<Hotellihuone>& hotellihuoneet, int huoneenKoko)
{
	int huoneNro = 0;
	for (Hotellihuone huone : hotellihuoneet)
	{
		if (huone.huonetyyppi == huoneenKoko && huone.varaustila == false)
		{
			huoneNro = huone.huoneen_numero;
		}
	}

	dialogi({ "Teidän huoneenne numero on ", to_string(huoneNro) });
	return huoneNro;
}

void tutkiVarausta(Hotellihuone& huone, int rivinLisays)
{
	double hinta = huone.varauksenHinta;

	ostringstream hintaStream;
	hintaStream.precision(2); // Määritetään tarkkuus kahteen desimaaliin
	hintaStream << fixed << hinta;
	
	sijainti(40, 14 + rivinLisays);
	cout << "Varaajan nimi: " << huone.varaajan_nimi << endl;
	cout << string(40, ' ') << endl;
	cout << string(40, ' ') << endl;
	cout << string(40, ' ') << "Varaus: " << huone.varausnumero << endl;
	cout << string(40, ' ') << "Huone: " << huone.huoneen_numero << ", tyyppi " << huone.huonetyyppi << "hh" << endl;
	cout << string(40, ' ') << "Öiden määrä: " << huone.oidenMaara << endl;
	cout << string(40, ' ') << "Hinta: " << hintaStream.str() << " euroa";
	cout << string(40, ' ') << endl;
	cout << string(40, ' ') << endl;
	cout << string(40, ' ') << endl;
}

void luoHotellihuoneet(vector<Hotellihuone>& huoneet)
{
	int huonenumero = 1;
	vector<string> kaytetytNimet;
	vector<int> kaytetytVarausnumerot;

	// Käydään läpi kaikki hotellin huoneet ja annetaan jokaiselle satunnaisesti arvotut tiedot
	for (Hotellihuone& huone : huoneet)
	{
		// Annetaan huoneelle numero ja huonetyyppi (1:n tai 2:n hengen huone)
		huone.huoneen_numero = huonenumero;
		if (huonenumero <= huoneet.size() / 2)
		{
			huone.huonetyyppi = 1;
			huone.hintaPerYo = 100;
		}
		else
		{
			huone.huonetyyppi = 2;
			huone.hintaPerYo = 150;
		}

		huonenumero++;

		// Arvotaan 50%:n mahdollisuudella, onko huone varattu vai ei
		int satunnainen = rand() % 100 + 1;
		if (satunnainen <= 50)
			huone.varaustila = true;
		else
			huone.varaustila = false;

		if (huone.varaustila) // Jos huone on varattu...
		{
			string nimi = "";
			do { //...luodaan varaajan nimi arpomalla nimet nimipankeista, ja varmistetaan, ettei varaajan nimi toistu
				string etunimi = etunimipankki[rand() % 17];
				string sukunimi = sukunimipankki[rand() % 17];
				nimi = etunimi + " " + sukunimi;
			} while (find(kaytetytNimet.begin(), kaytetytNimet.end(), nimi) != kaytetytNimet.end());

			huone.varaajan_nimi = nimi; // Lisätään luotu nimi huoneen varaajan nimeksi
			kaytetytNimet.push_back(nimi);

			int varausnro = 0;
			do {
				varausnro = satunnainenVarausnumero();
			} while (find(kaytetytVarausnumerot.begin(), kaytetytVarausnumerot.end(), varausnro) != kaytetytVarausnumerot.end());

			huone.varausnumero = varausnro;
			kaytetytVarausnumerot.push_back(varausnro);

			int oidenMaara = rand() % 28 + 1;
			huone.oidenMaara = oidenMaara;
			huone.varauksenHinta = varauksenHinta(oidenMaara, huone.hintaPerYo);
		}
	}
}

int satunnainenVarausnumero()
{
	return rand() % (99999 - 10000 + 1) + 10000;
}

double varauksenHinta(int oidenMaara, int hintaPerYo)
{
	int satunnaisLuku = rand() % 3;
	double lopullinenProsentti = (10 - satunnaisLuku) / 10.0;
	return lopullinenProsentti * oidenMaara * hintaPerYo;
}

double kayttajanVarauksenHinta(int oidenMaara, int hintaPerYo)
{
	int satunnaisLuku = rand() % 3;
	double lopullinenProsentti = (10 - satunnaisLuku) / 10.0;

	if (satunnaisLuku != 0)
	{
		dialogi({ "Onnittelut!", "Sait varaukseesi " + to_string(satunnaisLuku * 10) + "% alennusta!" });
		sleep_for(seconds(2));
	}

	return lopullinenProsentti * oidenMaara * hintaPerYo;
}

int vapaatHuonetyypit(vector<Hotellihuone>& huoneet, int tyyppi)
{
	int palautus = 0;
	for (Hotellihuone& huone : huoneet)
		if (huone.huonetyyppi == tyyppi && huone.varaustila == false)
			palautus++;

	return palautus;
}

void puhekupla()
{
	// Tekstikenttä on x = 10-49, y = 4-9
	sijainti(5, 2);  cout << " ________________________________________________      ";
	sijainti(5, 3);  cout << "||                                              ||     ";
	sijainti(5, 4);  cout << "||                                              ||     ";
	sijainti(5, 5);  cout << "||                                              ||     ";
	sijainti(5, 6);  cout << "||                                              ||     ";
	sijainti(5, 7);  cout << "||                                              ''=--__";
	sijainti(5, 8);  cout << "||                                                  _/ ";
	sijainti(5, 9);  cout << "||                                                _/   ";
	sijainti(5, 10); cout << "||_______________________________________________/     ";

	vastaanottoIloinen();
}

void vastaanottoIloinen()
{
	sijainti(62, 4);  cout << "  @@@@@@@@     ";
	sijainti(62, 5);  cout << " /      @@@@   ";
	sijainti(62, 6);  cout << "|  ^  ^   @@   ";
	sijainti(62, 7);  cout << " \\ '--'   /    ";
	sijainti(62, 8);  cout << "  =-ooo--=     ";
	sijainti(62, 9);  cout << " /   0     \\   ";
	sijainti(62, 10); cout << "|   000     |  ";
}

void vastaanottoVihainen()
{
	sijainti(62, 4);  cout << "  @@@@@@@@     ";
	sijainti(62, 5);  cout << " / _  _ @@@@   ";
	sijainti(62, 6);  cout << "|  U  U   @@   ";
	sijainti(62, 7);  cout << " \\  -     /    ";
	sijainti(62, 8);  cout << "  =-ooo--=     ";
	sijainti(62, 9);  cout << " /   0     \\   ";
	sijainti(62, 10); cout << "|   000     |  ";
}

void dialogi(vector<string> rivit)
{
	puhdistaDialogi();

	for (int j = 0; j < rivit.size(); j++)
	{
		for (int i = 0; i < rivit[j].size(); i++)
		{
			sijainti(10 + i, 4 + j); cout << rivit[j][i];

			if(rivit[j][i] == '.' || rivit[j][i] == '!' || rivit[j][i] == '?')
				sleep_for(milliseconds(200));
			else
				sleep_for(milliseconds(10));
		}
	}

	odotaVastausta();
}

void puhdistaDialogi()
{
	for (int i = 4; i <= 9; i++)
	{
		sijainti(10, i);
		cout << string(40,' ');
	}
}

void odotaVastausta()
{
	while (_kbhit()) _getch();
	sijainti(5, 15);
}

int kaksiVaihtoehtoa(string vaihtoehto1, string vaihtoehto2)
{
	int valinta = 1;
	char key;

	sijainti(5, 16); cout << ">";
	sijainti(8, 16); cout << vaihtoehto1;
	sijainti(8, 17); cout << vaihtoehto2;

	for (int i = 0;;)
	{
		key = _getch(); // Lukee yhden käyttäjän syöttämän kirjainmerkin ja tallentaa sen "key" -muuttujalle

		if (key == 80 && valinta == 1) // Jos käyttäjä painaa alaspäin-nuolta ja valinta on 1...
		{
			valinta = 2; // ...valitaan valikosta alempi valinta
			sijainti(5, 16); cout << " ";
			sijainti(5, 17); cout << ">";
		}
		else if (key == 72 && valinta == 2) // Jos käyttäjä painaa ylöspäin-nuolta ja valinta on 2...
		{
			valinta = 1; // ...valitaan valikosta ylempi valinta
			sijainti(5, 16); cout << ">";
			sijainti(5, 17); cout << " ";
		}
		else if (key == '\r') // Jos käyttäjä painaa Enter-painiketta
		{
			return valinta;
			break;
		}
	}
}

int kolmeVaihtoehtoa(string vaihtoehto1, string vaihtoehto2, string vaihtoehto3)
{
	int valinta = 1;
	char key;
	
	sijainti(5, 16); cout << ">";
	sijainti(8, 16); cout << vaihtoehto1;
	sijainti(8, 17); cout << vaihtoehto2;
	sijainti(8, 18); cout << vaihtoehto3;

	for (int i = 0;;)
	{
		key = _getch(); // Lukee yhden käyttäjän syöttämän kirjainmerkin ja tallentaa sen "key" -muuttujalle

		if (key == 80 && valinta <= 2) // Jos käyttäjä painaa alaspäin-nuolta ja valinta on 1 tai 2...
			valinta++; // ...valitaan valikosta alempi valinta (2 tai 3)			
		else if (key == 72 && valinta >= 2) // Jos käyttäjä painaa ylöspäin-nuolta ja valinta on 2 tai 3...
			valinta--; // ...valitaan valikosta ylempi valinta (1 tai 2)
		else if (key == '\r') // Jos käyttäjä painaa Enter-painiketta
		{
			return valinta;
			break;
		}

		if (valinta == 1)
		{
			sijainti(5, 16); cout << ">";
			sijainti(5, 17); cout << " ";
			sijainti(5, 18); cout << " ";
		}
		else if (valinta == 2)
		{
			sijainti(5, 16); cout << " ";
			sijainti(5, 17); cout << ">";
			sijainti(5, 18); cout << " ";
		}
		else
		{
			sijainti(5, 16); cout << " ";
			sijainti(5, 17); cout << " ";
			sijainti(5, 18); cout << ">";
		}
	}
}

void puhdistaVastaus()
{
	for (int i = 14; i <= 19; i++)
	{
		sijainti(0, i);
		cout << string(100, ' ');
	}
}

void alustaTerminaali()
{
	system("cls");
	puhekupla();
	vastaanottoIloinen();
}

// Funktio, joka määrittää tulostettavan tekstin sijainnin
void sijainti(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void piilotaKursori() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false; // Piilottaa kursorin
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void naytaKursori() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true; // Näyttää kursorin
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}