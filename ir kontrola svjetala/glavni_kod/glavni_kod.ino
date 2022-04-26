// Mislav Stipić 2021 završni seminarski, labaratorij, mikroregulatori

#include <IRremote.h> // uključi biblioteku za infracrveni senzor

int IRpin = 9;  // pin IR senzora
int led1 = 4;   // pin prve ledice
int led2 = 5;   // pin druge ledice
int led3 = LED_BUILTIN; // pin ugrađene ledice na arduinu koja služi za signalizaciju stanja sklopa (dali smo upalili uređaj ili ne)

unsigned int stanje;    // pomoćne varijable koje nam pomažu da izađemo iz _while_ petlji
unsigned int stanje1;

int itsONled[] = {0,0,0,0};    // potvrda dali su ledice upaljene (1) ili ugašene (0) (prva varijabla itsONled[0] se ne koristi !!!!!!!!!)
                               // ove potvrde nam služe za funkcionalnost koda

       // vremenske varijable koje služe za _strobo_ efekt
unsigned long predhodno_vrijeme = 0;        // varijabla u koju se sprema zadnje vrijeme kada su ledice promjenile stnje
const long interval = 100;               // interval kada ledice mjenjaju stanje (millisekunde)
unsigned long trenutno_vrijeme;             // varijabla koja sprema trenutno vrijeme

int ledState = LOW;       // pomoćna varijabla koja služi za mjenjanje stanja ledica (strobo efekt)

#define code1 16712445 // kod sa daljinskog (on/off) __ služi za palit ili gasit uređaj (zajedno sa signalizacijskom ledicom na ploči)
#define code2 16718565 // kod sa daljinskog (R)      __ ova tipka služi za palit ili gasit 1. efekt
#define code3 16751205 // kod sa daljinskog (G)      __ ova tipka služi za palit ili gasit 2. efekt
#define code4 16753245 // kod sa daljinskog (B)      __ ova tipka služi za palit ili gasit 3. efekt


IRrecv prijemnik(IRpin);    // pridjeli pin senzora _IRpin_ biblioteci

decode_results rezultat;    // stvori konstruktor _rezultati_


// funkcija setup služi za inicijalizaciju portova, aktivaciju biblioteka 
// ili izvršavanje funkcija samo jedan put
void setup(){
  
Serial.begin(9600); // baud rate, služi za aktivaciju serijskog monitora i postavljanje frekvencije komunikacije sa računalom

prijemnik.enableIRIn(); // Startaj prijemnik

pinMode(led1, OUTPUT);    //postavi portove za ledice kao izlazne
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);

}



// glavna petlja
void loop() {

unsigned int value;     // pomoćna varijabla za switch case
  
 if(prijemnik.decode(&rezultat)){   // prvo moramo upaliti uređaj te se traži da upremo dugme on/off
  
  value = rezultat.value;     // ako smo uprili dugme on/off spremi primljenu vrjednost u varijablu value
  
    if(value == 765){   // ako smo uprili on/off dopusti pristup while petlji unutar koje se mogu koristiti sve tipke
      
      stanje = 1;              // dopusti pristup while petlji
      digitalWrite(led3, HIGH);  // signal upaljene ledice na ploči nam govori da možemo birati modove rada
  
      digitalWrite(led1, LOW);      // ostale ledice moraju ostat ugašene
      digitalWrite(led2, LOW);

      itsONled[1] = 0;  // potvrda ostatku koda da su ledice upaljene/ugašene
      itsONled[2] = 0;
      itsONled[3] = 1;
        
 }
 
    else{               
      
      stanje = 0;               //ako nismo uprili on/off dugme tada nemamo pristup while petlji i moramo ponoviti unos
      digitalWrite(led3, LOW);
  
      digitalWrite(led1, LOW);  // ostale ledice moraju ostat ugašene
      digitalWrite(led2, LOW);
      
      itsONled[1] = 0;  // potvrda ostatku koda da su ledice upaljene/ugašene
      itsONled[2] = 0;
      itsONled[3] = 0;
      
 }
  prijemnik.resume();  // spremono za primanje novih rezultata sa daljinskog
}





while(stanje == 1){       // ako smo uprili on/off izvrši dalje ovu petlju

  if(prijemnik.decode(&rezultat)) {   // glavni if uvjet -> unesi mod rada ledica
      
    value = rezultat.value;     // spremi primljenu vrijednost u varijablu value

switch(value) {   // switch-aj različite modove rada po varijabli value

case code1:     // ako smo uprili dugme on/off po DRUGI put tada izađi iz glavne while petlje ili u suprotnom ostani u petlji i čekaj druge tipke/naredbe

     if(itsONled[3] == 1) { // ako smo uprili dugme on/off po drugi put izvrši sljedeće:
      
      digitalWrite(led3, LOW); // isključi integriranu ledicu (više ne možemo primati naredbe sa daljinskog, osim on/off)
      
      digitalWrite(led1, LOW); // ostale ledice moraju ostat ugašene
      digitalWrite(led2, LOW);
      stanje = 0;              //ako smo uprili on/off po drugi put tada mijenjamo uvjet za glavnu petlju
      itsONled[1] = 0;  // potvrda ostatku koda da su ledice upaljene/ugašene
      itsONled[2] = 0;
      itsONled[3] = 0; 
      
  } 
      
     else { // ako nismo uprili dugme po drugi put ništa se ne mjenja
      
      digitalWrite(led3, HIGH); // integrirana ledica ostaje upaljena
      
      digitalWrite(led1, LOW); // ostale ledice moraju ostat ugašene
      digitalWrite(led2, LOW);
      stanje = 1;         // uvjet za while petlju se ne mjenja 
      itsONled[1] = 0;  // potvrda ostatku koda da su ledice upaljene/ugašene
      itsONled[2] = 0; 
      itsONled[3] = 1; 
  }
break;


case code2:   // prvi mod rada je paljenje obe ledice ili gašenje obe ledice

     if(itsONled[1] == 1 && itsONled[2] == 1) { // ako su obe ledice bile upaljene izvrši kod
      
      digitalWrite(led1, LOW); // kada smo uprili dugme R po drugi puta tada gasimo ledice
      digitalWrite(led2, LOW);
      
      itsONled[1] = 0; // potvrda ostatku koda da su ledice upaljene/ugašene
      itsONled[2] = 0;
  } 
      
     else { // ako smo uprili dugme samo jednom tada upali ledice
      
      digitalWrite(led1, HIGH);  // kada smo uprili dugme R samo jednom tada palimo ledice
      digitalWrite(led2, HIGH);
      
      itsONled[1] = 1; // potvrda ostatku koda da su ledice upaljene/ugašene
      itsONled[2] = 1;
  }
break;



case code3:     // strobo mod rada ledica (treptanje ledica)

  stanje1 = 1;    // uvjet za ulazak u sljedeću while petlju
  
 while(stanje1 == 1){
        
    strobo();   // izvrši prvi blink ledica (funkcija je napisana ispod glavne loop petlje)
    
      if(prijemnik.decode(&rezultat)) {                      // ovaj if čeka da unesemo naredbu on/off kako bi mogli izaći iz ovog moda rada
             
         if(rezultat.value == code1){   //ako smo uprili on/off dugme izvrši sljedeće:
          
             stanje1 = 0;     // mijenjamo uvjet za ostati u ovoj while petlji kako bi prestao ovaj mod rada
             
               digitalWrite(led1, LOW); // gasimo obe ledice
               digitalWrite(led2, LOW);
               
               itsONled[1] = 0; // potvrda ostatku koda da su ledice upaljene/ugašene
               itsONled[2] = 0;        
         }
          prijemnik.resume(); // spremono za primanje novih rezultata sa daljinskog
        }
        
      }
   
break;

case code4:   // naizmjenično paljenje prve ili druge ledice na tipku

      if(itsONled[2] == 1 || itsONled[1] == 0) {
        digitalWrite(led2, LOW);
        digitalWrite(led1, HIGH);
        itsONled[1] = 1;
        itsONled[2] = 0;
      } 

      else {
       digitalWrite(led2, HIGH);
       digitalWrite(led1, LOW);
       itsONled[1] = 0;
       itsONled[2] = 1;
      }

break;


    } // zatvori switch case
    

 prijemnik.resume(); // spremono za primanje novih rezultata sa daljinskog
 
   } //zatvori glavni if uvjet u glavnoj while petlji

  } // zatvori glavni while uvjet
  
} // zatvori glavnu loop petlju





void strobo(){    // funkcija za izvršavanje strobo moda rada
  
  trenutno_vrijeme = millis(); // spremni trenutno vrijeme
  
  if (trenutno_vrijeme - predhodno_vrijeme >= interval) {   // ako je trenutno vrijeme - prošlo vrijeme >= zadanom intervalu tada izvrši:
    
    predhodno_vrijeme = trenutno_vrijeme;   // spremi predhodno vrijeme

   
    if (ledState == LOW) {   // ako smo upalili ledice , sada ih gasimo i obrnuto
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // prosljedi sljedeće stanje (promjenu stanja) ledicama kako bi se upalile ili gasile
    digitalWrite(led1, ledState); 
    digitalWrite(led2, ledState);
    
  }
  
}
