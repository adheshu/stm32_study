//Where is the led connected?
// PORT: D
//PIN: 13 Orange, 12 Green, 14 Red, 15 Blue

#include <stdint.h>

#define PERIPH_BASE			(0x40000000UL) // Get it from data sheet, memory map section

/*Locate AHB1 Peripherals (same as start of GPIOA) start address, it starts at  0x40020000
If we subtract 0x40020000 from 0x40000000,
we get the offset between Peripheral address and start of AHB1 Peripherals 0x20000
(Its OK if we neglect initial zeros)*/

#define AHB1PERIPH_OFFSET 	(0x20000UL)
/*Add 0x20000 to PERIPH_BASE to locate AHB1PERIPH_BASE*/
#define AHB1PERIPH_BASE 	(AHB1PERIPH_OFFSET + PERIPH_BASE)

/*Locate the port GPIOD and to calculate the offset from AHB1 Peripherals
 * Subtract GPIOD port address 0x40021000 from AHB1 Peripherals address 0x40020000 = 0xC00*/
#define GPIOD_OFFSET 		(0xC00UL)
/*Add C00 to AHB1 Peripherals address (AHB1PERIPH_BASE) to locate GPIOD_BASE*/
#define GPIOD_BASE			(AHB1PERIPH_BASE + GPIOD_OFFSET)

/*RCC: Reset and Clock Control is used to enable the clock to peripherals*/
#define RCC_OFFSET 			(0x3800UL)
#define RCC_BASE 			(AHB1PERIPH_BASE + RCC_OFFSET)

#define GPIOD_EN			(1U<<3) //shift 1 to position 3
#define PIN13				(1U<<13) //write 1 at PIN13
#define LED_ORANGE			(PIN13)

#define PIN15				(1U<<15) //write 1 at PIN15
#define LED_BLUE			(PIN15)

#define __IO	volatile //MISRA C suggests to use __IO instead of volatile

typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t DUMMY[4]; /*Added 4(uint32_t)*4(4 elements) = 16 DUMMY bytes to arrange the memory according to ref. manual*/
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
 } GPIO_TypeDef;

 typedef struct
 {
   __IO uint32_t DUMMY[12]; /*Added 12*4 DUMMY bytes to arrange the memory according to ref. manual*/
   __IO uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
 } RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE) //The address, RCC_BASE is a pointer to a RCC_TypeDef structure
#define GPIOD ((GPIO_TypeDef*)GPIOD_BASE)// The address, GPIOD_BASE is a pointer to a GPIO_TypeDef structure

int main (void)
{
	/*Step 1: Enable Clock Access to GPIOD*/

	RCC->AHB1ENR |= GPIOD_EN; // instead of writing =, we write |= so that other bits will remain unchanged

	/*Step 2: Set GPIOD PIN 3 as Output Pin*/

	GPIOD->MODER |= (1U<<26); //set only bit 26 to 1, keep other same by |=
	GPIOD->MODER &= ~(1U << 27); // set only bit 27 to zero , keep other same by |= ~

	GPIOD->MODER |= (1U<<30); //set only bit 30 to 1, keep other same by |=
	GPIOD->MODER &= ~(1U << 31); // set only bit 31 to zero , keep other same by |= ~

	while(1)
	{
		/*Step 3: Write HIGH to GPIOD PIN 13&15*/

		GPIOD->ODR ^= LED_ORANGE;
		for(int i=0; i<100000; i++){} // to insert delay
		GPIOD->ODR ^= LED_BLUE;
		for(int i=0; i<100000; i++){} // to insert delay
	}
}
