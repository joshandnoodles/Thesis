/* 
 * File:        usb_handler.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  The following file contains the main receiver and transmitter handler 
 *  functions for processing USB HID communication buffers. These contain the 
 *  functionality to break down received buffers as well as build new ones to be 
 *  sent to the USB host. Any core USB operations should be externalized in 
 *  other files. 
 */

#ifndef USB_HANDLER_H
#define	USB_HANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif

// function prototypes
void usbHandler( unsigned char *, unsigned char *, unsigned int );
void _insertTxBufUnsigned( unsigned char *, unsigned char );
void insertTxBufUnsignedChar( unsigned char *, unsigned char );
void insertTxBufUnsignedInt( unsigned char *, unsigned int );
void insertTxBufUnsignedLong( unsigned char *, unsigned long );

#ifdef	__cplusplus
}
#endif

#endif	/* USB_HANDLER_H */