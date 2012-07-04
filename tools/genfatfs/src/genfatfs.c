// fat filesystem generator for embedded systems
// Copyright (C) 2012 Nathan Huizinga <nathan.huizinga@gmail.com>
//
// Please direct support requests to raspberrypi-openwrt-dev@googlegroups.com
//
// 'genext2fs' portions taken from genext2fs.c in getext2fs:
//	Copyright (C) 2000 by Xavier Bestel <xavier.bestel@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version
// 2 of the License.
//
// Changes:
// 	 4 Jul 2012	Initial release
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <getopt.h>

#include <ff.h>

static char * app_name;
static char fileBuffer[64*1024];

extern void disk_fileimage_init(FILE * file);

DWORD get_fattime(void)
{
	return 0;
}

static void verror_msg(const char *s, va_list p)
{
	fflush(stdout);
	fprintf(stderr, "%s: ", app_name);
	vfprintf(stderr, s, p);
}

static void error_msg(const char *s, ...)
{
	va_list p;
	va_start(p, s);
	verror_msg(s, p);
	va_end(p);
	putc('\n', stderr);
}

static void error_msg_and_die(const char *s, ...)
{
	va_list p;
	va_start(p, s);
	verror_msg(s, p);
	va_end(p);
	putc('\n', stderr);
	exit(EXIT_FAILURE);
}

static void vperror_msg(const char *s, va_list p)
{
	int err = errno;
	if (s == 0)
		s = "";
	verror_msg(s, p);
	if (*s)
		s = ": ";
	fprintf(stderr, "%s%s\n", s, strerror(err));
}

static void perror_msg_and_die(const char *s, ...)
{
	va_list p;
	va_start(p, s);
	vperror_msg(s, p);
	va_end(p);
	exit(EXIT_FAILURE);
}

// Convert a numerical string to a float, and multiply the result by an
// IEC or SI multiplier if provided; supported multipliers are Ki, Mi, Gi, k, M
// and G.

float SI_atof(const char *nptr)
{
	float f = 0;
	float m = 1;
	char *suffixptr;

	f = strtof(nptr, &suffixptr);

	if (*suffixptr)
  {
		if (!strcmp(suffixptr, "Ki"))
			m = 1 << 10;
		else if (!strcmp(suffixptr, "Mi"))
			m = 1 << 20;
		else if (!strcmp(suffixptr, "Gi"))
			m = 1 << 30;
		else if (!strcmp(suffixptr, "k"))
			m = 1000;
		else if (!strcmp(suffixptr, "M"))
			m = 1000 * 1000;
		else if (!strcmp(suffixptr, "G"))
			m = 1000 * 1000 * 1000;
	}

	return f * m;
}

static FILE * xfopen(const char *path, const char *mode)
{
	FILE *fp;

	if ((fp = fopen(path, mode)) == NULL)
		perror_msg_and_die("%s", path);

	return fp;
}

static void printDirLevel(dirLevel)
{
  int i;

  for (i = 0; i < dirLevel; i++)
  {
    printf("+");
  }

  printf(" ");
}

// Add a single file to the image
static void add2fs_file(const char * filename)
{
  FILE * f_in;
  FIL f_out;
  FRESULT res;
  size_t fileSize;
  size_t readSize;
  struct stat stbuf;
  UINT bytesWritten;

  // Open input file
  f_in = xfopen(filename, "rb");

  if (fstat(fileno(f_in), &stbuf) != 0)
    perror_msg_and_die("Error reading file size of \"%s\"", filename);

  // Create output file
  if (f_open(&f_out, filename, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    perror_msg_and_die("Error creating \"%s\" in image", filename);

  // Copy contents
  fileSize = stbuf.st_size;

  while (fileSize > 0)
  {
    if (fileSize > sizeof(fileBuffer))
    {
      readSize = sizeof(fileBuffer);
    }
    else
    {
      readSize = fileSize;
    }

    if (fread(fileBuffer, readSize, 1, f_in) != 1)
      perror_msg_and_die("Error reading from file \"%s\"", filename);

    if (f_write(&f_out, fileBuffer, readSize, &bytesWritten) != FR_OK)
      perror_msg_and_die("Error writing to file \"%s\" in image", filename);

    if (bytesWritten != readSize)
      perror_msg_and_die("Error writing to file \"%s\" in image (full?!?)", filename);

    fileSize -= readSize;
  }

  // Close input/output files
  if (f_close(&f_out) != FR_OK)
    perror_msg_and_die("Error closing file \"%s\" in image", filename);

  fclose(f_in);
}

// Add a tree of entries to the image from the current directory
static void add2fs_from_dir(int verbose, int dirLevel)
{
  DIR *dh;
  struct dirent *dent;
  struct stat st;

	if ((dh = opendir(".")) == NULL)
		perror_msg_and_die(".");

	while ((dent = readdir(dh)) != NULL)
	{
		if ((!strcmp(dent->d_name, ".")) || (!strcmp(dent->d_name, "..")))
			continue;

		lstat(dent->d_name, &st);

    switch(st.st_mode & S_IFMT)
    {
      case S_IFLNK:
        // Handle symbolic links as normal files
      case S_IFREG:
        if (verbose)
        {
          printDirLevel(dirLevel);
          printf("Adding file: %s\n", dent->d_name);
        }

        add2fs_file(dent->d_name);
      break;

      case S_IFDIR:
        if (verbose)
        {
          printDirLevel(dirLevel);
          printf("Adding dir: %s\n", dent->d_name);
        }

        if (f_mkdir(dent->d_name) != FR_OK)
          perror_msg_and_die("Error creating directory \"%s\" in image", dent->d_name);

        // Change to directory one level deeper

        if (chdir(dent->d_name) < 0)
          perror_msg_and_die(dent->d_name);

        if (f_chdir(dent->d_name) != FR_OK)
          perror_msg_and_die("Error chdir in image to: %s", dent->d_name);

        // Add sub-directory recursive
        add2fs_from_dir(verbose, dirLevel + 1);

        // Change to directory one level up

        chdir("..");

        if (f_chdir("..") != FR_OK)
          perror_msg_and_die("Error chdir in image to: ..");
      break;

      default:
        error_msg("ignoring entry %s", dent->d_name);
        break;
    }
  }

	closedir(dh);
}

static void populate_fs(char **dopt, int didx, int verbose)
{
  int i;
  char * pdest;
  struct stat st;
  FILE * fh;
  int pdir;

  for (i = 0; i < didx; i++)
  {
    // If a [:path] is given, change to that directory in the image.
    if((pdest = strchr(dopt[i], ':')))
    {
      *(pdest++) = 0;
      if (f_chdir(pdest) != FR_OK)
        error_msg_and_die("path %s not found in file system", pdest);
    }

		stat(dopt[i], &st);

		switch(st.st_mode & S_IFMT)
		{
			case S_IFREG:
				error_msg_and_die("Adding from file %s is not supported", dopt[i]);
				break;

			case S_IFDIR:
				if ((pdir = open(".", O_RDONLY)) < 0)
					perror_msg_and_die(".");

				if (chdir(dopt[i]) < 0)
					perror_msg_and_die(dopt[i]);

        add2fs_from_dir(verbose, 1);

				if (fchdir(pdir) < 0)
					perror_msg_and_die("fchdir");

				if (close(pdir) < 0)
					perror_msg_and_die("close");
				break;

			default:
				error_msg_and_die("%s is neither a file nor a directory", dopt[i]);
		}
  }
}

// TODO: add version info in cmake file?
#define VERSION "0.2"
static void showversion(void)
{
	printf("genfatfs " VERSION "\n");
	printf("Using FatFs version R0.09 (http://elm-chan.org/fsw/ff/00index_e.html)\n");
}

static void showhelp(void)
{
	fprintf(stderr, "Usage: %s [options] image\n"
	"Create a fat file system image from directories/files\n\n"
	"  -d, --root <directory>\n"
	"  -b, --size-in-blocks <blocks>\n"
	"  -h, --help\n"
	"  -V, --version\n"
	"  -v, --verbose\n\n"
	"Report bugs to raspberrypi-openwrt-dev@googlegroups.com\n", app_name);
}

#define MAX_DOPT 128

int main(int argc, char ** argv)
{
	int nbblocks = -1;
	char * fsout = "-";
	char * dopt[MAX_DOPT];
	int didx = 0;
	int c;
	int verbose = 0;
  FATFS Fatfs;    /* File system object */
	FRESULT rc;			/* Result code */
  FILE * fp = NULL;

	struct option longopts[] = {
	  { "root",		required_argument,	NULL, 'd' },
	  { "size-in-blocks",	required_argument,	NULL, 'b' },
	  { "help",		no_argument,		NULL, 'h' },
	  { "version",		no_argument,		NULL, 'V' },
	  { "verbose",		no_argument,		NULL, 'v' },
	  { 0, 0, 0, 0}
	} ;

	app_name = argv[0];

	while ((c = getopt_long(argc, argv, "x:d:b:hVv", longopts, NULL)) != EOF)
  {
		switch(c)
		{
			case 'd':
				dopt[didx++] = optarg;
				break;

			case 'b':
				nbblocks = SI_atof(optarg);
				break;

			case 'h':
				showhelp();
				exit(0);
        break;

			case 'V':
				showversion();
				exit(0);
        break;

			case 'v':
				verbose = 1;
				showversion();
				break;

			default:
				error_msg_and_die("Note: options have changed, see --help or the man page.");
        break;
		}
	}

	if (optind < (argc - 1))
		error_msg_and_die("Too many arguments. Try --help.");

	if (optind > (argc - 1))
		error_msg_and_die("Not enough arguments. Try --help.");

	fsout = argv[optind];

  // No input file, create one
  int fileSize = nbblocks * 1024;

  if (nbblocks < 8)
    error_msg_and_die("Too few blocks, see --help.");

  // Create empty file
  fp = xfopen(fsout, "w+b");

  if (verbose)
  {
    printf("Creating: \"%s\" with size: %d bytes\n", fsout, fileSize);
  }

  // Make it the correct size
  if (ftruncate(fileno(fp), fileSize))
    perror_msg_and_die("Error creating new file: %s", fsout);

  disk_fileimage_init(fp);

  // Register volume work area (never fails)
  f_mount(0, &Fatfs);

  // Make file system in the newly created image

  if (verbose)
  {
    printf("Creating fat file system\n");
  }

  // Create file system at the beginning of the image
  if (f_mkfs(0, 1, 0) != FR_OK)
    perror_msg_and_die("Error making fat file system");

  if (verbose)
  {
    printf("Populating fat file system. Please wait...\n");
  }

  populate_fs(dopt, didx, verbose);

  if (verbose)
  {
    printf("Done\n");
  }

finalizer:
  // Unregister volume work area
	f_mount(0, NULL);

  // Close image file
  fclose(fp);

	return 0;
}
