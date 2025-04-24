# OGX-Cloud
A manager for cloud-based game libraries, for the Original Xbox. 

Built using the [NXDK](https://github.com/XboxDev/nxdk) toolchain, it uses no proprietary code or Microsoft dependencies, and is entirely open source and legal to use.  

![current_status](https://raw.githubusercontent.com/tegan-lamoureux/ogx-cloud/refs/heads/main/graphics/current_status_screenshot.png "screenshot")  

## What is it?
A game library manager that allows you to sync games from your remote server (ex: seedbox, local NAS, AWS storage) to your modded original Xbox's hard drive, with Xbox-local data integrity verification using MD5 hashes from the server.  

It also includes a server setup / metadata creation script (`tools/server_format`) to generate the required hashes and tags your server library needs for compatibility.

The server must have FTP access enabled, and a formatted library (each game in its own folder, with a ogxc config file - created with the above tool). The config file just contains a metadata tag for the library, a list of files in the game directory, and their corresponding MD5 hash. See the `tools/` directory for more information.

Modded Xbox?? In 2025? Yes. :)  
See: 
* https://github.com/Ryzee119/OpenXenium
* https://github.com/Team-Resurgent
* https://www.reddit.com/r/originalxbox
* https://www.ogxbox.com/
* xbox-scene on discord

## What is the current state?
Garbage lol. It connects to a remote FTP server and lists the files / directories. Renders a static background, and can draw text. Development is ongoing.

Edit - Getting there! Server side setup and metadata creation is working, UI has a functional on screen keyboard. :)

## What technologies does it use?
[NXDK](https://github.com/XboxDev/nxdk), and a forked (and modified + extended) version of a lightweight FTP client built for LWIP. 

See: https://github.com/gezedo/lwftp and relevant copyright notices in the library headers.

### Modularity
Most of the code was written to be as modular as possible. The FTP library, TTF Font rendering, and On Screen Keyboard are all able to be directly used in any other project.

See the `ogx-cloud/lib` folder. :)

## Building
1. Get a copy of [NXDK](https://github.com/XboxDev/nxdk), unpack next to this repo (or wherever you want, and modify `NXDK_DIR` in the Makefile)  
2. Source `nxdk/bin/activate` to prep the terminal environment  
3. `make`, and then copy over the iso or .xbe and run!!

My typical workflow includes the `format` make target, which lints with clang-format, and launches the iso locally for [emulation via xemu](https://xemu.app/). I push to my xbox more rarely, for larger feature checks. 

Example:  
`make format && make && xemu -dvd_path ./ogxcloud.iso`

## TODO (In order of priority)
- [ ] Update / finalize changes to FTP client to handle needs (dir list, download large multi-chunk files)  
- [X] Graphical UI  
- [ ] On screen keyboard  
- [X] MD5 library (server metadata tool) 
- [ ] NXDK CI, build binary/iso on release tags  
