#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <diskio.h>
#include <ffconf.h>

#if 0
#define IO_TRACE
#endif

static FILE * fp = NULL;

void disk_fileimage_init(FILE * file)
{
  fp = file;
}

DSTATUS disk_initialize(BYTE drive)
{
#ifdef IO_TRACE
  printf("disk_initialize\n");
#endif

  if (drive)
    return STA_NOINIT;			/* Supports only single drive */

  if (fp == NULL)
  {
    return STA_NOINIT;
  }

	return 0;
}

DSTATUS disk_status(BYTE drive)
{
#ifdef IO_TRACE
  printf("disk_status\n");
#endif

	return 0;
}

DRESULT disk_read(BYTE drive, BYTE* buffer, DWORD sectorNumber, BYTE sectorCount)
{
  long new_ofs = sectorNumber * _MAX_SS;
  size_t s;

#ifdef IO_TRACE
  printf("disk_read(%d, %d) = %ld: ", (int)sectorNumber, sectorCount, new_ofs);
#endif

  // Goto the correct offset
  if (fseek(fp, new_ofs, SEEK_SET) != 0)
  {
#ifdef IO_TRACE
    printf("ERROR! (1)\n");
#endif
    return RES_ERROR;
  }

  // Read the requested number of sectors
  if (fread(buffer, _MAX_SS, sectorCount, fp) != sectorCount)
  {
#ifdef IO_TRACE
    printf("ERROR! (2)\n");
#endif
    return RES_ERROR;
  }

#ifdef IO_TRACE
  printf("OK\n");
#endif

	return RES_OK;
}

DRESULT disk_write(BYTE drive, const BYTE* buffer, DWORD sectorNumber, BYTE sectorCount)
{
  long new_ofs = sectorNumber * _MAX_SS;

#ifdef IO_TRACE
  printf("disk_write: ");
#endif

  // Goto the correct offset
  if (fseek(fp, new_ofs, SEEK_SET) != 0)
  {
#ifdef IO_TRACE
    printf("ERROR! (1)\n");
#endif
    return RES_ERROR;
  }

  // Write the requested number of sectors
  if (fwrite(buffer, _MAX_SS, sectorCount, fp) != sectorCount)
  {
#ifdef IO_TRACE
    printf("ERROR! (2)\n");
#endif
    return RES_ERROR;
  }

#ifdef IO_TRACE
  printf("OK\n");
#endif

	return RES_OK;
}

DRESULT disk_ioctl(BYTE drive, BYTE command, void* buffer)
{
  DRESULT rv = RES_ERROR;

  switch(command)
  {
    case (CTRL_SYNC):
    if (fsync(fileno(fp)) == 0)
    {
      rv = RES_OK;
    }
    break;

    case (GET_BLOCK_SIZE):
    {
      WORD * pW = (WORD *)buffer;
      *pW = 1;
      rv = RES_OK;
    }
    break;

    case (GET_SECTOR_COUNT):
    {
      struct stat stbuf;
      if (fstat(fileno(fp), &stbuf) == 0)
      {
        WORD nrSectors = stbuf.st_size / _MAX_SS;
        WORD * pW = (WORD *)buffer;
        *pW = nrSectors;

        rv = RES_OK;
      }
    }
    break;

    default:
    printf("disk_ioctl: unsupported command! (%d)\n", command);
    rv = RES_PARERR;
    break;
  }

#ifdef IO_TRACE
  printf("disk_ioctl(%d): %d\n", command, rv);
#endif

	return rv;
}


