# vimperor
A vim clone using ncurses and C++

I was pretty interested to see how text editors like vim worked, so I
decided to try my hand at making my own editor. I'm using ncurses here
because it's much easier to use than escape sequences, and C++ because 
I like it better than C, and because I'd never written anything this big
in C++ before.

## Installation

```
git clone https://github.com/SirIsaacNeutron/vimperor
cd vimperor
make vimperor
```

You will need [GNU Make](https://www.gnu.org/software/make/) and 
[g++](https://gcc.gnu.org/) installed in order for the above command to
work. Obviously you can edit the **makefile** to use a different C++
compiler if you wish.

## Usage

```
./vimperor [path_to_file_here]
```
