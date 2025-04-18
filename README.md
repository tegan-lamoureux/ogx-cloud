# OGX-Cloud
A manager for cloud-based game libraries, for the Original Xbox. 

### What is it?
A manager that allows you to sync games from a remote / cloud server to your OG Xbox, with Xbox-local data integrity verification using MD5 hashes from the server. 

It's mostly just a passion project of mine.

### What is the current state?
Garbage lol. It connects to a remote FTP server and lists the files / directories. Development is ongoing.

### What does it use?
Built with [NXDK](https://github.com/XboxDev/nxdk), and a forked (and modified + extended) version of a lightweight FTP client built for LWIP. 

See: https://github.com/gezedo/lwftp and relevant copyright notices in the library headers.

### TODO (In order of priority)
- [ ] Update / finalize changes to FTP client to handle needs (dir list, download large multi-chunk files)  
- [ ] Graphical UI  
- [ ] On screen keyboard
- [ ] MD5 library