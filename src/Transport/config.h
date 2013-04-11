#ifndef _CONFIG_H
#define _CONFIG_H

#include <daffie/event.h>
#include "efievent.h"
#include "agentd.h"
#include "cl.h"

#include "CEarlabDataStream.h"

#define CONFIG_NAMELEN EFI_NAMESIZE
#define CONFIG_PATHLEN AGENTD_PATHLEN

typedef struct {
	char    instname[CONFIG_NAMELEN];       /* Module instance name   */
	char    modname[CONFIG_NAMELEN];        /* Module executable name */
	char    host[CONFIG_NAMELEN];           /* Host name              */
	char    paramf[CONFIG_PATHLEN];         /* Parameter file name    */
	char    insrc[MAX_ARGS][CONFIG_NAMELEN];/* Input source name */
	int     clientid;                       /* DAFFIE client id       */
	int     index;                          /* Index of this entry    */

	int     noutput;                        /* Number of outputs      */
	int     out_type[MAX_ARGS];             /* Stream type of outputs */
	int     out_dim[MAX_ARGS];              /* Dimens. of each output */
	int     out_rank[MAX_ARGS][MAX_DIM];    /* Rank for each dimen.   */
	int     out_desc[MAX_ARGS][MAX_DIM];    /* Output array descriptor*/
	int     out_size[MAX_ARGS];             /* Max elem in each output*/
	int     out_dst[MAX_ARGS];              /* Output destination     */
	int     out_pos[MAX_ARGS];              /* Arg position of above  */
	int     out_tag[MAX_ARGS];              /* Output tag id          */

	int     ninput;                         /* Number of inputs       */
	int     in_type[MAX_ARGS];              /* Stream type of inputs  */
	int     in_dim[MAX_ARGS];               /* Dimens. of each input  */
	int     in_rank[MAX_ARGS][MAX_DIM];     /* Rank for each dimen.   */
	int     in_desc[MAX_ARGS][MAX_DIM];     /* Input array descriptor */
	int     in_size[MAX_ARGS];              /* Max elem in each input */
	int     in_src[MAX_ARGS];               /* Input source mod       */
	int     in_pos[MAX_ARGS];               /* Arg position of above  */
	int     in_tag[MAX_ARGS];               /* Input tag id           */

} MODULE_CONFIG;

EXTERNAL int  read_config(char *config, MODULE_CONFIG *mods);
EXTERNAL int  wire_config(int nmods, MODULE_CONFIG *mods);
EXTERNAL void config_error(FILE *f, char *prefix);
EXTERNAL void print_config(FILE *f, MODULE_CONFIG modules[], int nmods);
EXTERNAL int  find_module_byname(char *name, int nmods, MODULE_CONFIG modules[]);
EXTERNAL int  find_module_byclientid(int id, int nmods, MODULE_CONFIG modules[]);


#endif
