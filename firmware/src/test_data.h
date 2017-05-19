/* 
 * File:        test_data.h
 * Author:      Josh
 * Created:     !!
 * Revisions:
 *  !!
 * Description: 
 *  !!
 */

#ifndef TEST_DATA_H
#define	TEST_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

// variables available to external modules
extern uint8_t testDataRand256BytesNoReplacement[];
extern uint8_t testDataSeq256BytesNoReplacement[];
extern uint8_t testDataRand256BytesWithReplacement[];


#ifdef	__cplusplus
}
#endif

#endif	/* TEST_DATA_H */