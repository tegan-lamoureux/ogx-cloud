# Tools

## server_format.py

A formatting script for your server library. This tool makes the required metadata tags on your server for OGXC to operate. It will not modify any file contents or folder structures, only create new metadata tags (.ogxc files)

NOTE: This only works with linux or bsd based file paths at the moment. Windows based servers will likely fail.

### Expected File Structure

Each game should be in its own folder, in a single library folder on the server. Ex:  
* `192.168.0.12:~/xbox_library/KOTOR/...`  
* `192.168.0.12:~/xbox_library/NFSU2/...`  
* `192.168.0.12:~/xbox_library/Fable/...`  

The server can be on your local network, or on the internet somewhere. You just need FTP access to it. 

It does not matter the format of the game. In my case, I use [Team Resurgent's Repackinator tool](https://github.com/Team-Resurgent/Repackinator) to make CCIs and a corresponding .xbe launcher, but you can have ISOs, zips, raw game files, homebrew, apps, etc. The library manager is file-agnostic, it only needs the required metadata files to operate. That's what this tool creates.

### Running 

When running the script, it will check every folder in the current working directory and make a single metadata tag file in that directory. In the above server example, you would copy the script to  
`192.168.0.12:~/xbox_library/server_formatter.py`  
and run it from there. (Or just run it from wherever it is, while in the library folder.)

It would then create the following metadata tags:  
* `xbox_library/library.ogxc`  
* `xbox_library/KOTOR/game.ogxc`  
* `xbox_library/NFSU2/game.ogxc`  
* `xbox_library/Fable/game.ogxc`  

### Limitations

* The script will not operate on recursive directories. Only the folders found in the current running directory. 

* The script will not run from a different location. It must be copied to the top level library folder.

* The script will not format your folders or library for you, you must handle the folder organization yourself. It is well suited to the output directory for Team Resurgent's Repackinator tool, and can be ran from there with no other modifications. 

This is just because I am lazy, feel free to make a PR to expand it :)

### Metadata Format

There are two ascii text files. In the first one,  with the first line being the folder name (game name), and the rest being a line-by-line list of files and their corresponding hashes. There will be a second file, containing only the hash of the completed config file. 

game.ogxc:  
````
game(folder)_name
file1 md5hash
file2 md5hash
file3 md5hash
````

game.ogxc.md5:  
````
game.ogxc md5hash
````

### Sample output (Current state of tool)
````
$ python ../os_projects/ogxcloud/ogx-cloud/tools/server_format.py

I will build metadata tags in the following library directory, is this what you want?
/home/tegan/test
[y/N] y

Processing: THE ELDER SCROLLS III - M.G.O.T.Y (USA)
        MD5: f461fa010e23cfd92e0359e36e4c5a2a   File: default.tbn
        MD5: c880b7e200986583b71ec6a2784a10fb   File: default.xbe
        MD5: 8588c943893e5e66e25cceeaf78e5fe1   File: THE ELDER SCROLLS III - M.G.O (USA).cci
Processing: JSRF - JET SET RADIO FUTURE (USA)
        MD5: f98135990501d1b18da102cb0c286416   File: JSRF - JET SET RADIO FUTURE (USA).cci
        MD5: 9e9ea8a46808846756cb52665320152b   File: default.tbn
        MD5: 9b81d60cd3bc8a27b0920ed1689a3cc2   File: default.xbe
````