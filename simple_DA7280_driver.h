/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
#define DA7280_ADDR_8BIT                                (0x94)


#define ENABLE_DA7280_LOG                               (0)
/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void DA7280_PlaybackIndex(unsigned char idx , unsigned char loop , unsigned char enableContinue);

