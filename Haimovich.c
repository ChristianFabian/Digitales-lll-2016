#include <xc.h>
#include "Ap_ini.h"
#include "Haimovich.h"

void enmascarado(void){
        LATA&=0b11110000;
        dato&=0b00001111;
        LATA|=dato;
}

void multiplexado(void){
    
    cont++;
    
    switch(cont){
        case 1:
            DISP4=0;
            dato=UniMil;
            enmascarado();
            DISP1=1;
            DOT=0;
            break;
        case 6:
            DISP1=0;
            dato=Cent;
            enmascarado();
            DISP2=1;
            DOT=1;
            break;
        case 11:
            DISP2=0;
            dato=Dec;
            enmascarado();
            DISP3=1;
            DOT=0;
            break;
        case 16:
            DISP3=0;
            dato=Uni;
            enmascarado();
            DISP4=1;
            DOT=0;
            break;
        case 20:
            cont=0;
            break;
    }
    
}
