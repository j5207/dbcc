#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "mpc.h"

typedef enum {
	endianess_motorola_e = 0,
	endianess_intel_e = 1,
} endianess_e;

typedef struct {
	char *name;          /**< name of the signal */
	unsigned bit_length; /**< bit length in message buffer */
	unsigned start_bit;  /**< starting bit position in message */
	endianess_e endianess; /**< endianess of message */
	bool is_signed;      /**< if true, value is signed */
	double scaling;      /**< scaling */
	double offset;       /**< offset */
	size_t ecu_count;    /**< ECU count */
	char **ecus;         /**< ECUs sending/receiving */
	union { /**< not used, but would contain the data */
		int64_t integer;
		uint64_t uinteger;
		double floating;
	} data;
} signal_t;

typedef struct {
	char *name;    /**< can message name */
	char *ecu;     /**< name of ECU @todo check this makes sense */
	unsigned dlc;  /**< length of CAN message 0-8 bytes */
	unsigned id;   /**< identifier, 11 or 29 bit */
	uint64_t data; /**< data, up to eight bytes, not used for generation */
	size_t signal_count; /**< number of signals */
	signal_t **signals; /**< signals that can decode/encode this message*/
} can_msg_t;

typedef struct {
	int message_count;
	can_msg_t **messages;
} dbc_t;

signal_t *signal_new(void);
void signal_delete(signal_t *signal);
can_msg_t *can_msg_new(void);
void can_msg_delete(can_msg_t *msg);
signal_t *ast2signal(mpc_ast_t *ast);
can_msg_t *ast2msg(mpc_ast_t *ast);
dbc_t *ast2dbc(mpc_ast_t *ast);
void dbc_delete(dbc_t *dbc);

#endif
