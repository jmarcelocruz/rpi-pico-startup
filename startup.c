#include <stdint.h>

#include <hardware/regs/clocks.h>
#include <hardware/regs/resets.h>
#include <hardware/regs/rosc.h>
#include <hardware/regs/scb.h>
#include <hardware/regs/ssi.h>
#include <hardware/regs/systick.h>
#include <hardware/regs/watchdog.h>
#include <hardware/regs/xosc.h>

__attribute__((always_inline)) static inline void configure_xip(void) {
    /* TODO: optimize for Pico's Quad-SPI flash */
    SSI_HW->ssienr = 0;
    SSI_HW->baudr = 2;
    SSI_HW->ctrlr[0] = (0U << 21) | ((32 - 1) << 16) | (3U << 8);
    SSI_HW->ctrlr[1] = (0 << 0);
    SSI_HW->spi_ctrlr0 = (3U << 24) | (6U << 2) | (2U << 8) | (0U << 0);
    SSI_HW->ssienr = 1;
}

__attribute__((always_inline)) static inline void configure_vector_table(void) {
    extern void (*vectors[])(void);
    SCB_HW->vtor = (uint32_t)vectors;
}

static void configure_clocks(void) {
    /* enable XOSC */
    XOSC_HW->startup = 47; /* results in just over 1ms delay for the reference 12MHz crystal */
    XOSC_HW->ctrl = (XOSC_HW->ctrl & ~(0xFFF << 12)) | (0xFAB << 12);
    while ((XOSC_HW->status & (1U << 31)) == 0) {
        ;
    }
    /* switch clk_ref source to XOSC */
    CLOCKS_HW->ref.ctrl = (CLOCKS_HW->ref.ctrl & ~(3U << 0)) | (2U << 0);
    while(CLOCKS_HW->ref.selected == 0) {
        ;
    }
    /* stop ROSC */
    ROSC_HW->ctrl = (ROSC_HW->ctrl & ~(0xFFF << 12)) | (0xD1E << 12);
    /* configure peripheral clock to run off of XOSC */
    CLOCKS_HW->peri.ctrl = (1U << 11) | (0x4 << 5);
    /* configure watchdog tick generation */
    WATCHDOG_HW->tick = (1U << 9) | (12 << 0);
    /* configure systick for 1ms tick, using watchdog tick as a reference */
    SYSTICK_HW->rvr = 999; /* count to 0 every 1000 reference clock cycles */
    SYSTICK_HW->cvr = 0;
    SYSTICK_HW->csr = (0U << 2) | (1U << 1) | (1U << 0);
}

__attribute__((section(".stage2"))) void _reset(void) {
    /* load .data section from flash into ram */
    extern uint32_t _erodata, _sdata, _edata;
    for (uint32_t *src = &_erodata, *dst = &_sdata; dst < &_edata; src++, dst++) {
        *dst = *src;
    }
    /* zero .bss section */
    extern uint32_t _sbss, _ebss;
    for (uint32_t *dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0;
    }

    configure_xip();
    configure_vector_table();
    configure_clocks();

    extern void main(void);
    main();
}

__attribute__((weak)) void NMI_Handler(void);
__attribute__((weak)) void HardFault_Handler(void);
__attribute__((weak)) void SVCall_Handler(void);
__attribute__((weak)) void PendSV_Handler(void);
__attribute__((weak)) void SysTick_Handler(void);

__attribute__((weak)) void Timer0_IRQ_Handler(void);
__attribute__((weak)) void Timer1_IRQ_Handler(void);
__attribute__((weak)) void Timer2_IRQ_Handler(void);
__attribute__((weak)) void Timer3_IRQ_Handler(void);
__attribute__((weak)) void PWM_IRQ_Handler(void);
__attribute__((weak)) void XIP_IRQ_Handler(void);
__attribute__((weak)) void PIO0_0_IRQ_Handler(void);
__attribute__((weak)) void PIO0_1_IRQ_Handler(void);
__attribute__((weak)) void PIO1_0_IRQ_Handler(void);
__attribute__((weak)) void PIO1_1_IRQ_Handler(void);
__attribute__((weak)) void DMA0_IRQ_Handler(void);
__attribute__((weak)) void DMA1_IRQ_Handler(void);
__attribute__((weak)) void IO_Bank0_IRQ_Handler(void);
__attribute__((weak)) void IO_QSPI_IRQ_Handler(void);
__attribute__((weak)) void SIO_Proc0_IRQ_Handler(void);
__attribute__((weak)) void SIO_Proc1_IRQ_Handler(void);
__attribute__((weak)) void Clocks_IRQ_Handler(void);
__attribute__((weak)) void SPI0_IRQ_Handler(void);
__attribute__((weak)) void SPI1_IRQ_Handler(void);
__attribute__((weak)) void UART0_IRQ_Handler(void);
__attribute__((weak)) void UART1_IRQ_Handler(void);
__attribute__((weak)) void ADC_IRQ_Handler(void);
__attribute__((weak)) void I2C0_IRQ_Handler(void);
__attribute__((weak)) void I2C1_IRQ_Handler(void);
__attribute__((weak)) void RTC_IRQ_Handler(void);

extern void _estack(void);
__attribute__((section(".vectors"))) void (*vectors[])(void) = {
    _estack, _reset, NMI_Handler, HardFault_Handler, 0, 0, 0, 0, 0, 0, 0,
    SVCall_Handler, 0, 0, PendSV_Handler, SysTick_Handler, Timer0_IRQ_Handler,
    Timer1_IRQ_Handler, Timer2_IRQ_Handler, Timer3_IRQ_Handler, PWM_IRQ_Handler,
    XIP_IRQ_Handler, PIO0_0_IRQ_Handler, PIO0_1_IRQ_Handler, PIO1_0_IRQ_Handler,
    PIO1_1_IRQ_Handler, DMA0_IRQ_Handler, DMA1_IRQ_Handler, IO_Bank0_IRQ_Handler,
    IO_QSPI_IRQ_Handler, SIO_Proc0_IRQ_Handler, SIO_Proc1_IRQ_Handler,
    Clocks_IRQ_Handler, SPI0_IRQ_Handler, SPI1_IRQ_Handler, UART0_IRQ_Handler,
    UART1_IRQ_Handler, ADC_IRQ_Handler, I2C0_IRQ_Handler, I2C1_IRQ_Handler,
    RTC_IRQ_Handler
};
