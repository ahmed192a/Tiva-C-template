#include <stdint.h>
#include "tm4c123gh6pm.h"

void GPIO_init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;  // enable Port F
    GPIO_PORTF_DIR_R |= 0x02;   // make PF1 output
    GPIO_PORTF_DEN_R |= 0x02;   // enable digital function on PF1
}

void Timer0_init(void) {
    SYSCTL_RCGCTIMER_R |= 0x01;  // enable Timer 0
    TIMER0_CTL_R &= ~0x01;       // disable Timer 0A
    TIMER0_CFG_R = 0x00000000;   // set 32-bit mode
    TIMER0_TAMR_R = 0x00000002;  // periodic mode
    TIMER0_TAILR_R = 16000000;   // 1 sec @ 16 MHz clock
    TIMER0_ICR_R |= 0x01;        // clear Timer 0A timeout flag
    TIMER0_IMR_R |= 0x01;        // enable Timer 0A timeout interrupt
    NVIC_EN0_R |= 1 << 19;       // enable interrupt 19 in NVIC
    TIMER0_CTL_R |= 0x01;        // enable Timer 0A
}

void strign_print(char * data){
    while(*data != '\0'){
        UART0_DR_R = *data; // load data
        while((UART0_FR_R & 0x20) != 0); // wait until TXFF is 0
        data++;
    }
}


volatile uint32_t counter = 0;

void Timer0A_Handler(void) {
    TIMER0_ICR_R |= 0x01;  // clear the interrupt flag
    counter++;
    if (counter == 1) {
        GPIO_PORTF_DATA_R ^= 0x02;  // toggle LED
        counter = 0;
        strign_print("Hello World\n\r");

    }
}

int main(void) {
    GPIO_init();

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
    Timer0_init();

    while (1) {
        // main loop
    }
    return 0;
}


// int main() {
//     // Enable the GPIO port F peripheral
//     SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
//     // Delay for clock stabilization
//     volatile uint32_t delay = SYSCTL_RCGCGPIO_R;

//     // Set PF0 as a digital output
//     GPIO_PORTF_DIR_R |= 0x01;
//     GPIO_PORTF_DEN_R |= 0x01;

//     while(1) {
//         // Toggle the PF0 output
//         GPIO_PORTF_DATA_R ^= 0x01;
//         // Delay for some time
//         for (delay = 0; delay < 1000000; delay++);
//         // printf("hello\n");
//     }

//     return 0;
// }

// #include "tm4c123gh6pm.h"

// // this program will toggle the led on the board every 1 second
// void setup(){
//     SYSCTL_RCGCGPIO_R |= 0x20; // enable clock to PORTF
//     GPIO_PORTF_DIR_R |= 0x02; // set PF1 as output
//     GPIO_PORTF_DEN_R |= 0x02; // enable digital I/O on PF1
//     GPIO_PORTF_DATA_R |= 0x02; // set PF1
// }


// int main()
// {
//     setup();
//     while(1)
//     {
//         GPIO_PORTF_DATA_R ^= 0x02; // toggle PF1
//         // wait 1 second
//         for(int i = 0; i < 1000000; i++);
//     }
//     return 0;
// }