
#include <board/driverlib_include.h>
#include <scheduling/integrated_task.h>

void cpu_copy(uint16_t* from, uint16_t* to, uint16_t size)
{
    while (size > 0) { *to++ = *from++; size--; }
}


void dma_prepare(unsigned long from, unsigned long to, uint16_t size)
{
#if defined(__MSP430FR5969__) || defined(__MSP430FR5994__)
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) &= ~DMAEN;
    __data16_write_addr((unsigned short)(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0SA), from);
    __data16_write_addr((unsigned short)(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0DA), to);
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0SZ) = size;
    HWREG16(DMA_BASE + DMA_CHANNEL_0 + OFS_DMA0CTL) |= DMAEN;
#else
    return;
#endif
}


