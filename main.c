#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	int fd = 2;

	// Update fd using inline assembly
	asm volatile (
		"mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd)
	);

	if (ac == 2)
	{
		// Open file in read-only mode
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			// Handle file open errors
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return EXIT_FAILURE;
		}
		info->readfd = fd;
	}

	// Populate environment list
	populate_env_list(info);

	// Read command history
	read_history(info);

	// Execute shell commands
	hsh(info, av);

	return EXIT_SUCCESS;
}
