# OGXC FTP Client

Higher layer usage API for the low level lwftp library. Functionally, this is a lightweight FTP client.

Written as a modular / standalone library, depends only on LWFTP.

Suppported operations (some in progress):  
- Internet connectivity check (pings google.com for DNS resolution)
- Connection to an FTP server in passive mode
- LS/DIR directories (LIST command), and return a linked list of filenames / paths.
- Upload a file
- Download a file
- Close connection
