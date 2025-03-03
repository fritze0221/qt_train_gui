
/*
 *	hwp-conf.h
 *
 *	this file provides the interface to the actual hardware.
 *	it basically consists of a few definitions, such as port
 * 	addresses, a list of pre-configured ADCs, a list of
 *	pre-configured DACs.
 *
 *	this file is intendes to be included in hw-ports.c,
 *	io-board.c, and sw-ports.c all these modules ought to
 *	provide data structures with at least one entry per
 *	available hardware port.
 *
 */

#define HWP_ADDRLIST	{ 27, 34,25,26 } // Hier muessen alle Hardware-Adressen rein, die genutzt werden

#define HWP_ADCLIST	{1} //{ 4, 5 }	// Gibt an, welches Element aus HWP_ADDRLIST ein ADC ist |  list of hardware A/D controllers
#define HWP_DACLIST	{} //{ 4, 5 }	// list of hardware D/A controllers

#define HWP_SIZE	(sizeof((int []) HWP_ADDRLIST)/sizeof( int ))


