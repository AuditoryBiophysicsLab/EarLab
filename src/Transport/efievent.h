/*
 * DAFFIE events for Earlab Functional Interface
 */
#ifndef _EFI_EVENT_H
#define _EFI_EVENT_H

#include "CMatrixN.h"
#include "CSpike.h"
#include "CEarlabDataStream.h"

#include <daffie/event.h>

#define MAX_DATASIZE         0xffff  /* (64*1024 - 1) */

#define EFI_FILENAME_SIZE       256
#define EFI_NAMESIZE             64
#define EFI_MAXARG               16
#define EFI_MAXDIM               10

/*
 * EFI Dimension descriptors
 */
#define EFI_NONE                   0 /* NONE              */

#define EFI_DIM_SAMPLE             10  /* scalar sample    */
#define EFI_DIM_TIME               20  /* time             */
#define EFI_DIM_CHANNEL            30  /* channels         */
#define EFI_DIM_DELAY              40  /* delays           */
#define EFI_DIM_REPS               50  /* repetitons       */

/* Scalar types */
#define EFI_WAVEFORM_PRESSURE      1  /* micropascals      */
#define EFI_WAVEFORM_DISPLACEMENT  2  /* nanometers        */
#define EFI_WAVEFORM_VOLTAGE       3  /* microvolts        */
#define EFI_NERUON_ST              4  /* spike train times */
#define EFI_NERUON_IFR             5  /* firing rates      */

/*
 * Status messages
 */

/* Report types (type) */
#define EFI_STATUS_LOAD            1 /* Module load             */
#define EFI_STATUS_PARAM           2 /* Module parameters       */
#define EFI_STATUS_CONFIG          3 /* Module configuration    */
#define EFI_STATUS_START           4 /* Module start            */
#define EFI_STATUS_STOP            5 /* Module stop             */
#define EFI_STATUS_ADVANCE         6 /* Module advance          */
#define EFI_STATUS_SEND            7 /* Module send data        */
#define EFI_STATUS_CONTROL         8 /* Msg for control layer   */
#define EFI_STATUS_FRAME           9 /* Frame msg from control  */
#define EFI_STATUS_UNLOAD         10 /* Module unload           */

#define EFI_STATUS_MAXTYPE 10

static char *
StatusTypeMsg[] = {
	"INVALID",
	"load",
	"param",
	"config",
	"start",
	"stop",
	"advance",
	"send",
	"control",
	"frame",
	"unload",
	NULL
};

/* Status indicator (status) */
#define EFI_STATUS_OK              1 /* operation okay          */
#define EFI_STATUS_READY           1 /* module ready (equiv OK) */
#define EFI_STATUS_IDLE            2 /* module is idle          */
#define EFI_STATUS_STARTING        3 /* operation starting      */
#define EFI_STATUS_WAITDATA        4 /* module awaiting data    */
#define EFI_STATUS_INFO            5 /* information             */
#define EFI_STATUS_WARNING         6 /* possible error          */
#define EFI_STATUS_ERROR           7 /* fatal runtime error     */
#define EFI_STATUS_DIED            8 /* client prematurely died */

#define EFI_STATUS_MAXSTATUS EFI_STATUS_DIED

static char *
StatusStatusMsg[] = {
	"UNKNOWN",
	"ready",
	"idle",
	"starting",
	"waitdata",
	"info",
	"warning",
	"run error",
	"died",
};

/*
 * Control types
 */
#define EFI_CTL_QUERY   1
#define EFI_CTL_PROBE   2
#define EFI_CTL_STATUS  3

/*
 * NB in the DATA events:
 *     elems is total elements for this frame which may comprise
 *           multiple events
 *     index0,indexN are element indices for this event
 */
typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             elems;                  /* total elem in stream   */
	int             tag;                    /* stream tag             */
	int             frame;                  /* frame number           */
	short           endian;                 /* 0=little, 1=big        */
	short           type;                   /* data type (wave,spike) */
	int             index0;                 /* first element of data  */
	int             indexN;                 /* last element of data   */
	MATRIXN_T       data[1];                /* data payload           */
} EVENT_EFI_WAVEDATA;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             elems;                  /* total elem in stream   */
	int             tag;                    /* stream tag             */
	int             frame;                  /* frame number           */
	short           endian;                 /* 0=little, 1=big        */
	short           type;                   /* data type (wave,spike) */
	int             index0;                 /* first element of data  */
	int             indexN;                 /* last element of data   */
	TSpike          data[1];                /* data payload           */
} EVENT_EFI_SPIKEDATA;

/*
 * The following is the "untyped" version of an EFI DATA stream
 * are not used as actual events, but only as a placeholder for
 * one of the actual typed events
 */
typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             elems;                  /* number of elements     */
	int             tag;                    /* stream tag             */
	int             frame;                  /* frame number           */
	short           endian;                 /* 0=little, 1=big        */
	short           type;                   /* data type (wave,spike) */
	int             index0;                 /* first element of data  */
	int             indexN;                 /* last element of data   */
	BYTE            data[4];                /* data payload           */
} EVENT_EFI_DATA;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             ninput;                 /* Number of input args   */
	int             input_type[EFI_MAXARG]; /* Types of input args    */
	int             input_rank[EFI_MAXARG][EFI_MAXDIM]; /* Number of elements  */
	int             noutput;                /* Number of outputs      */
	int             output_type[EFI_MAXARG];/* Type of outputs        */
	int             output_rank[EFI_MAXARG][EFI_MAXDIM]; /* Number of elements  */
	int             output_size[EFI_MAXARG];/* Max number of elements */
} EVENT_EFI_START;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             rc;                     /* return code            */
} EVENT_EFI_STOP;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             exitcode;               /* Exit code              */
} EVENT_EFI_UNLOAD;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             frame;                  /* Frame number           */
} EVENT_EFI_ADVANCE;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             type;                   /* Status being reported  */
	int             status;                 /* Status                 */
	int             detail;                 /* Where the devil lives  */
	int             frame;                  /* Frame number           */
	int             timestamp;              /* Timestamp in millisecs */
} EVENT_EFI_STATUS;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	char            filename[EFI_FILENAME_SIZE];
	char            section[EFI_NAMESIZE];
} EVENT_EFI_PARAMETER_FILE;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	char            paramspec[4];           /* parameter spec as a string */
} EVENT_EFI_PARAMETER_SET;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	char            name[EFI_NAMESIZE];     /* Module name            */
	int             ninput;                 /* Number of input args   */
	int             input_type[EFI_MAXARG]; /* Types of input args    */
	int             noutput;                /* Number of outputs      */
	int             output_type[EFI_MAXARG];/* Type of outputs        */
} EVENT_EFI_SPEC;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             ninput;                 /* Number of input args   */
	int             input_type[EFI_MAXARG]; /* Types of input args    */
	int             input_desc[EFI_MAXARG]; /* Input array descriptor */
	int             input_tag[EFI_MAXARG];  /* Tag for input          */
	int             input_from[EFI_MAXARG]; /* Source of input        */
	int             input_rank[EFI_MAXARG][EFI_MAXDIM]; /* Array dims */
	int             input_size[EFI_MAXARG]; /* Max # of elements      */
	int             noutput;                /* Number of outputs      */
	int             output_type[EFI_MAXARG];/* Type of outputs        */
	int             output_desc[EFI_MAXARG];/* Output array descriptor*/
	int             output_tag[EFI_MAXARG]; /* Tag for output         */
	int             output_to[EFI_MAXARG];  /* Destination of output  */
	int             output_rank[EFI_MAXARG][EFI_MAXDIM];/* Array dims */
	int             output_size[EFI_MAXARG];/* Max # of elements      */
} EVENT_EFI_CONFIG;

typedef struct {
	BYTE            data[MAX_DATASIZE];     /* maximum payload        */
} EVENT_EFI_MAXDATA;

typedef struct {
	EVENT_HEAD      head;                   /* DAFFIE header          */
	int             type;                   /* Control type           */
	int             param;                  /* Argument/parameter     */
	int             value;                  /* Value of above         */
	int             flag;                   /* Flag                   */
	int             tag;                    /* Tag for response       */
	int             clientid;               /* Client to send to      */
} EVENT_EFI_CTL;

#define ET_EFI_SPEC              1000
#define ET_EFI_CONFIG            1001
#define ET_EFI_STATUS            1002
#define ET_EFI_PARAMETER_FILE    1010
#define ET_EFI_START             1011
#define ET_EFI_ADVANCE           1012
#define ET_EFI_STOP              1013
#define ET_EFI_CTL               1014
#define ET_EFI_UNLOAD            1015
#define ET_EFI_PARAMETER_SET     1016
#define ET_EFI_WAVEDATA          1017
#define ET_EFI_SPIKEDATA         1018

#define ET_EFI_DATA_LO ET_EFI_WAVEDATA
#define ET_EFI_DATA_HI ET_EFI_SPIKEDATA

#define ET_EFI_MIN 1000
#define ET_EFI_MAX 1019

#define EFI_EVENTS \
	EVENT_EFI_SPEC           ev_efi_spec;           \
	EVENT_EFI_CONFIG         ev_efi_config;         \
	EVENT_EFI_PARAMETER_FILE ev_efi_parameter_file; \
	EVENT_EFI_START          ev_efi_start;          \
	EVENT_EFI_ADVANCE        ev_efi_advance;        \
	EVENT_EFI_STOP           ev_efi_stop;           \
	EVENT_EFI_UNLOAD         ev_efi_unload;         \
	EVENT_EFI_CTL            ev_efi_ctl;            \
	EVENT_EFI_MAXDATA        ev_efi_maxdata;        \
	EVENT_EFI_WAVEDATA       ev_efi_wavedata;       \
	EVENT_EFI_SPIKEDATA      ev_efi_spikedata;      \


#include "agentd.h"

#endif




