/**@file main.c
 * @brief dbcc - produce serialization and deserialization code for CAN DBC files
 * @copyright Richard James Howe
 * @license MIT */
#include <assert.h>
#include <stdint.h>
#include "mpc.h"
#include "util.h"
#include "can.h"
#include "parse.h"
#include "2c.h"
#include "options.h"

typedef enum {
	CONVERT_TO_C
} conversion_type_e;


static char *replace_file_type(const char *file, const char *suffix)
{
	assert(file);
	assert(suffix);
	char *name = duplicate(file);
	char *dot = strrchr(name, '.');
	if(*dot)
		*dot = '\0';
	size_t name_size = strlen(name) + strlen(suffix) + 2;
	name = reallocator(name, name_size); /* + 1 for '.', + 1 for '\0' */
	strcat(name, ".");
	strcat(name, suffix);
	return name;
}

static int dbc2cWrapper(dbc_t *dbc, const char *dbc_file, const char *file_only, dbc2c_options_t *copts)
{
	assert(dbc);
	assert(dbc_file);
	assert(file_only);
	// char *cname = replace_file_type(dbc_file,  "c");
	// printf("the dbc file is %s\n", dbc_file);
	// printf("the read only file is %s\n", file_only);
	char *hname = replace_file_type(dbc_file,  "h");
	char *fname = replace_file_type(file_only, "h");
	// FILE *c = fopen_or_die(cname, "wb");
	FILE *h = fopen_or_die(hname, "wb");
	const int r = dbc2c(dbc, h, fname, copts);
	// fclose(c);
	fclose(h);
	// free(cname);
	free(hname);
	free(fname);
	return r;
}

int main(int argc, char **argv)
{
	// log_level_e log_level = get_log_level();
	conversion_type_e convert = CONVERT_TO_C;
	const char *outdir = NULL;
	dbc2c_options_t copts = {
		.use_time_stamps           =  false,
		.use_doubles_for_encoding  =  false,
		.generate_print            =  false,
		.generate_pack             =  false,
		.generate_unpack           =  false,
		.generate_asserts          =  true,
	};


	for(int i = dbcc_optind; i < argc; i++) {
		debug("reading => %s", argv[i]);
		mpc_ast_t *ast = parse_dbc_file_by_name(argv[i]);
		if(!ast) {
			warning("could not parse file '%s'", argv[i]);
			continue;
		}
		if(verbose(LOG_DEBUG))
			mpc_ast_print(ast);

		dbc_t *dbc = ast2dbc(ast);

		char *outpath = dbcc_basename(argv[i]);
		if(outdir) {
			outpath = allocate(strlen(outpath) + strlen(outdir) + 2 /* '/' + '\0'*/);
			strcat(outpath, outdir);
			strcat(outpath, "/");
			strcat(outpath, dbcc_basename(argv[i]));
		}

		int r = 0;
		r = dbc2cWrapper(dbc, outpath, dbcc_basename(argv[i]), &copts);
		if(r < 0)
			warning("conversion process failed: %u/%u", r, convert);

		if(outdir)
			free(outpath);
		dbc_delete(dbc);
		mpc_ast_delete(ast);
	}

	return 0;
}

