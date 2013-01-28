#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#define BLOCKSIZE 1

int main(int argc, char **argv)
{
    char *input = argv[1];
	char *output = argv[2];

	int in_fd;
	int out_fd;

	char buf[BLOCKSIZE];

	int num_read;
	int num_written;

	char *theFilename = input;
	char *fileModTimestamp;
	char *fileOwnerID;
	char *fileGroupID;
	char *fileMode;
	int  *fileSize;
	char *fileMagic;

    struct stat sb;

   if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   if (stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

   printf("File type:                ");

   switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

   printf("I-node number:            %ld\n", (long) sb.st_ino);

   printf("Mode:                     %lo (octal)\n",
            (unsigned long) sb.st_mode);

   printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
            (long) sb.st_uid, (long) sb.st_gid);

   printf("Preferred I/O block size: %ld bytes\n",
            (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",
            (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) sb.st_blocks);

   printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

   exit(EXIT_SUCCESS);




	off_t file_size;
	off_t copied;

	in_fd = open(input, O_RDONLY);
	if(in_fd == -1){
		perror("Can't open input file");
		exit(-1);
	}

	out_fd = open(output, O_WRONLY | O_CREAT, 0777);
	if(out_fd == -1){
		perror("Can't open output file");
		exit(-1);
	}

	file_size = lseek(in_fd, 0, SEEK_END);
	copied = 0;
	lseek(in_fd, -1, SEEK_END);

	while(copied < file_size){
		num_read = read(in_fd, buf, BLOCKSIZE);
		num_written = write(out_fd, buf, BLOCKSIZE);

		if (num_read != num_written){
			perror("Error writing file");
			unlink(output);
			exit(-1);
		}

		copied += num_written;

		lseek(in_fd, -2, SEEK_CUR);
	}

	return 0;
}
