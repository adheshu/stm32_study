//Where is the led connected?
// PORT: D
//PIN: 13 Orange, 12 Green, 14 Red, 15 Blue

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

#define AHB1EN_R_OFFSET 	(0x30UL)
#define RCC_AHB1EN_R		(*(volatile unsigned int *)(RCC_BASE + AHB1EN_R_OFFSET))

#define MODE_R_OFFSET		(0x00UL)
#define GPIOD_MODE_R 		(*(volatile unsigned int *)(GPIOD_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET			(0x14UL)
#define GPIOD_OD_R 			(*(volatile unsigned int *)(GPIOD_BASE + OD_R_OFFSET))

#define GPIOD_EN			(1U<<3) //shift 1 to position 3 to enable RCC to port D
#define PIN13				(1U<<13) //write 1 at PIN13
#define LED_ORANGE			(PIN13)

int main (void)
{
	/*Step 1: Enable Clock Access to GPIOD*/
	RCC_AHB1EN_R |= GPIOD_EN; // instead of writing =, we write |= so that other bits will remain unchanged

	/*Step 2: Set GPIOD PIN 3 as Output Pin*/
	GPIOD_MODE_R |= (1U<<26); //set only bit 6 to 1, keep other same by |=
	GPIOD_MODE_R &= ~(1U << 27); // set only bit 7 to zero , keep other same by |= ~

	while(1)
	{
		/*Step 3: Write HIGH to GPIOD PIN 3*/
		GPIOD_OD_R |= LED_ORANGE;
	}
}
