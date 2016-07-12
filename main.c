#include <xc.h>
#include "confbits.h"   
#include "Ap_ini.h"      
#include "lemos.h"       //funciones personalizadas Prof Lemos
#include "Haimovich.h"

# define _XTAL_FREQ  20000000   //para ser usado con la macro __delayms()
unsigned int frec, botfrec;  //Variale que define la frecuencia del Timer1
unsigned char Inc_Dec=0, Start1=0;
unsigned int Cont3=0, Cont4=0, incremento=0, decremento=0;
void main(void) {
    
    union {
        struct {
            unsigned char ARRIBA :1; //declaro las variables de los botones
            unsigned char ABAJO :1;
            unsigned char START:1;
            unsigned char RESET :1;
            unsigned char Dummy   :4;

        }flags;
            unsigned char RESET1;
    }MemoriaDeBotones;
    
    pic_ini13();                    
    timer_ini13();       
    timer1_ini13();        //Llama a la funcion Timmer1
    INTCONbits.GIE = 1;    
    ei();     
    
    T0CONbits.TMR0ON = 1;
    T1CONbits.TMR1ON = 0;  //Apago el Timer1 para que el programa arranque sin insertar ninguna frecuencia
    

    while(1){   
        
        if(EstadoDeLosBotones==16 || EstadoDeLosBotones==32 || EstadoDeLosBotones==64 || EstadoDeLosBotones==128){
            T1CONbits.TMR1ON = 1;   //Inicio el Timer1 cuando se presione cualquiera de los botones   
        }
        
        switch(EstadoDeLosBotones){
            case 16:    
                
                incremento++;
                
                if(incremento >= 40000){
                    if(Uni<9){
                        Uni++;
                    }
                    else{
                        Uni=0;
                        Dec++;
                        if(Dec>5){
                             Dec=0;
                             Cent++;
                           if(Cent>9){
                                Cent=0;
                                UniMil++;
                               if(UniMil>5){
                                    UniMil=0;
                                    Cent=0;
                                    Dec=0;
                                    Uni=0;
                               }
                           }
                        }
                    }
                incremento=0;   
                }
                break;
                
            case 32:                   
                    
                    decremento++;
                    
                    if(decremento >= 40000){
                        if(Uni<=9){
                            Uni--; 
                            if(Uni>9){
                                 Uni=9;
                                 Dec--;
                                 if(Dec>5){
                                    Dec=5;
                                    Cent--;
                                    if(Cent>9){
                                        Cent=9;
                                        UniMil--;
                                        if(UniMil>5){
                                            UniMil=5;
                                            Cent=9;
                                            Dec=5;
                                            Uni=9;
                                        }
                                    }
                                }
                            }
                        }
                        decremento=0;
                    }  
                break;
                
            case 64:
                 if(!MemoriaDeBotones.flags.START){
                    MemoriaDeBotones.flags.START = 1;
                    Start1=~Start1;
                }    
                break;
            
            case 128:
                 if(!MemoriaDeBotones.flags.RESET){
                    MemoriaDeBotones.flags.RESET = 1;
                    Uni=0;
                    Dec=0;
                    Cent=0;
                    UniMil=0;
                    Cont3=0;
                    Start1=0;
                    LED1=0;
                }
                break;
                        
                
            case 0:                 //Release (suelto boton))
                MemoriaDeBotones.RESET1=0; 
            default:
                break;      
                
        }
        
        
        
    }
}
void __interrupt myISR(void){
    /*Aquí se ejecuta el código de las interrupciones que hayan sucedido,
     *por defecto el XC8 entra siempre en esta interrupción denominada de
     *Alta prioridad
     *NO OLVIDES DESCOMENTAR ei(), PARA HABILITAR LAS INTERRUPCIONES
     *
     */
    //el siguiente código verifica que el flag de TIMER0 esté habilitado
#ifdef AP_INI_H
        if(TMR0IF){
            INTCONbits.TMR0IF = 0;  //borra el flag de la interrupción de timer
            TMR0L = 209;           //reinicia el timer
            TMR0H = 0xFF;
        #ifdef  LEMOS_H
            tic_timer0();           //llama a la función tic_timer
        #endif
        }
     #endif

#ifdef AP_INI_H
        if (TMR1IF){
                PIR1bits.TMR1IF = 0; //Borra el flag de la interrupción de Timer1
                TMR1=53590;
               
                if(Start1)
                    Cont3++;
                
                if(Cont3==1000){
                    Cont3=0;
                    Uni++;
                    if(Uni>9){
                        Uni=0;
                        Dec++;
                        if(Dec>5){
                            Dec=0;
                            Cent++;
                            if(Cent>9){
                                Cent=0;
                                UniMil++;
                                if(UniMil>5){
                                    LED1=1;
                                    Uni=0;
                                    Dec=0;
                                    Cent=0;
                                    UniMil=0;
                                    Start1=0;
                                }
                            }
                        }
                    }
                }
                
                if(LED1==1 && Start1==0){
                    Cont4++;
                    if(Cont4>=5000)
                        LED1=0;
                }    
                multiplexado();
        }
#endif
}