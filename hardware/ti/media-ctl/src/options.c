/*
 * Media controller test application
 *
 * Copyright (C) 2010-2011 Ideas on board SPRL
 *
 * Contact: Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include "options.h"

#define MEDIA_DEVNAME_DEFAULT		"/dev/media0"

struct media_options media_opts = {
	.devname = MEDIA_DEVNAME_DEFAULT,
};

static void usage(const char *argv0, int verbose)
{
	printf("%s [options] device\n", argv0);
	printf("-d, --device dev	Media device name (default: %s)\n", MEDIA_DEVNAME_DEFAULT);
	printf("-e, --entity name	Print the device name associated with the given entity\n");
	printf("-V, --set-v4l2 v4l2	Comma-separated list of formats to setup\n");
	printf("    --get-v4l2 pad	Print the active format on a given pad\n");
	printf("-h, --help		Show verbose help and exit\n");
	printf("-i, --interactive	Modify links interactively\n");
	printf("-l, --links		Comma-separated list of links descriptors to setup\n");
	printf("-p, --print-topology	Print the device topology (implies -v)\n");
	printf("    --print-dot		Print the device topology as a dot graph (implies -v)\n");
	printf("-r, --reset		Reset all links to inactive\n");
	printf("-v, --verbose		Be verbose\n");

	if (!verbose)
		return;

	printf("\n");
	printf("Links and formats are defined as\n");
	printf("\tlink            = pad '->' pad '[' flags ']' ;\n");
	printf("\tpad             = entity ':' pad-number ;\n");
	printf("\tentity          = entity-number | ( '\"' entity-name '\"' ) ;\n");
	printf("\n");
	printf("\tv4l2            = pad '[' v4l2-properties ']' ;\n");
	printf("\tv4l2-properties = v4l2-property { ',' v4l2-property } ;\n");
	printf("\tv4l2-property   = v4l2-mbusfmt | v4l2-crop | v4l2-interval\n");
	printf("\t                | v4l2-compose | v4l2-interval ;\n");
	printf("\tv4l2-mbusfmt    = 'fmt:' fcc '/' size ;\n");
	printf("\tv4l2-crop       = 'crop:' rectangle ;\n");
	printf("\tv4l2-compose    = 'compose:' rectangle ;\n");
	printf("\tv4l2-interval   = '@' numerator '/' denominator ;\n");
	printf("\n");
	printf("\trectangle       = '(' left ',' top, ')' '/' size ;\n");
	printf("\tsize            = width 'x' height ;\n");
	printf("\n");
	printf("where the fields are\n");
	printf("\tentity-number   Entity numeric identifier\n");
	printf("\tentity-name     Entity name (string) \n");
	printf("\tpad-number      Pad numeric identifier\n");
	printf("\tflags           Link flags (0: inactive, 1: active)\n");
	printf("\tfcc             Format FourCC\n");
	printf("\twidth           Image width in pixels\n");
	printf("\theight          Image height in pixels\n");
	printf("\tnumerator       Frame interval numerator\n");
	printf("\tdenominator     Frame interval denominator\n");
}

#define OPT_PRINT_DOT		256
#define OPT_GET_FORMAT		257

static struct option opts[] = {
	{"device", 1, 0, 'd'},
	{"entity", 1, 0, 'e'},
	{"set-format", 1, 0, 'f'},
	{"set-v4l2", 1, 0, 'V'},
	{"get-format", 1, 0, OPT_GET_FORMAT},
	{"get-v4l2", 1, 0, OPT_GET_FORMAT},
	{"help", 0, 0, 'h'},
	{"interactive", 0, 0, 'i'},
	{"links", 1, 0, 'l'},
	{"print-dot", 0, 0, OPT_PRINT_DOT},
	{"print-topology", 0, 0, 'p'},
	{"reset", 0, 0, 'r'},
	{"verbose", 0, 0, 'v'},
};

int parse_cmdline(int argc, char **argv)
{
	int opt;

	if (argc == 1) {
		usage(argv[0], 0);
		return 1;
	}

	/* parse options */
	while ((opt = getopt_long(argc, argv, "d:e:f:hil:prvV:", opts, NULL)) != -1) {
		switch (opt) {
		case 'd':
			media_opts.devname = optarg;
			break;

		case 'e':
			media_opts.entity = optarg;
			break;

		/* 'f' is supported for backward compatibility reasons and will
		 * be removed later.
		 */
		case 'f':
			fprintf(stderr, "Warning: the -f option is deprecated "
				"and has been replaced by -V.\n");
		case 'V':
			media_opts.formats = optarg;
			break;

		case 'h':
			usage(argv[0], 1);
			exit(0);

		case 'i':
			media_opts.interactive = 1;
			break;

		case 'l':
			media_opts.links = optarg;
			break;

		case 'p':
			media_opts.print = 1;
			media_opts.verbose = 1;
			break;

		case 'r':
			media_opts.reset = 1;
			break;

		case 'v':
			media_opts.verbose = 1;
			break;

		case OPT_PRINT_DOT:
			media_opts.print_dot = 1;
			break;

		case OPT_GET_FORMAT:
			media_opts.pad = optarg;
			break;

		default:
			printf("Invalid option -%c\n", opt);
			printf("Run %s -h for help.\n", argv[0]);
			return 1;
		}
	}

	return 0;
}

