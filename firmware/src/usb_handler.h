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
void usbHandler( uint8_t *, uint8_t *, uint16_t );
void _insertTxBufUnsigned( uint8_t *, uint8_t );
void insertTxBufUnsignedChar( uint8_t *, uint8_t );
void insertTxBufUnsignedInt( uint8_t *, uint16_t );
void insertTxBufUnsignedLong( uint8_t *, uint32_t );

#ifdef	__cplusplus
}
#endif

#endif	/* USB_HANDLER_H */