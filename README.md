# TRash
T Radheshyam's spinoff of shell! A POSIX based shell made by T Radheshyam (you see the TRash?) (Part of Assignment for Operating Systems and Networking Course)

The following are the C files used: 

**The Code has been divided into the following files:**

-   [`history.c`](./src/history.c)
-   [`main.c`](./src/main.c)
-   [`command_fgbg.c`](./src/command_fgbg.c)
-   [`command_ls.c`](./src/command_ls.c)
-   [`directoryMovements.c`](./src/directoryMovements.c)
-   [`history.c`](./src/history.c)
-   [`terminalDisp.c`](./src/terminalDisp.c)

All the header files are contained in the folder [`Headerfiles`](./src/HeaderFiles)

Running:
run `make` to create the executable file call `makeTRash`.
Then execute `makeTRash`

Functionalities Implemented:
- Builtin commands: `echo`, `pwd`, `cd`, `ls` with support for flags like `-l` and '`-a`
- Foreground and Background Processes. 
    - Foreground Process: your shell will wait for this process to complete and regain control when this process exits.
    - Background Process: Any command invoked with "&" is treated as a background command. This implies that your shell will spawn that process and doesn't wait for the process to exit.
- `pinfo`
- `history`
- Input Output Redirection
- Signal Handling