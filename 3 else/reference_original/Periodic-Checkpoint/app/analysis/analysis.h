
#ifndef APP_ANALYSIS_ANALYSIS_H_
#define APP_ANALYSIS_ANALYSIS_H_

#include <stdint.h>

void analysis_clear();

void analysis_runtask(uint16_t task_id, uint16_t backup_size);

void analysis_printout();

#endif /* APP_ANALYSIS_ANALYSIS_H_ */
