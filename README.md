# TwoNodes-LoRaWAN

[Romanian description]
Aplicația implementată a avut ca scop principal acomodarea cu fluxul de lucru necesar pentru a transmite informații către o rețea folosind protocolul LoRa, și, mai mult de atât, preluarea datelor din rețea și folosirea acestora în aplicația finală. În implementarea acestui proiect am avut ca punct de sprijin o listă de funcționalități pe care aplicația trebuie să le realizeze la final.
Cerințe funcționale:

1.	Transmiterea informațiilor de la senzor (temperatură și umiditate) către consola din The Things Network prin intermediul rețelei LoRaWAN.
2.	Decodarea mesajului recepționat și folosirea informațiilor pentru a crea o aplicație folosind platforma IFTTT.
3.	Alimentare de la acumulator și activarea modului de economisire energie pentru nod.
4.	Afișarea datelor primite de la senzor în interfața ThingSpeak.
5.	Afișarea informațiilor primite de la un nod pe un al doilea nod.

Am început implementarea aplicației prin folosirea unei plăci TTGO pentru a construi nodul care va transmite datele măsurate de senzor, placă ce folosește la bază un microcontroler ESP32 la care este atașat un modul Semtech SX1276 prin care se creează implementarea protocolului LoRa de comunicare radio. Am început prin folosirea unui exemplu oferit de către producătorul plăcii de dezvoltare, exemplu pe care l-am modificat astfel încât să îndeplinească cerințele proiectului. Astfel, am adăugat la exemplu activarea modului de economisire energie disponibil pe microcontrollerul ESP32 și am setat placa să transmită informația de la senzor la fiecare 60 de secunde, după care se activează din nou modul de hibernare. Fiecare transmisie a datelor este marcată vizual prin aprinderea ledului de pe placă timp de o secundă.

La placa TTGO am conectat un senzor de temperatură și umiditate DHT11, cele două valori fiind transmise către rețea sub forma unui mesaj al cărui conținut este reprezentat de 2 bytes, primul reprezentând temperatura, iar cel de al doilea umiditatea. În platforma The Things Network are loc o decodare a mesajului, obținându-se astfel valorile de la senzor.

Valorile astfel traduse sunt transmise către IFTTT, care mai departe creează un mesaj și îl trimite către contul meu de Facebook Messenger. Pe lângă cele două valori, către IFTTT este trimisă și o a treia valoare, de tip boolean, care este folosită pentru a verifica dacă mesajul provine de la nodul care are și senzorul, sau este un mesaj de la nodul la care mesajul trebuie să ajungă. Astfel, trimit către Facebook Messenger doar date relevante legate de temperatură și umiditate, din minut în minut.

Pentru a transmite datele către ThingSpeak, am ales să creez un script pe un alt site și să-l conectez prin intermediul integrării HTTP ce este disponibilă în platforma The Things Network. Script-ul verifică dacă cererea înregistrată a venit de la placa ce conține senzorul, placă numită „ttgo-node”. În caz afirmativ, preia valorile temperaturii și a umidității pe care le transmite către canalul de ThingSpeak și către celălalt nod (despre care vom discuta imediat). Pentru a transmite datele către celălalt nod, scriptul formează un mesaj JSON și creează un mesaj de „downlink” care este transmis rețelei.

Nodul care recepționează mesajele este implementat folosind o plăcuță The Things Uno, aceasta fiind placa originală The Things Network, construită pe platforma unui Arduino UNO, cea mai importantă diferență dintre cele două plăci de dezvoltare fiind modulul radio prezent pe placa The Things UNO, Microchip RN2483. La această placă am conectat un ecran oled SSD1306 pe care sunt afișate informațiile primite de la rețea. Pentru a fi posibilă recepționarea mesajelor de la rețea, este nevoie ca această placă să simuleze continuu transmisia unor mesaje către rețea, acest procedeu având numele de „pool”. Practic, placa transmite câte un bit de date, date care nu sunt verificate, ci doar ignorate, dar care ajută rețeaua să pregătească mesajul pentru placă, care la următoarea transmisie va primi și datele pregătite pentru ea.

La alimentarea, această placă așteaptă întâi să fie acceptată în rețea, moment care este marcat prin afișarea pe ecran a mesajului „Joined”. În momentul recepției primului mesaj de la rețea, textul se modifică, pe ecran fiind afișate datele de la senzor.

