
#ifndef DRIVER_NV_H_
#define DRIVER_NV_H_

#define __nv __attribute__((section(".TI.persistent")))

#define __ro_nv __attribute__((section(".ro_nv_vars")))

#endif /* DRIVER_NV_H_ */
