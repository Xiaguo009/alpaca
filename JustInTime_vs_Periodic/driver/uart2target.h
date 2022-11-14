
#ifndef DRIVER_UART2TARGET_H_
#define DRIVER_UART2TARGET_H_

#include <driver/driverlib_include.h>
#include <driver/nv.h>
#include <driver/target.h>
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

/* P2.0 UCA0TXD/UCA0SIMO P2SEL1.x=1, P2SEL0.x=0
 * P2.1 UCA0RXD/UCA0SOMI P2SEL1.x=1, P2SEL0.x=0
 */

#if defined(__MSP430FR5994__)
    #define UART_TX_PORT            GPIO_PORT_P2
    #define UART_TX_PIN             GPIO_PIN0
    #define UART_TX_FUNCTION        GPIO_SECONDARY_MODULE_FUNCTION
    #define UART_BASEADDR           EUSCI_A0_BASE
#elif defined(__MSP430FR5969__)
    #define UART_TX_PORT            GPIO_PORT_P2
    #define UART_TX_PIN             GPIO_PIN5
    #define UART_TX_FUNCTION        GPIO_SECONDARY_MODULE_FUNCTION
    #define UART_BASEADDR           EUSCI_A1_BASE
#elif defined(__MSP430FR2433__)
    #define UART_TX_PORT            GPIO_PORT_P2
    #define UART_TX_PIN             GPIO_PIN6
    #define UART_TX_FUNCTION        GPIO_PRIMARY_MODULE_FUNCTION
    #define UART_BASEADDR           EUSCI_A1_BASE
#else
#error "ERROR: UNSUPPORTED MSP TARGET!"
#endif

#if SMCLK_FREQUENCY_IN_MHZ == 1
#define CLK_PRESCALAR_USED          8
#define FIRST_MODE_REG_USED         0
#define SECOND_MODE_REG_USED        214
#define OVER_SAMP_USED              0
#elif SMCLK_FREQUENCY_IN_MHZ == 2
#define CLK_PRESCALAR_USED          17
#define FIRST_MODE_REG_USED         0
#define SECOND_MODE_REG_USED        74
#define OVER_SAMP_USED              0
#elif SMCLK_FREQUENCY_IN_MHZ == 4
#define CLK_PRESCALAR_USED          2
#define FIRST_MODE_REG_USED         2
#define SECOND_MODE_REG_USED        187
#define OVER_SAMP_USED              1
#elif SMCLK_FREQUENCY_IN_MHZ == 8
#define CLK_PRESCALAR_USED          4
#define FIRST_MODE_REG_USED         5
#define SECOND_MODE_REG_USED        85
#define OVER_SAMP_USED              1
#else
#error "WORKING FREQUENCY ILLEGAL!"
#endif

void uart2target_init();

#endif /* DRIVER_UART2TARGET_H_ */
