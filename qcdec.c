#include <stdarg.h>
#include <stdio.h>

#include "qcc.h"

void AddSourceFile(const char *parentpath, const char *filename)
{
}

void compilecb(void)
{
}

void EditFile(const char *name, int line, pbool setcontrol)
{
}

void GUI_DialogPrint(const char *title, const char *text)
{
}

int GUIprintf(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
}

void *GUIReadFile(const char *fname, unsigned char *(*buf_get)(void *ctx, size_t len), void *buf_ctx, size_t *out_size, pbool issourcefile)
{
}

static void *QCDEC_ReadFile(const char *filename, long *filesize)
{
	FILE *file = fopen(filename, "rb");
	char *buffer = NULL;

	if (file)
	{
		fseek(file, 0, SEEK_END);
		*filesize = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (*filesize == -1)
			printf("ERROR: Could not get size of file \"%s\"\n", filename);
		else
		{
			buffer = malloc(*filesize);

			if (buffer)
			{
				if (fread(buffer, 1, *filesize, file) != *filesize)
				{
					printf("ERROR: Could not read %li bytes from file \"%s\"\n", *filesize, filename);
					free(buffer);
					buffer = NULL;
				}
			}
			else
				printf("ERROR: Could not allocate %li bytes\n", *filesize);
		}

		fclose(file);
	}
	else
		printf("ERROR: Could not open file \"%s\" for reading\n", filename);

	return buffer;
}

static pbool QCDEC_WriteFile(const char *filename, void *filedata, size_t filesize)
{
	FILE *file = fopen(filename, "wb");
	pbool result = false;

	if (file)
	{
		result = fwrite(filedata, 1, filesize, file) == filesize;

		if (!result)
			printf("ERROR: Could not write %li bytes to file \"%s\"\n", filesize, filename);

		fclose(file);
	}
	else
		printf("ERROR: Could not open file \"%s\" for writing\n", filename);

	return result;
}

static int Decompile(char *filename)
{
	long size;
	void *progs = QCDEC_ReadFile(filename, &size);

	if (!progs)
		return 1;

	void DecompileProgsDat(char *name, void *buf, size_t bufsize);
	DecompileProgsDat(filename, progs, size);

	extern vfile_t *qcc_vfiles;

	while(qcc_vfiles)
	{
		vfile_t *vf = qcc_vfiles;
		qcc_vfiles = vf->next;

		QCDEC_WriteFile(vf->filename, vf->file, vf->size);

		free(vf->file);
		free(vf);
	}

	free(progs);

	return 0;
}

int main (int argc, char **argv)
{
	if (argc == 2)
		return Decompile(argv[1]);

	printf("Usage: %s progs.dat\n", argv[0]);
	return 1;
}
