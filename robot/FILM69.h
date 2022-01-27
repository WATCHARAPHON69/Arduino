#ifndef FILM69_h
#define FILM69_h
#include "Arduino.h"

class FILM69
{
    public:
       void DW(int DWpin,int IO);
       void AW(int AWpin,int ANALOG);
       int DR(int DRpin);
       int AR(int ARpin);
       bool TIME(unsigned long period);



      
       //String text();
    private:
        int _DWpin,_IO,_ANALOG,_AWpin,_DRpin,_ARpin;
        unsigned long _period;
        //bool _time;
        
        
};
#endif
