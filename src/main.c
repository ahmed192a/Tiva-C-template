#include "tm4c123gh6pm.h"
#include <stdint.h>


void strign_print(char * data){
    while(*data != '\0'){
        UART0_DR_R = *data; // load data
        while((UART0_FR_R & 0x20) != 0); // wait until TXFF is 0
        data++;
    }
}

// this program will enanle the uart and print the string "Hello World" on the qemu terminal emulator

int main()
{
    // enable PLL and set the freq to 16 MHz
    SYSCTL_RCC2_R |= 0x80000000; // RCC2
    SYSCTL_RCC2_R |= 0x800; // BYPASS2, PLL bypass
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x7C0) + 0x540; // XTAL = 10101, 16 MHz
    SYSCTL_RCC2_R &= ~0x70; // configure for main oscillator source
    SYSCTL_RCC2_R &= ~0x2000; // activate PLL by clearing PWRDN
    SYSCTL_RCC2_R |= 0x40000000; // use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22); // set the desired system divider to 5
    while((SYSCTL_RIS_R & 0x40) == 0); // wait for the PLL to lock by polling PLLLRIS
    SYSCTL_RCC2_R &= ~0x800; // enable PLL by clearing BYPASS
    // enable the timer 0
    SYSCTL_RCGCTIMER_R |= 0x01; // enable clock to Timer Block 0
    TIMER0_CTL_R &= ~0x01; // disable Timer before initialization
    TIMER0_CFG_R = 0x04; // 16-bit option
    TIMER0_TAMR_R = 0x02; // periodic mode and down-counter
     // Timer A interval load value register which is 1 sec
    TIMER0_TAILR_R = 16000000 - 1; // Timer A interval load value register
    TIMER0_ICR_R = 0x01; // clear the TimerA timeout flag
    TIMER0_CTL_R |= 0x01; // enable Timer A after initialization
    // enable the uart

    SYSCTL_RCGCUART_R |= 0x01; // enable clock to UART0
    SYSCTL_RCGCGPIO_R |= 0x01; // enable clock to PORTA
    UART0_CTL_R &= ~0x01; // disable UART
    UART0_IBRD_R = 104; // IBRD = int(16,000,000 / (16 * 9600)) = int(104.1667)
    UART0_FBRD_R = 11; // FBRD = int(0.1667 * 64 + 0.5) = 11
    UART0_LCRH_R = 0x70; // 8-bit, no parity bits, one stop, no FIFOs
    UART0_CTL_R |= 0x301; // enable UART, TXE, RXE
    GPIO_PORTA_AFSEL_R |= 0x03; // enable alt funct on PA1-0
    GPIO_PORTA_DEN_R |= 0x03; // enable digital I/O on PA1-0
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011; // configure PA1-0 as UART
    GPIO_PORTA_AMSEL_R = 0; // disable analog functionality on PA

    while(1)
    {
        strign_print("Hello World\n\r");

        for (uint32_t i = 0; i < 999999; i++); // delay for 1 sec
        for (uint32_t i = 0; i < 999999; i++); // delay for 1 sec
        for (uint32_t i = 0; i < 999999; i++); // delay for 1 sec
        for (uint32_t i = 0; i < 999999; i++); // delay for 1 sec


        // // use Timer 0 to wait for 1 sec
        // while((TIMER0_RIS_R & 0x01) == 0); // wait for TimerA timeout flag to set
        // TIMER0_ICR_R = 0x01; // clear the TimerA timeout flag

    }
}

