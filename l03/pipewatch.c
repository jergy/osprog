#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

ssize_t readBytes(int infd)
{
	char buf[2048];
    ssize_t bytesRead;
    ssize_t totalBytesRead = 0;
    char *ret;

    time_t curtime;
    
    

    for (;;)
    {
    	bytesRead = read(infd, buf, sizeof(buf));

    	if ( bytesRead != -1 )
    	{
            totalBytesRead += bytesRead;
    	}
        if ( bytesRead == 0 )
        {
            break;
        }
        else
        {
            ret = strstr(buf, "quit");
            if (ret != NULL)
            {
                if (strcmp(ret, "quit\n") == 0)
                {
                    // printf("Compare passed\n");
                    // printf("The substring is: %s\n", ret);
                    exit(0);
                }
            }
            else
            {
                ret = strstr(buf, "time");
                if (ret != NULL)
                {
                    if (strcmp(ret, "time\n") == 0)
                    {
                        time(&curtime);
                        printf("%s", ctime(&curtime));
                        exit(0);
                    }
                }
            }
        }
	}
	close(infd);
		
    return totalBytesRead;
}

int main(int argc, char* argv[])
{
	for (;;)
	{
		fd_set fds;
		int infds[argc-1];
		int c = 0;
    	ssize_t totalBytesRead = 0;

		struct stat st;

		for (int i = 1; i < argc; i++)
		{
			if (stat(argv[i], &st) != -1)
			{
				if (!S_ISFIFO(st.st_mode))
				{
					fprintf(stderr, "%s\n", "error");
					exit(-1);
				}
			}

			mkfifo (argv[i], 0666);

			if ((infds[c] = open(argv[i], O_RDONLY | O_NONBLOCK)) == -1)
			{
				if (errno == EACCES) 
		    	{
					fprintf(stderr, "You don't have permission to access %s", argv[1]);
				}
				else if (errno == EAGAIN) 
				{
					fprintf(stderr, "There is no data available right now %s", argv[1]);
				}
				else if (errno == EINTR) 
				{
					fprintf(stderr, "This call did not succeed because it was interrupted %s", argv[1]);
				}
				exit(EXIT_FAILURE);
			}
			c += 1;
		}

		c = 0;

		FD_ZERO(&fds); 
		int numberForSelect = 0;
		for (int i = 1; i < argc; i++)
		{
			FD_SET(infds[c], &fds);
			numberForSelect += infds[c];
			c += 1;
		}
		c = 0;

		select(numberForSelect + 1, &fds, NULL, NULL, NULL);

		for (int i = 1; i < argc; i++)
		{
			if (FD_ISSET(infds[c], &fds))
			{
				totalBytesRead = readBytes(infds[c]);
				printf("%s: ", argv[i]);
				printf("%zd bytes\n", totalBytesRead);
			}	
			c += 1;
		}
	}
	
	return EXIT_SUCCESS;
}
