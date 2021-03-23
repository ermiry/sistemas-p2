#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE		1024

static const char *nuestros_nombres = { "Erick Salas Romero\nRicardo Whaibe Martinez\nMariana Peña Hernandez\nMaximiliano Escobar Valencia\nGarciacano Garcia Gabriel\nUriel Castañeda Gomez\n\n" };

static int checar_por_directorio_creado (const char *dirname) {

	struct stat dirstats = { 0 };
	return stat (dirname, &dirstats);

}

static void borrar_directorio (const char *dirname) {

	char buffer[BUFFER_SIZE] = { 0 };

	// borrar archivos dentro del directorio
	DIR *dp = opendir (dirname);
	if (dp) {
		struct dirent *ep = NULL;
		while ((ep = readdir (dp)) != NULL) {
			if (strcmp (ep->d_name, ".") && strcmp (ep->d_name, "..")) {
				(void) snprintf (buffer, BUFFER_SIZE - 1, "%s/%s", dirname, ep->d_name);
				(void) printf ("Borrando archivo %s...\n", buffer);

				(void) remove (buffer);
			}
		}

		(void) closedir (dp);
	}

	// borrar directorio
	if (rmdir (dirname)) {
		(void) printf ("Error %d al borrar el directorio %s!\n\n", errno, dirname);
	}

}

static void listar_directorio (const char *dirname) {

	// listar directorio actual
	DIR *dp = opendir (".");
	if (dp) {
		struct dirent *ep = NULL;
		while ((ep = readdir (dp)) != NULL) {
			// if (strcmp (ep->d_name, ".") && strcmp (ep->d_name, "..")) {
				(void) printf ("Archivo: %s\n", ep->d_name);

				listar_atributos_del_archivo (ep->d_name, false);
			// }
		}

		(void) closedir (dp);
	}

}

static void listar_atributos_del_archivo (const char *filename, bool all) {

    // atributos del archivo de nombres
    struct stat filestat = { 0 };
    (void) stat (filename, &filestat);
    if (all) {
        (void) printf ("Atributos del archivo %s: \n", filename);
        (void) printf ("ID of device containing file: %ld\n", filestat.st_dev);
        (void) printf ("Inode number: %ld\n", filestat.st_ino);
        (void) printf ("File type and mode: %d\n", filestat.st_mode);
        (void) printf ("Number of hard links: %ld\n", filestat.st_nlink);
        (void) printf ("User ID of owner: %d\n", filestat.st_uid);
        (void) printf ("Group ID of owner: %d\n", filestat.st_gid);
        (void) printf ("Device ID (if special file): %ld\n", filestat.st_rdev);
        (void) printf ("Total size, in bytes: %ld\n", filestat.st_size);
        (void) printf ("Block size for filesystem I/O: %ld\n", filestat.st_blksize);
        (void) printf ("Number of 512B blocks allocated: %ld\n\n", filestat.st_blocks);
    }

    else {
        (void) printf ("Atributos del archivo %s: \n", filename);
        (void) printf ("Inode number: %ld\n\n", filestat.st_ino);
    }

}

static unsigned int crear_archivo_nombres (const char *filename) {

    unsigned int retval = 1;

    FILE *nombres = fopen (filename, "w+");
    if (nombres) {
        (void) printf ("Se creo el archivo %s\n", filename);

        size_t wrote = fwrite (nuestros_nombres, strlen (nuestros_nombres), 1, nombres);
        if (wrote) {
            (void) printf ("Se escribireron %lu elementos en %s\n\n!", wrote, filename);

            retval = 0;
        }

        (void) fclose (nombres);
    }

    else {
        (void) printf ("Error %d al crear el archivo %s!\n\n", errno, filename);
    }

    return retval;

}

static void crear_symbolic_link (const char *filename) {

    // crear symbolic link
    const char *simbolico = { "simbolico" };

    (void) printf ("Creando link simbolico a %s...\n", filename);

    if (!symlink (filename, simbolico)) {
        (void) printf ("Link simbolico %s a %s creado!\n", simbolico, filename);
    }

    else {
        (void) printf ("Error %d al crear link simbolico!\n", errno);
    }

}
static void crear_directorio (const char *dirname, const char *filename) {

    char buffer[BUFFER_SIZE] = { 0 };

    if (!mkdir (dirname, 0777)) {
        (void) snprintf (buffer, BUFFER_SIZE - 1, "%s/%s", dirname, filename);
        if (!crear_archivo_nombres (buffer)) {
            listar_atributos_del_archivo (buffer, true);

            crear_symbolic_link (buffer);

            crear_hard_link (buffer);

            listar_directorio (dirname);
        }
    }

    else {
        (void) printf ("Error %d al crear el directorio %s!\n\n", errno, dirname);
    }

}

int main (int argc, const char **argv) {

    if (argc > 2) {
        const char *dirname = argv[1];
        const char *filename = argv[2];

        bool delete_dir = false;
        if (argc > 3) {
            if (!strcmp (argv[3], "--delete")) {
                delete_dir = true;
            }
        }

        if (!checar_por_directorio_creado (dirname)) {
            (void) printf ("El directorio %s ya existe\n", dirname);

            if (delete_dir) {
                (void) printf ("Borrando directorio...\n");
                borrar_directorio (dirname);
                (void) printf ("Borrado!\n\n");

                crear_directorio (dirname, filename);
            }
        }

        else {
            crear_directorio (dirname, filename);
        }
    }

    else {
        (void) printf ("Se requiere el nombre del directorio y el nombre del archivo!\n\n");
    }

    return 0;

}
