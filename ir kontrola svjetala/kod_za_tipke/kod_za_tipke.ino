#include <IRremote.h>     //uključi biblioteku

int IRpin = 9;              // pin za infracrveni senzor
IRrecv prijemnik(IRpin);     // pridjeli pin senzora _IRpin_ biblioteci
decode_results rezultat;     // stvori konstruktor _rezultati_

void setup(){
  
Serial.begin(9600);   // baud rate (nužan da bi mogli čitati primljene vrijednosti)
prijemnik.enableIRIn();  // Startaj prijemnik

}

void loop(){
  unsigned int value;  // pomoćna varijabla 
  if (prijemnik.decode(&rezultat)){       // ako smo poprimili neku vrijednost sa senzora izvrši kod

    Serial.println(rezultat.value, DEC);  // Printaj 'results.value' u decimalnom obliku
    
    prijemnik.resume();          // primi nove rezultate daljinskog

 }

}
