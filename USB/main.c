#include "stm32l476xx.h"
#include "lcd.h"

void usb_ctrl_pins_init(void);
void msi48_calibrate(void);
void usb_clk_init(void);
void usb_otg_core_init(void);
int usb_otg_device_init(void);
void usb_otg_endpt_init(void);
void ch2_init(void);

int main(void)
{
	usb_ctrl_pins_init();
	LCD_Initialization();
	LCD_Clear();
	usb_clk_init();
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER2;
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
	usb_otg_core_init();
	int clkspd = usb_otg_device_init();	
	if (clkspd == 48) {
		LCD_DisplayString((uint8_t *)"48MHz");	
	}
	GPIOB->ODR |= GPIO_ODR_ODR_2;
	
	// Turn on V_BUS
	GPIOC->ODR |= GPIO_ODR_ODR_11;
	
	ch2_init();

	while (1){
	}
}

void msi48_calibrate(void)
{

	// Turn on LSE 32.768 kHz external oscillator for MSI calibration
//	RCC->BDCR |= RCC_BDCR_LSEON;

	// Wait until the LSE is ready
//	while (RCC->BDCR & RCC_BDCR_LSERDY == 0);
	
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= 0x0B << 4;  // 48 MHz

	// Indicate that MSIRANGE value in RCC_CR will be used
	RCC->CR |= RCC_CR_MSIRGSEL;

	// Enable MSI PLL mode
	RCC->CR |= RCC_CR_MSIPLLEN;

	// Enable MSI clock
	RCC->CR |= RCC_CR_MSION;

	// Wait until MSI is ready
	while (RCC->CR & RCC_CR_MSIRDY == 0);
}

void usb_clk_init(void)
{
	msi48_calibrate();

	// Select MSI 48 MHz as the USB clock
	RCC->CCIPR &= ~RCC_CCIPR_CLK48SEL;
	RCC->CCIPR |= 0x03 << 26;
}

void usb_ctrl_pins_init(void)
{
	// OTG_FS_VBUS (PC11)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODER11;	
	GPIOC->MODER |= GPIO_MODER_MODER11_0; // dig. output

	// OTG_FS_ID (PC12)
	GPIOC->MODER &= ~GPIO_MODER_MODER12;
	GPIOC->MODER |= GPIO_MODER_MODER12_1;
	GPIOC->AFR[1] &= ~GPIO_AFRH_AFRH5;
	GPIOC->AFR[1] |= 10 << (5 * 4);

	// OTG_FS_DM (D-) (PA11)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER11;
	GPIOA->MODER |= GPIO_MODER_MODER11_1;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH4;
	GPIOA->AFR[1] |= 10 << (4 * 4);

	// OTG_FS_DP (D+) (PA12)
	GPIOA->MODER &= ~GPIO_MODER_MODER12;
	GPIOA->MODER |= GPIO_MODER_MODER12_1;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH5;
	GPIOA->AFR[1] |= 10 << (5 * 4);
}

void usb_otg_core_init(void)
{
	/* Exit power-down mode */
	USB_OTG_FS->GCCFG &= ~USB_OTG_GCCFG_PWRDWN;

	/* Enable session request protocol (SRP) */
	USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_SRPCAP;

	/* (1) */

	// global interrupt mask
	USB_OTG_FS->GAHBCFG |= USB_OTG_GAHBCFG_GINT;
	
	// Rx FIFO non-empty
	USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_RXFLVL;

	// interrupt indicates that Periodic Tx FIFO is completely empty
	USB_OTG_FS->GAHBCFG |= USB_OTG_GAHBCFG_PTXFELVL;


	/* (2) */
	
	// HNP capable
	USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_HNPCAP;
	
	// SRP capable
	USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_SRPCAP;

	// Set timeout calibration
	USB_OTG_FS->GUSBCFG &= ~USB_OTG_GUSBCFG_TOCAL;
	USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_TOCAL_0;

	// Set USB turnaround time
	USB_OTG_FS->GUSBCFG |= 0xF << 10;


	/* (3) */
	
	// Unmask OTG interrupts	
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_OTGINT;	

	// Unmask mode mismatch interrupts
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_MMISM;

	
	/* (4) */
	if (USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD) {
		LCD_DisplayString((uint8_t *)"Host");
	} else {
		LCD_DisplayString((uint8_t *)"Device");
	}
	
}

int usb_otg_device_init(void)
{

	/* (1) */

	// full speed (USB 1.1 transceiver clock is 48 MHz)	
	USB_DEV->DCFG |= USB_OTG_DCFG_DSPD_0 | USB_OTG_DCFG_DSPD_1;
	
	// Send nonzero-length status OUT handshake
	USB_DEV->DCFG &= ~USB_OTG_DCFG_NZLSOHSK;


	/* (2) */

	// Unmask bits in OTG_GINTMSK	
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_USBRST;  // usb reset
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_ENUMDNEM;  // enumeration done
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_ESUSPM;  // early suspend
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_USBSUSPM;  // USB suspend
	USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_SOFM;  // start of frame
	

	/* (3) */

	// Enable V_BUS sensing in "B" device mode and supply 5 volts accors the pull-up resistor on D+ line
	USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_VBDEN;


	/* (4) */
	RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;	
	// Wait for USBRST interrupt in OTG_GINTSTS
	while (USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_USBRST);

	// Wait for enumeration to complete
	while (USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_ENUMDNE);	

	// Read OTG_DSTS reg. to determine enumeration speed
	if (USB_DEV->DSTS & USB_OTG_DSTS_ENUMSPD == 0x03) {
		return 48;
	} else {
		return -1;
	}
}

void usb_otg_endpt_init(void)
{

}

void ch2_init(void)
{
	// Host must perform transfer every odd frame	
	USB_HOST_CH_2->HCCHAR |= USB_OTG_HCCHAR_ODDFRM;	
	
	// Enable USB host channel 2
	USB_HOST_CH_2->HCCHAR |= USB_OTG_HCCHAR_CHENA;	
}
