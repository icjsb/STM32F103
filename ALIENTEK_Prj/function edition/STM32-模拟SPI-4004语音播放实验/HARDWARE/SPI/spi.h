#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define	SS          PBout(9)  //	
#define	SCLK        PBout(13)  //	
#define	MOSI        PBout(15)  //	

void SPI1_Init(void);
void spi_send(u8 isdx);
void isd_stop(void);
void isd_pu(void);
void isd_pd(void);
void isd_play(void);
void isd_setplay(u8 adl,u8 adh);
void bofan(u8 mun);


#endif

