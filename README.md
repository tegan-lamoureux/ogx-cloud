# OGX-Cloud
A manager for cloud-based game libraries, for the Original Xbox.  

![current_status](https://raw.githubusercontent.com/tegan-lamoureux/ogx-cloud/refs/heads/main/graphics/current_status_screenshot.png "screenshot")  

## What is it?
A game manager that allows you to sync games from a remote / cloud server to your (modded) OG Xbox's hard drive, with Xbox-local data integrity verification using MD5 hashes from the server. 

It's mostly just a passion project of mine.

Modded Xbox?? In 2025? Yes. :)  
See: 
* https://github.com/Ryzee119/OpenXenium
* https://github.com/Team-Resurgent
* https://www.reddit.com/r/originalxbox
* https://www.ogxbox.com/
* xbox-scene on discord

## What is the current state?
Garbage lol. It connects to a remote FTP server and lists the files / directories. Renders a static background, and can draw text. Development is ongoing.

## What technologies does it use?
Built with [NXDK](https://github.com/XboxDev/nxdk), and a forked (and modified + extended) version of a lightweight FTP client built for LWIP. 

See: https://github.com/gezedo/lwftp and relevant copyright notices in the library headers.

## Building
1. Get a copy of [NXDK](https://github.com/XboxDev/nxdk), unpack next to this repo (or wherever you want, and modify `NXDK_DIR` in the Makefile)  
2. Source `nxdk/bin/activate` to prep the terminal environment  
3. `make`, and then copy over the iso or .xbe and run :)  

## TODO (In order of priority)
- [ ] Update / finalize changes to FTP client to handle needs (dir list, download large multi-chunk files)  
- [ ] Graphical UI  
- [ ] On screen keyboard  
- [ ] MD5 library  
- [ ] NXDK CI, build binary/iso on release tags  
