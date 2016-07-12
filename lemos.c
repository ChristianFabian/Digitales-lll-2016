#include <xc.h>
#include "Ap_ini.h"
#include "lemos.h"

//algunas funciones para tomar de ejemplo

void leds(unsigned int Periodo_ms) {
    static unsigned char i = 1;


    switch (i) {
        case 1:if (!led_tout) {
                LED1 = ~LED1;
                led_tout = Periodo_ms;
                i++;
            }
            break;
        case 2:if (!led_tout) {
                LED2 = ~LED2;
                led_tout = Periodo_ms;
                i++;
            }
            break;
        case 3:if (!led_tout) {
                LED3 = ~LED3;
                led_tout = Periodo_ms;
                i++;
            }
            break;
        case 4:if (!led_tout) {
                LED4 = ~LED4;
                led_tout = Periodo_ms;
                i = 1;
            }
            break;
        default:
        {
            led_tout = 1000;
            i = 1;
        }
    }
}

void Send_Disp(unsigned char NroDisp, unsigned char Dato) {

    LATA = 0; //Borro el dato

    DISP1 = 0;
    DISP2 = 0;
    DISP3 = 0;
    DISP4 = 0;

    switch (NroDisp) {
        case 1:
        {
            DISP1 = 1;
        }
        break;
        case 2:
        {
            DISP2 = 1;
        }
        break;
        case 3:
        {
            DISP3 = 1;
        }
        break;
        case 4:
        {
            DISP4 = 1;
        }
        break;
        default:
        {
            DISP1 = 1;
        }
        break;
    }
    Dato = Dato & 0x0F; //borro el nibble superior
    LATA = LATA & 0xF0; //borro el nibble inferior
    LATA = LATA | Dato; //copio el nibble inferior de Dato en el puerto A
}

void Send_4Disp(unsigned char Umil, unsigned char Cent, unsigned char Dec, unsigned char Uni) {

    static unsigned char Nro_Disp = 1;

    switch (Nro_Disp) {
        case 1:
        {
            if (!mux_tout) {
                Send_Disp(1, Umil);
                mux_tout = MuxSet;
                Nro_Disp = 2;
            }
        }
            break;
        case 2:
        {
            if (!mux_tout) {
                Send_Disp(2, Cent);
                mux_tout = MuxSet;
                Nro_Disp = 3;
            }
        }
            break;
        case 3:
        {
            if (!mux_tout) {
                Send_Disp(3, Dec);
                mux_tout = MuxSet;
                Nro_Disp = 4;
            }
        }
            break;
        case 4:
        {
            if (!mux_tout) {
                Send_Disp(4, Uni);
                mux_tout = MuxSet;
                Nro_Disp = 1;
            }
        }
            break;
        default:
        {
            mux_tout = MuxSet;
            Nro_Disp = 1;
        }
    }
}

unsigned char Botones13() {
    /*esta funcion es funcional unicamente para el shield o placa 1.3
     *Declara el estado de cada uno de los botones que puede ser
     *Boton_Libre
     *Boton_Espera
     *Boton_Detecta
     *Boton_Libera
     */
    unsigned char Boton_Presionado = NoPresiona;

    static unsigned char Estado_BOT = BotonLibre;


    switch (Estado_BOT) {
        case BotonLibre:
            if (!BOT1 || !BOT2 || !BOT3 || !BOT4) {
                Estado_BOT = BotonEnEspera;
                bot_tout = BotonReleaseSet;
            }
            break;
        case BotonEnEspera:
            if (!bot_tout) Estado_BOT = BotonDetectado;
            break;
        case BotonDetectado:
            if (!BOT4)Boton_Presionado = 4;
            else if (!BOT3)Boton_Presionado = 3;
            else if (!BOT2)Boton_Presionado = 2;
            else if (!BOT1)Boton_Presionado = 1;
            else Boton_Presionado = NoPresiona;

            bot_tout = BotonReleaseSet;
            Estado_BOT = BotonEsperaLiberarce;

            break;
        case BotonEsperaLiberarce:
            if (BOT1 && BOT2 && BOT3 && BOT4 && !bot_tout) Estado_BOT = BotonLibre;
            break;
        default:
            Boton_Presionado = NoPresiona;
            Estado_BOT = BotonLibre;
            break;

    }
    return Boton_Presionado;


}

unsigned char debounce(unsigned char sample)
/*recibe la variable sample que contiene 8 bits y devuelve el estado de los
 * pulsadores
 */ {
    static unsigned char state, cnt0, cnt1;
    unsigned char delta;
    delta = (sample) ^ state;
    cnt1 = (cnt1 ^ cnt0) & delta;
    cnt0 = ~cnt0 & delta;
    state ^= delta & (~(cnt0 | cnt1));
    return state;
}

void tic_timer0(void) {
    static unsigned char debounce_tic = DebounceSet;
    if (mux_tout)mux_tout--;
    if (bot_tout)bot_tout--;
    if (led_tout)led_tout--;
    //llama a la funcion debounce si han pasado 
    if (debounce_tic)debounce_tic--;
    else {
        debounce_tic = DebounceSet;
        EstadoDeLosBotones = ((debounce(PORTD))^0xF0)&0xF0;
    }
}

