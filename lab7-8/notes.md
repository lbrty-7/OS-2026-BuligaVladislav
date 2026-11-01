# Lab 8 – notes (answers to the Observe boxes)

Buliga Vladislav, FAF-243. The full report with screenshots is in `report.md` / `Lab7-8_BuligaVladislav_FAF-243.docx`.

## run, Inside xv6

1. In xv6 run.c uses fork(), exec("echo", args) and wait(&status). On Linux in Lab 7 run.c used fork(), execlp("ls", "ls", "-l", NULL) and waitpid(pid, &status, 0). What is the same is the whole idea and the order of the three steps: fork makes a copy of the process, exec replaces the program in the child and does not return if it succeeds, and wait makes the parent sleep until the child ends and gives its exit status. Only the details differ: exec in xv6 takes an array of arguments (like execv) and no PATH search is done, while execlp takes a list and searches PATH; and on Linux the status has to be decoded with WEXITSTATUS.

2. "hello from the child" is printed by echo, not by the code of run. The child of run only builds the argument array and calls exec; after a successful exec the process is running the echo program, which prints its arguments. run itself prints only the lines that start with "parent:" and "child:".

3. The status is simply the number that the child passes to exit. In the xv6 run.c the line after exec is exit(1), so the parent sees 1, while in the Linux run.c it is exit(127), so the parent sees 127. 127 is only a convention (the one bash uses for "command not found"); the kernel just passes on whatever value the child gives.

## pingpong

1. A pipe works only in one direction and does not know who is reading. With a single pipe, after the parent writes the ping byte it would immediately call read on the same pipe and could read back its own byte before the child does, so the child would wait forever. Also, neither side could tell whether a byte is a ping or a pong. With two pipes there is one channel for each direction: the parent only writes into p2c and only reads from c2p, and the child does the opposite.

2. The call read(c2p[0], &b, 1) in the parent. read on an empty pipe blocks, so the parent sleeps there until the child writes the byte back. In the same way, the read(p2c[0], &b, 1) in the child makes the child wait for the ping. This is also why "received ping" is always printed before "received pong".
