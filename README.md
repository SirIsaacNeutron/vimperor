# vimperor
A vim-inspired text editor using ncurses and C++

I was pretty interested to see how text editors like vim worked, so I
decided to try my hand at making my own editor. I'm using ncurses here
because it's much easier to use than escape sequences, and C++ because 
I like it better than C, and because I'd never written anything this big
in C++ before.

**Note**: I don't plan on working on vimperor anymore as I have grown bored of working on it.

## Installation

```
git clone https://github.com/SirIsaacNeutron/vimperor
cd vimperor
make vimperor
```

You will need [GNU Make](https://www.gnu.org/software/make/) and 
[g++](https://gcc.gnu.org/) installed in order for the above command to
work. Obviously you can edit the **makefile** to use a different C++
compiler if you wish. And of course, you will need ncurses installed.

### Installing ncurses

#### Windows

Unfortunately, I don't have access to a Windows machine, so **vimperor has not been tested on Windows.** This section may not be very helpful to you as a result.

Go to [http://invisible-island.net/ncurses/](http://invisible-island.net/ncurses/) and download the proper version of ncurses for your Windows machine (32-bit or 64-bit). Then, download [MinGW](http://mingw.org/).

#### macOS

As far as I'm aware, ncurses is already installed on all Macs, so Mac users are good to go if they've installed everything else vimperor requires.

#### Linux 

See [https://www.ostechnix.com/how-to-install-ncurses-library-in-linux/](https://www.ostechnix.com/how-to-install-ncurses-library-in-linux/) and follow the instructions for your Linux distribution.


## Usage

```
./vimperor [path_to_file_here]
```

Cursor movement work like vim's, but vimperor doesn't support adding numbers before motions. Typing "3j" to move the cursor down 3 lines won't work; you'll have to type "j" 3 times to do that.

Press the escape key to go to Normal mode, "i" to go to Insert mode, and "R" to go to Replace mode. In vimperor these modes work very much like vim's, although Normal mode is slightly different. While in Normal mode, press "s" to save a file, and "q" to exit vimperor.

Vimperor also supports the "x", "w", and "b" motions. They work similar to vim's implementation of these motions.
