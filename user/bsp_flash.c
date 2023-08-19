#include "bsp_flash.h"
 
FLASH_ProcessTypeDef p_Flash; 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];
 

u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
 
#if STM32_FLASH_WREN	   
 
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
	    WriteAddr+=2;
	}  
} 

void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   
	u16 secoff;	   
	u16 secremain; 	   
 	u16 i;    
	u32 offaddr;   
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;
	
	HAL_FLASH_Unlock();					    
	offaddr=WriteAddr-STM32_FLASH_BASE;		
	secpos=offaddr/STM_SECTOR_SIZE;			
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		
	secremain=STM_SECTOR_SIZE/2-secoff;		   
	if(NumToWrite<=secremain)secremain=NumToWrite;
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		for(i=0;i<secremain;i++)	
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break; 	  
		}
		if(i<secremain)				
		{
			Flash_PageErase(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);	
			FLASH_WaitForLastOperation(FLASH_WAITETIME);            	
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);							
																		
			for(i=0;i<secremain;i++)
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);  
		}else 
		{
			FLASH_WaitForLastOperation(FLASH_WAITETIME);       	
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain); 
		}
		if(NumToWrite==secremain)break;
		else
		{
			secpos++;				
			secoff=0;				 	 
		   	pBuffer+=secremain;  	
			WriteAddr+=secremain*2;		   
		   	NumToWrite-=secremain;	
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;
			else secremain=NumToWrite;
		}	 
	};	
	HAL_FLASH_Lock();		
}
#endif

void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);
		ReadAddr+=2;	
	}
}
 
 
void Flash_PageErase(uint32_t PageAddress)
{
  /* Clean the error context */
  p_Flash.ErrorCode = HAL_FLASH_ERROR_NONE;
 
#if defined(FLASH_BANK2_END)
  if(PageAddress > FLASH_BANK1_END)
  { 
    /* Proceed to erase the page */
    SET_BIT(FLASH->CR2, FLASH_CR2_PER);
    WRITE_REG(FLASH->AR2, PageAddress);
    SET_BIT(FLASH->CR2, FLASH_CR2_STRT);
  }
  else
  {
#endif /* FLASH_BANK2_END */
    /* Proceed to erase the page */
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    WRITE_REG(FLASH->AR, PageAddress);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
#if defined(FLASH_BANK2_END)
 
  }
#endif /* FLASH_BANK2_END */
}

void set_default_mqvalue(void)
{
	uint16_t mq4_1 = 10000;
	uint16_t mq4_2 = 10000;
	uint16_t mq4_3 = 10000;
	Flash_PageErase(MQ4_1_ADDR);
	Flash_PageErase(MQ4_2_ADDR);
	Flash_PageErase(MQ4_3_ADDR);
	STMFLASH_Write(MQ4_1_ADDR,&mq4_1,2);
	STMFLASH_Write(MQ4_2_ADDR,&mq4_2,2);
	STMFLASH_Write(MQ4_3_ADDR,&mq4_3,2);
}
