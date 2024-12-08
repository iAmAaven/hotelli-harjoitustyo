# C++ ohjelmoinnin perusteet -kurssin harjoitustyö 2024
Kurssin tehtävänantona oli tehdä C++ -komentoriviohjelma hotellihuoneen varaamiselle.  
  
## Tehtävänannon mukaan suunniteltuja ominaisuuksia, jotka uskon riittävän korkeimpaan arvosanaan (5):
- Hotellissa on satunnaisesti valittu parillinen määrä huoneita 40:n ja 300:n väliltä, joista puolet ovat yhden hengen huoneita ja loput kahden hengen huoneita.
- Ohjelma arpoo varattujen huoneiden määrän, ja tekee näille varaustiedot.
- **Jokaisella varauksella on seuraavat tiedot:**
  - Huoneen numero (1 - n)
  - Huonetyyppi (1hh tai 2hh)
  - Varaustila (varattu = true, vapaa = false)
  - Varausnumero (10000 - 99999)
  - Varaajan nimi (Etunimi Sukunimi)
  - Öiden määrä (1 - 365)
  - Hinta (double)
- Käyttäjä voi päättää haluaako yhden vai kahden hengen huoneen, ja valita vapaan huoneen itse tai pyytää ohjelmaa valitsemaan hänen puolestaan. Ohjelma myös tarkistaa käyttäjän haluaman huonetyypin saatavuuden ennen jatkamista.
- Käyttäjän tulee antaa öiden määrä, jonka jälkeen ohjelma laskee varauksen hinnan.
- Yhden hengen huoneen hinta on 100 €/yö ja kahden hengen huoneen hinta on 150 €/yö. Ohjelma myös arpoo varauksen lopussa tuleeko lopulliseen hintaan 10% vai 20% alennusta, vai ei ollenkaan.
- Ohjelma antaa käyttäjää tekemään useamman varauksen, kuitenkin enintään vapaiden huoneiden verran.
- Käyttäjä voi tutkia varauksia varausnumerolla tai varaajan nimellä.
  - Kun käyttäjä etsii varauksia varaajan nimellä, ohjelma tulostaa kaikkien kyseisellä nimellä tehtyjen varausten tiedot "laatikoihin".
- Virheentarkistus:
  - Ohjelma varmistaa, että kaikki käyttäjän syötteet ovat annettu oikeassa muodossa.
  - Ohjelma on varautunut kaikkiin mahdollisiin tilanteisiin, esim. mitä tulostetaan, kun käyttäjä haluaa varata huoneen, mutta vapaita huoneita ei enää ole, yms.
  
## Joitain lisäominaisuuksia, joita olen lisännyt / aion lisätä:
- Hienosti ja selkeästi toteutettu päävalikko/käyttöliittymä, jossa hotellin vastaanottaja "puhuu" käyttäjälle.
- Vastaanottajan "animoitu" teksti ja ilmeet tietyissä kohdissa.
- Kysymyksiä varten, joihin on "rajoitettu määrä" vastauksia, rakensin valikon, jota voi navigoida nuolinäppäimillä. Tämä osoittautui myös luovaksi tavaksi vähentää käyttäjän mahdollisuuksia antaa vääränmuotoisia syötteitä.
- Jokaisen varatun huoneen tiedoissa on myös varaajan nimi. Käyttäjä voi tutkia varauksen tietoja, jos arvaa nimen oikein.
