#include <stdlib.h>
#include "dg_init.h"
#include "dg_logger.h"

int
main (int argc, char **argv){

  int result = dg_init(argc, argv);

  log_close();

  exit (result);
}
