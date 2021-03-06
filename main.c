#include <xc.h>
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000

#define DELAY_IN 2000
#define DELAY_SIGNAL 3000

void init_ports() {
    TRISA = ~0x01; // 0 is out rest is in
    TRISB = 0xFF; 
    PORTA = 0x00;
    PORTB = 0x00;
}

int get_pin(int id) {
    if (id >= 8){
        return (PORTA >> (id-5)) & 1;
    }
    return (PORTB >> id) & 1;
}

void main(void) {
    init_ports();
    unsigned long counters[10];
    int signal_flag = 0;
    int signal_timer = 0;
    
    while (1) {
        for (int i = 0; i < 10; i++) {
            if (!get_pin(i)) {
                counters[i] = 0;
            }
            counters[i]++;
            if (counters[i] >= DELAY_IN) {
                counters[i] = 0;
                signal_flag = 1;
                PORTAbits.RA0 = 1;
            }
        }
        if (signal_flag){
            signal_timer++;
            if (signal_timer >= DELAY_SIGNAL){
                signal_flag = 0;
                signal_timer = 0;
                PORTAbits.RA0 = 0;
            }
        }
    }
    return;
}

