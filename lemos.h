/* 
 * File:   Lemos.h
 * Author: Administrador
 *
 * Created on 11 de junio de 2014, 07:33
 */

#ifndef LEMOS_H
#define	LEMOS_H

#ifdef	__cplusplus
extern "C" {
#endif

//Envia un dato a cualquiera de los cuatro displays
void Send_Disp(unsigned char NroDisp,unsigned char Dato);
//envía cuatro datos a los cuatro displays
void Send_4Disp(unsigned char Umil,unsigned char Cent,unsigned char Dec,unsigned char Uni);
//Los leds hacer un efecto de desplazamiento ajustando la demora
void leds(unsigned int Periodo);
//Devuelve un el numero de boton presionado entre 1 y 4 0=ninguno
unsigned char Botones13(void);
//devuelve un registro completo de 8 bits con 8 botones filtrados
unsigned char debounce(unsigned char sample);
//sincroniza contadores a partir de 1ms de interrupción de timer
void tic_timer0(void);


/*variables globales utilizadas por las funciones incluidas*/
unsigned char mux_tout,bot_tout;
unsigned int  led_tout;
volatile unsigned char EstadoDeLosBotones;

#define MuxSet 4;               //Tiempo en ms para el multiplexado
#define BotonReleaseSet 40;     //Tiempo en ms para la liberación de botones
#define NoPresiona 0;           //valor nulo para ningun boton presionado
#define DebounceSet 10;          //tiempo en ms para chequear los botones

enum{
    BotonLibre,
    BotonEnEspera,
    BotonDetectado,
    BotonEsperaLiberarce,
         
};

#ifdef	__cplusplus
}
#endif

#endif	/* LEMOS_H */

