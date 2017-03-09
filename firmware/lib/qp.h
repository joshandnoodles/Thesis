/* 
 * File:        qp.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef QP_H
#define	QP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// maximum size allowed for bulk ADC buffer
#define QP_BULK_BUFFER_SIZE         4096

// variables available to external modules
uint16_t qpLastCh1VSenseReg;
uint16_t qpLastCh2VSenseReg;
uint16_t qpLastCh3VSenseReg;
uint16_t qpLastCh4VSenseReg;
uint16_t qpBulkBuffer[QP_BULK_BUFFER_SIZE];
uint32_t qpBulkBufferIdx;
int16_t qpAlignmentXAxis;
int16_t qpAlignmentYAxis;

void initQp( void );
float qpReadCh1VSense( void );
uint16_t qpReadCh1VSenseReg( void );
float qpReadCh2VSense( void );
uint16_t qpReadCh2VSenseReg( void );
float qpReadCh3VSense( void );
uint16_t qpReadCh3VSenseReg( void );
float qpReadCh4VSense( void );
uint16_t qpReadCh4VSenseReg( void );
char qpAlign( void );
uint16_t * qpReadAllBulk( uint32_t );

// static function prototypes

#ifdef	__cplusplus
}
#endif

#endif	/* QP_H */