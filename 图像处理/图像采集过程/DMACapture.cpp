/*DMA初始化*/
void DMA1_Init(void)
{
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMAMASK;
	DMAMUX_CHCFG0 = DMAMUX_CHCFG_SOURCE(51);
	DMA_TCD0_CITER_ELINKNO=DMA_CITER_ELINKNO_CITER(MAXCOLUM);
	DMA_TCD0_BITER_ELINKNO=DMA_BITER_ELINKNO_BITER(MAXCOLUM);
	DMA_TCD0_SADDR=(unit32)&GPIOS_PDIR;
	DMA_TCD0_SOFF=0;
	DMA_TCD0_NBYTES_MLNO=DMA_NBYTES_MLNO_NBYTES(1);
	DMA_TCD0_SLAST=0;
	DMA_TCD0_DLASTSGA=0;
	DMA_TCD0_DADDR=(uint32)pix[0];
	DMA_TCD0_DOFF=1;
	DMA_TCD0_ATTR=DMA_ATTR_SSIZE(0)+DMA_ATTR_DSIZE(0);
	DMA_TCD0_CSR=DMA_CSR_DREQ_MASK;
	//DMA_TCD0_CSR |= DMA_CSR_INTMAJOR_MASK;
}
/*场中断服务子程序*/
void HREF_isr(void)
{
	PORTE_PCR0|=PORT_PCR_ISF_MASK;
	row=0;
	rownum=0;
	disable=irq(91);
	enable_irq(87);
}
/*行中断服务子程序*/
viod HSYNC_isr()
{
	PORTA_PCR16 |= PORT_PCR_ISF_MASK;
	if(rownum++>15){
		if(rownum%4==0){
			DMA_TCD0_CSR |= DMA_CSR_INTMAJOR_MASK;
			DMA_ERQ |= DMA_ERQ0_MASK;
			row++;
			if(row==MAXLINE){
				disable_irq(87);
				enable_irq(91);
				change_signal=1;
			}
		}
	}
}