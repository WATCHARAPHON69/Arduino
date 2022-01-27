
#include "FILM69.h"

void FILM69::DW(int DWpin,int IO){
    _DWpin  = DWpin;
    _IO = IO;
    pinMode(_DWpin,OUTPUT);
    digitalWrite(_DWpin,IO);
}

void FILM69::AW(int AWpin,int ANALOG){
    _AWpin  = AWpin;
    _ANALOG = ANALOG;
    pinMode(_AWpin,OUTPUT);
    analogWrite(_AWpin,ANALOG);
}

int FILM69::DR(int DRpin){
    _DRpin  = DRpin;
    pinMode(_DRpin,INPUT);
    return digitalRead(_DRpin);
}

int FILM69::AR(int ARpin){
    _ARpin  = ARpin;
    pinMode(_ARpin,INPUT);
    return analogRead(_ARpin);
}
unsigned long last_time = 0;
bool satatus = 0; 
bool FILM69::TIME(unsigned long period){
    
    _period = period;
    if ( millis() - last_time > period) {
        last_time = millis();
        satatus = !satatus;

         return satatus;
    }
   
    
}
