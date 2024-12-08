// Aapo Harjunpää
// Harjoitustyö - Hotellihuoneen varausohjelma (5p)

// Lisätään tarvittavat kirjastot
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <locale>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>

using namespace std; // Mahdollistaa std-komentojen käytön ilman std::-etuliitettä
using namespace this_thread;	// Mahdollistavat ajastuskomennot,
using namespace chrono;			// kuten sleep_for() -funktion

// Luodaan globaalit vakiomuuttujat
const int MAX_HUONEET = 300, MIN_HUONEET = 40;

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

struct Hotellihuone {
	int huoneen_numero = 0;
	int huonetyyppi = 0;
	bool varaustila = false;
	int varausnumero = 1;
	string varaajan_nimi = "";
	double hinta = 0;
	int oidenMaara = 0;
};

// DEKLAROIDAAN FUNKTIOT
void paavalikko(vector<Hotellihuone>& huoneet);
void varaaHuone(vector<Hotellihuone>& hotellihuoneet);
void valitseHuoneItse(vector<Hotellihuone>& hotellihuoneet);
void valitseHuoneSatunnaisesti(vector<Hotellihuone>& hotellihuoneet);
void tutkiVarausta();
void luoHotellihuoneet(vector<Hotellihuone>& huoneet);
int vapaatHuonetyypit(vector<Hotellihuone>& huoneet, int tyyppi);
void puhekupla();
void vastaanotto();
void dialogi(vector<string> rivit);
void puhdistaDialogi();
void odotaVastausta();
int kaksiVaihtoehtoa(string vaihtoehto1, string vaihtoehto2);
int kolmeVaihtoehtoa(string vaihtoehto1, string vaihtoehto2, string vaihtoehto3);
void puhdistaVastaus();
void sijainti(int x, int y);
void piilotaKursori();
void naytaKursori();

int main()
{
	setlocale(LC_ALL, "FI_fi");
	srand(time(0));

	piilotaKursori();

	int huoneiden_maara = 0;

	do {
		huoneiden_maara = rand() % (MAX_HUONEET - MIN_HUONEET + 1) + MIN_HUONEET;
	} while (huoneiden_maara % 2 != 0);

	vector<Hotellihuone> hotellihuoneet(huoneiden_maara);
	luoHotellihuoneet(hotellihuoneet);

	puhekupla();
	dialogi({"Tervetuloa aaventen hotelliin!", "" ,"Miten voin olla avuksi?"});
	paavalikko(hotellihuoneet);
	
	return EXIT_SUCCESS;
}

void paavalikko(vector<Hotellihuone>& huoneet)
{
	// TODO:
	//		Moro! Haluan...
	//		1. vaihtoehto: varata huoneen
	//		2. vaihtoehto: tutkia varausta
	//		3. vaihtoehto: kotiin

	sijainti(5, 14); cout << "Moro! Haluan...";

	int valinta = kolmeVaihtoehtoa("varata huoneen", "tutkia varausta", "kotiin");

	puhdistaVastaus();

	if (valinta == 1)
	{
		varaaHuone(huoneet);
	}
	else if (valinta == 2)
	{
		tutkiVarausta();
	}
}

void varaaHuone(vector<Hotellihuone>& hotellihuoneet)
{
	int huoneenKoko = 1;

	dialogi({"Kuinka monen hengen huonetta etsit?"});
	
	sijainti(5, 14); cout << "Etsin...";
	huoneenKoko = kaksiVaihtoehtoa("yhden hengen huonetta", "kahden hengen huonetta");

	puhdistaVastaus();

	dialogi({ "Meillä on " + to_string(vapaatHuonetyypit(hotellihuoneet, huoneenKoko)) + " "
		+ to_string(huoneenKoko) + ":n hengen huonetta vapaa-", "na.", "", "Haluatko valita huoneen itse?", "", "" });

	int valinta = kaksiVaihtoehtoa("Kyllä", "En");
	puhdistaVastaus();

	if (valinta == 1)
	{
		valitseHuoneItse(hotellihuoneet);
	}
	else
	{
		valitseHuoneSatunnaisesti(hotellihuoneet);
	}
}

void valitseHuoneItse(vector<Hotellihuone>& hotellihuoneet)
{
	dialogi({ "Selvä!", "", "Tässä lista vapaista huoneistamme.", "Kerro vain haluamasi huoneen numero ja", "minä hoidan loput!"});
}

void valitseHuoneSatunnaisesti(vector<Hotellihuone>& hotellihuoneet)
{

}

void tutkiVarausta()
{

}

void luoHotellihuoneet(vector<Hotellihuone>& huoneet)
{
	int huonenumero = 1;
	vector<string> kaytetytNimet;

	// Käydään läpi kaikki hotellin huoneet ja annetaan jokaiselle satunnaisesti arvotut tiedot
	for (Hotellihuone& huone : huoneet)
	{
		// Annetaan huoneelle numero ja huonetyyppi (1:n tai 2:n hengen huone)
		huone.huoneen_numero = huonenumero;
		if (huonenumero <= huoneet.size() / 2)
			huone.huonetyyppi = 1;
		else
			huone.huonetyyppi = 2;

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
		}
	}
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

	vastaanotto();
}

void vastaanotto()
{
	sijainti(62, 4);  cout << "  @@@@@@@@     ";
	sijainti(62, 5);  cout << " /      @@@@   ";
	sijainti(62, 6);  cout << "|  ^  ^   @@   ";
	sijainti(62, 7);  cout << " \\ '--'   /    ";
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
				sleep_for(milliseconds(500));
			else
				sleep_for(milliseconds(30));
		}
	}

	odotaVastausta();
}

void puhdistaDialogi()
{
	for (int i = 4; i <= 9; i++)
	{
		sijainti(10, i);
		cout << "                                        ";
	}
}

void odotaVastausta()
{
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
		cout << "                                 ";
	}
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