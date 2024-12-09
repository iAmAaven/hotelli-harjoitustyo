// Aapo Harjunp��
// Harjoitusty� - Hotellihuoneen varausohjelma (5p)


#define NOMINMAX

// Lis�t��n tarvittavat kirjastot
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

using namespace std; // Mahdollistaa std-komentojen k�yt�n ilman std::-etuliitett�
using namespace this_thread;	// Mahdollistavat ajastuskomennot,
using namespace chrono;			// kuten sleep_for() -funktion

// Luodaan globaalit vakiomuuttujat
const int MAKS_HUONEET = 300, MIN_HUONEET = 40;

// Etunimipankin tiedot ovat top 18 nimipalvelu.dvv.fi -sivuston suosituimpien etunimien listalta 2020-vuosikymmenen ajalta
	// Sukunimet ovat saman sivuston "Yleisimm�t sukunimet" -listalta
// N�it� vektoreita k�ytet��n satunnaisesti luotujen huonevarausten varaajien nimen luomisessa
// Etunimi� ja sukunimi� on kumpiakin 18, koska maksimi huoneiden m��r� on 300.
	// 300:n neli�juuri on 17,32, jolloin 18 on seuraava kokonaisluku.
	// Eri nimiyhdistelmi� saadaan siis enint��n 324 kpl (18*18),
	// jolloin varaajan nimi ei voi olla kahdessa eri huoneessa sama.
const vector<string> etunimipankki = {
	"Olavi", "Juhani", "Johannes", "Mikael", "Oliver", "Ilmari", "Onni", "Elias", "Eino",
	"Maria", "Sofia", "Aurora", "Olivia", "Emilia", "Aino", "Matilda", "Linnea", "Ellen",
};
const vector<string> sukunimipankki = {
	"Korhonen", "Virtanen", "M�kinen", "Nieminen", "M�kel�", "H�m�l�inen", "Laine", "Heikkinen", "Koskinen",
	"J�rvinen", "Lehtonen", "Lehtinen", "Saarinen", "Niemi", "Salminen", "Heinonen", "Heikkil�", "Kinnunen",
};

struct Hotellihuone {
	int huoneen_numero = 0;
	int huonetyyppi = 0;
	bool varaustila = false;
	int varausnumero = 1;
	string varaajan_nimi = "";
	int oidenMaara = 0;
	int hintaPerYo = 0;
	double varauksenHinta = 0;
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

int main()
{
	setlocale(LC_ALL, "FI_fi");
	srand(time(0));

	piilotaKursori();

	int huoneiden_maara = 0;

	do {
		huoneiden_maara = rand() % (MAKS_HUONEET - MIN_HUONEET + 1) + MIN_HUONEET;
	} while (huoneiden_maara % 2 != 0);

	vector<Hotellihuone> hotellihuoneet(huoneiden_maara);
	luoHotellihuoneet(hotellihuoneet);
	
	paavalikko(hotellihuoneet);
	
	dialogi({ "Turvallista kotimatkaa!" });

	return EXIT_SUCCESS;
}

void paavalikko(vector<Hotellihuone>& huoneet)
{
	for (int i = 0;;)
	{
		// Tavoiteltu ulkoasu p��valikolle:
		// 
		//		Moro! Haluan...
		// 
		//		>  varata huoneen
		//		   tutkia varausta
		//		   kotiin
		//
		
		alustaTerminaali();

		puhekupla();
		dialogi({ "Tervetuloa aaventen hotelliin!", "" ,"Miten voin olla avuksi?" });

		sijainti(5, 14); cout << "Moro! Haluan...";

		int valinta = kolmeVaihtoehtoa("varata huoneen", "tutkia varausta", "kotiin");

		puhdistaVastaus();

		if (valinta == 1) // Jos k�ytt�j� haluaa varata huoneen...
		{
			varaaHuone(huoneet); //...kutsutaan siihen tarkoitettu aliohjelma
		}
		else if (valinta == 2)
		{
			dialogi({ "Voitte antaa minulle varausnumeron", "tai varaajan nimen, jos haluat", "tutkia jokaista sill� nimell� tehty�", "varausta." });
			int tutkintaValinta = 0;
			tutkintaValinta = kaksiVaihtoehtoa("Minulla on varausnumero", "Haluan tutkia nimell�ni tekemi� varauksia");
			puhdistaVastaus();

			if (tutkintaValinta == 1)
			{
				int varausnroSyote = 0;

				while (true)
				{
					dialogi({ "Selv�!", "Anna varausnumero." });
					naytaKursori();
					sijainti(5, 14); cout << "Varausnumeroni on...";
					sijainti(8, 16); cin >> varausnroSyote;
					puhdistaVastaus();
					piilotaKursori();

					bool varausLoydetty = false;

					if (cin.fail())
					{
						cin.clear(); // Tyhjenn� virhetila
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
								dialogi({ "Aah! T�ss� sinun varauksesi." });
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
							dialogi({ "En l�yt�nyt varausta tuolla", "numerolla. Haluatko yritt��", "uudelleen?" });
							int uudelleenValinta = 0;
							uudelleenValinta = kaksiVaihtoehtoa("Kyll�", "En");
							puhdistaVastaus();

							if (uudelleenValinta == 2)
								break;
						}
					}
				}
			}
			else // K�ytt�j� haluaa tutkia varauksia nimen perusteella
			{
				string nimiSyote = "";
				cin.clear();

				while (true)
				{
					dialogi({ "Antakaa minulle varaajan nimi,", "kiitos."});
					bool varausLoydetty = false;
					naytaKursori();
					sijainti(5, 14); cout << "Varaajan nimi on...";
					sijainti(8, 16); getline(cin, nimiSyote); // Yrit� ottaa k�ytt�j�n sy�te
					puhdistaVastaus();

					if (cin.fail() || nimiSyote.empty()) // Jos sy�tteess� on virhe tai se on tyhj�
					{
						cin.clear(); // Tyhjenn� virhetila
						cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjenn� puskurin sis�lt�
						puhdistaVastaus(); // Tyhjenn� k�ytt�j�n n�kym�
					}
					else
					{
						piilotaKursori();
						int riviLisays = 0;
						for (Hotellihuone huone : huoneet)
						{
							if (huone.varaustila == true && huone.varaajan_nimi == nimiSyote)
							{
								varausLoydetty = true;
								tutkiVarausta(huone, riviLisays);
								riviLisays += 10;
							}
						}

						if (varausLoydetty == true)
						{
							dialogi({ "Aah! T�ss� sinun varauksesi." });
							int kiitosValinta = 0;
							kiitosValinta = kaksiVaihtoehtoa("Kiitos!", "Kiitos paljon!");
							puhdistaVastaus();

							break;
						}
						else
						{
							dialogi({ "En l�yt�nyt varausta tuolla", "nimell�. Haluatko yritt��", "uudelleen?" });
							int uudelleenValinta = 0;
							uudelleenValinta = kaksiVaihtoehtoa("Kyll�", "En");
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
		dialogi({"Meill� ei ole en�� vapaita", "huoneita. Pahoittelut."});
		int selva = 0;
		selva = kaksiVaihtoehtoa("Selv�", "Asia selv�");
		return;
	}

	int huoneenKoko = 1;

	dialogi({"Kuinka monen hengen huonetta etsitte?"});
	
	sijainti(5, 14); cout << "Etsin...";
	huoneenKoko = kaksiVaihtoehtoa("yhden hengen huonetta", "kahden hengen huonetta");

	if (vapaatHuonetyypit(hotellihuoneet, huoneenKoko) == 0)
	{
		dialogi({ "Valitettavasti meill� ei ole", "en�� vapaita " + to_string(huoneenKoko) + ":n hengen huoneita." });
		int selva = 0;
		selva = kaksiVaihtoehtoa("Selv�", "Asia selv�");
		return;
	}

	puhdistaVastaus();

	dialogi({ "Meill� on " + to_string(vapaatHuonetyypit(hotellihuoneet, huoneenKoko)) + " "
		+ to_string(huoneenKoko) + ":n hengen huonetta vapaa-", "na.", "", "Haluatteko valita huoneen itse?", "", "" });

	int valinta = kaksiVaihtoehtoa("Kyll�", "En");
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
		asiaSelv = kaksiVaihtoehtoa("Selv�", "Asia selv�");
		puhdistaVastaus();
	}

	//

	int olikoOikeaNimi = 0;
	string varaajanNimiSyote = "";
	cin.clear();

	do {
		alustaTerminaali();
		varaajanNimiSyote = kayttajanNimi();
		dialogi({ varaajanNimiSyote + "?", "Sainko nimen oikein?" });
		olikoOikeaNimi = kaksiVaihtoehtoa("Kyll�", "Et, nimi on...");
	} while (olikoOikeaNimi != 1);


	puhdistaVastaus();
	dialogi({"Selv�!", "Kuinka monta y�t� haluatte viett��", "hotellissamme?", "Varauksen enimm�ispituus on", "365 p�iv��."});

	int oidenMaaraSyote;

	while (true)
	{
		naytaKursori();
		sijainti(5, 14); cout << "Kuinka monta y�t�k�?";
		sijainti(8, 16); cin >> oidenMaaraSyote;
		piilotaKursori();

		if (cin.fail() || oidenMaaraSyote < 1 || oidenMaaraSyote > 365)
		{
			cin.clear(); // Tyhjenn� virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			puhdistaVastaus();
		}
		else break;
	}

	alustaTerminaali();
	dialogi({to_string(oidenMaaraSyote) + " y�t� siis!", "Odottakaa, kun lasken varauksenne hinnan.", "...", "...", "..."});

	double hinta = kayttajanVarauksenHinta(oidenMaaraSyote, hotellihuoneet[huoneNroSyote-1].hintaPerYo);

	ostringstream hintaStream;
	hintaStream.precision(2); // M��ritet��n tarkkuus kahteen desimaaliin
	hintaStream << fixed << hinta;

	dialogi({ "Varauksen lopullinen hinta on","", hintaStream.str() + " euroa." });

	sleep_for(seconds(2));

	dialogi({ "Oletteko tyytyv�inen varaukseenne", "vai haluatteko peruuttaa sen?" });
	int tyytyvaisyys = kaksiVaihtoehtoa("Olen tyytyv�inen", "Haluan perua varauksen");
	puhdistaVastaus();

	if (tyytyvaisyys == 1)
	{
		dialogi({ "Hienoa!", "Maksakaa " + hintaStream.str() + " euroa,", "niin saatte avaimen."});
		
		int maksatko = 0;
		maksatko = kaksiVaihtoehtoa("T�ss� " + hintaStream.str() + " euroa, olkaa hyv�", "En halua maksaa");
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

			dialogi({ "Kiitos varauksestanne!", "", "T�ss� viel� varauksesi tiedot."});
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
		dialogi({ "Asia harvinaisen selv�.", "Haluatteko tehd� eri varauksen?" });
		vastaanottoVihainen();
		int paatos = kaksiVaihtoehtoa("Kyll�", "En");
		puhdistaVastaus();

		if (paatos == 1)
			varaaHuone(hotellihuoneet);
	}
}

int valitseHuoneItse(vector<Hotellihuone>& hotellihuoneet, int huoneenKoko)
{
	dialogi({ "Selv�!", "", "T�ss� lista vapaista huoneistamme.", "Kerro vain haluamasi huoneen numero ja", "min� hoidan loput!"});

	vector<int> vapaatHuonenumerot;

	int huoneNroSyote;

	while(true)
	{
		naytaKursori();
		tulostaVapaatHuoneet(hotellihuoneet, huoneenKoko, vapaatHuonenumerot);

		sijainti(5, 14); cout << "Haluan huoneen nro...";
		sijainti(8, 16); cin >> huoneNroSyote;
		piilotaKursori();

		if (cin.fail() || find(vapaatHuonenumerot.begin(), vapaatHuonenumerot.end(), huoneNroSyote) == vapaatHuonenumerot.end())
		{
			cin.clear(); // Tyhjenn� virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			puhdistaVastaus();
		}
		else break;
	}

	alustaTerminaali();

	dialogi({"Asia selv�! Huone " + to_string(huoneNroSyote) + " siis!"});
	return huoneNroSyote;
}

string kayttajanNimi()
{
	dialogi({ "Mill� nimell� varaus tulee,", "arvon ihminen?", });

	string nimiSyote = "";

	while (true)
	{
		naytaKursori();
		sijainti(5, 14); cout << "Varaus tulee nimell�...";
		sijainti(8, 16);

		getline(cin, nimiSyote); // Yrit� ottaa k�ytt�j�n sy�te

		if (cin.fail() || nimiSyote.empty()) // Jos sy�tteess� on virhe tai se on tyhj�
		{
			cin.clear(); // Tyhjenn� virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjenn� puskurin sis�lt�
			puhdistaVastaus(); // Tyhjenn� k�ytt�j�n n�kym�
		}
		else break; // Sy�te on kunnossa, lopeta silmukka
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

	alustaTerminaali();

	dialogi({ "Teid�n huoneenne numero on ", to_string(huoneNro) });
	return huoneNro;
}

void tutkiVarausta(Hotellihuone& huone, int rivinLisays)
{
	double hinta = huone.varauksenHinta;

	ostringstream hintaStream;
	hintaStream.precision(2); // M��ritet��n tarkkuus kahteen desimaaliin
	hintaStream << fixed << hinta;

	if (rivinLisays != 0)
	{
		sijainti(40, 13);
		cout << "Kaikki varaukset nimell�";
	}
	
	sijainti(40, 14); cout << huone.varaajan_nimi;

	sijainti(40, 17 + rivinLisays); cout << "Varaus: " << huone.varausnumero;
	sijainti(40, 19 + rivinLisays); cout << "Huone: " << huone.huoneen_numero << ", tyyppi " << huone.huonetyyppi << "hh";
	sijainti(40, 20 + rivinLisays); cout << "�iden m��r�: " << huone.oidenMaara;
	sijainti(40, 21 + rivinLisays); cout << "Hinta: " << hintaStream.str() << " euroa";
}

void luoHotellihuoneet(vector<Hotellihuone>& huoneet)
{
	int huonenumero = 1;
	vector<string> kaytetytNimet;
	vector<int> kaytetytVarausnumerot;

	// K�yd��n l�pi kaikki hotellin huoneet ja annetaan jokaiselle satunnaisesti arvotut tiedot
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

			huone.varaajan_nimi = nimi; // Lis�t��n luotu nimi huoneen varaajan nimeksi
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
	// Tekstikentt� on x = 10-49, y = 4-9
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
		key = _getch(); // Lukee yhden k�ytt�j�n sy�tt�m�n kirjainmerkin ja tallentaa sen "key" -muuttujalle

		if (key == 80 && valinta == 1) // Jos k�ytt�j� painaa alasp�in-nuolta ja valinta on 1...
		{
			valinta = 2; // ...valitaan valikosta alempi valinta
			sijainti(5, 16); cout << " ";
			sijainti(5, 17); cout << ">";
		}
		else if (key == 72 && valinta == 2) // Jos k�ytt�j� painaa yl�sp�in-nuolta ja valinta on 2...
		{
			valinta = 1; // ...valitaan valikosta ylempi valinta
			sijainti(5, 16); cout << ">";
			sijainti(5, 17); cout << " ";
		}
		else if (key == '\r') // Jos k�ytt�j� painaa Enter-painiketta
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
		key = _getch(); // Lukee yhden k�ytt�j�n sy�tt�m�n kirjainmerkin ja tallentaa sen "key" -muuttujalle

		if (key == 80 && valinta <= 2) // Jos k�ytt�j� painaa alasp�in-nuolta ja valinta on 1 tai 2...
			valinta++; // ...valitaan valikosta alempi valinta (2 tai 3)			
		else if (key == 72 && valinta >= 2) // Jos k�ytt�j� painaa yl�sp�in-nuolta ja valinta on 2 tai 3...
			valinta--; // ...valitaan valikosta ylempi valinta (1 tai 2)
		else if (key == '\r') // Jos k�ytt�j� painaa Enter-painiketta
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

// Funktio, joka m��ritt�� tulostettavan tekstin sijainnin
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
	cursorInfo.bVisible = true; // N�ytt�� kursorin
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}