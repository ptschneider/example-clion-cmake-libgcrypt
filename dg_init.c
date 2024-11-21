/* This program uses the same features as example 2, and uses options and
   arguments.

   We now use the first four fields in ARGP, so here’s a description of them:
     OPTIONS  – A pointer to a vector of struct argp_option (see below)
     PARSER   – A function to parse a single option, called by argp
     ARGS_DOC – A string describing how the non-option arguments should look
     DOC      – A descriptive string about this program; if it contains a
                 vertical tab character (\v), the part after it will be
                 printed *following* the options

   The function PARSER takes the following arguments:
     KEY  – An integer specifying which option this is (taken
             from the KEY field in each struct argp_option), or
             a special key specifying something else; the only
             special keys we use here are ARGP_KEY_ARG, meaning
             a non-option argument, and ARGP_KEY_END, meaning
             that all arguments have been parsed
     ARG  – For an option KEY, the string value of its
             argument, or NULL if it has none
     STATE– A pointer to a struct argp_state, containing
             various useful information about the parsing state; used here
             are the INPUT field, which reflects the INPUT argument to
             argp_parse, and the ARG_NUM field, which is the number of the
             current non-option argument being parsed
   It should return either 0, meaning success, ARGP_ERR_UNKNOWN, meaning the
   given KEY wasn’t recognized, or an errno value indicating some other
   error.

   Note that in this example, main uses a structure to communicate with the
   parse_opt function, a pointer to which it passes in the INPUT argument to
   argp_parse.  Of course, it’s also possible to use global variables
   instead, but this is somewhat more flexible.

   The OPTIONS field contains a pointer to a vector of struct argp_option’s;
   that structure has the following fields (if you assign your option
   structures using array initialization like this example, unspecified
   fields will be defaulted to 0, and need not be specified):
     NAME   – The name of this option’s long option (may be zero)
     KEY    – The KEY to pass to the PARSER function when parsing this option,
               *and* the name of this option’s short option, if it is a
               printable ascii character
     ARG    – The name of this option’s argument, if any
     FLAGS  – Flags describing this option; some of them are:
                 OPTION_ARG_OPTIONAL – The argument to this option is optional
                 OPTION_ALIAS        – This option is an alias for the
                                        previous option
                 OPTION_HIDDEN       – Don’t show this option in –help output
     DOC    – A documentation string for this option, shown in –help output

   An options vector should be terminated by an option with all fields zero. */

#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include "dg_pkcrypt.h"
#include "dg_get_configfile.h"
#include "dg_logger.h"

const char *argp_program_version =
  "argp-ex3 1.0";
const char *argp_program_bug_address =
  "<bug-gnu-utils@gnu.org>";

/* Program documentation. */
static char doc[] = "Argp example #3 -- a program with options and arguments using argp"
                   "\v"
                   "Two arguments are required regardless of whether any options are specifried.";

/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 ARG2";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",    'v', 0,      0,  "Produce verbose output" },
  {"quiet",      'q', 0,      0,  "Don't produce any output" },
  {"keygen_rsa", 'k', 0,      0,  "generate protected rsa keypair" },
  {"decrypt_rsa",'d', 0,      0,  "decrypt protected rsa keypair" },
  {"silent",     's', 0,      OPTION_ALIAS },
  {"config",     'c', "FILE", 0,"config file" },
  {"output",     'o', "FILE", 0,
   "Output to FILE instead of standard output" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char* args[2];                /* arg1 & arg2 */
  int silent, verbose,keygen_rsa, decrypt_rsa;
  char* output_file;
  char* config_file;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    case 'c':
      arguments->config_file = arg;
    break;
    case 'k':
      arguments->keygen_rsa = 1;
    break;
    case 'd':
      arguments->decrypt_rsa = 1;
    break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


int
dg_init(int argc, char** argv){
  struct arguments arguments;

  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.keygen_rsa = 0;
  arguments.decrypt_rsa = 0;
  arguments.output_file = "";
  arguments.config_file = "";

  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  log_init(DG_LOG_ALL, "/tmp/tst-dourgaur.log" );
  log_msg(DG_INFO, "startup CLI options & args processed");
  char logbuf[256];
  int result = 0;

  if(strlen(arguments.config_file) > 0) {
    rsa_samples_t entries;
    result = dg_get_config(arguments.config_file,&entries);
    if(result) {
      /* failed */
      //printf("Error reading config file %s\n", arguments.config_file);

      sprintf(logbuf, "error reading config file %s", arguments.config_file);
      log_msg(DG_ERR, logbuf );
    }
    else {
      /* succeeded */
      /*
      printf("Success reading config file %s\n", arguments.config_file);
      printf("rsa one  : %s\n", entries.one );
      printf("rsa two  : %s\n", entries.two );
      printf("rsa three: %s\n", entries.three );
      printf("rsa four : %s\n", entries.four );
      */
      sprintf(logbuf, "success reading config file %s", arguments.config_file);
      log_msg(DG_INFO, logbuf );
      sprintf(logbuf,"\nrsa 1: %s\nrsa 2: %s\nrsa 3: %s\nrsa 4: %s\n",entries.one,entries.two,entries.three,entries.four);
      log_msg(DG_INFO, logbuf );
    }
  }



  if(arguments.keygen_rsa) {
    result = dg_keygen_rsa(arguments.args[0]);
  }
  else if(arguments.decrypt_rsa) {
    result = dg_decrypt_rsa(arguments.args[0]);
  }
  else {
    printf ("\nARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
            "VERBOSE = %s\nSILENT = %s\n",
            arguments.args[0], arguments.args[1],
            arguments.output_file,
            arguments.verbose ? "yes" : "no",
            arguments.silent ? "yes" : "no");
  }

  return result;
}

