# C-doku

![Preview](https://i.imgur.com/e51hLYw.gifv)

A command line sudoku game written in C. It can generate new games, solve games, validate games, and tell if there is a single solution to a game (thus validating new games). The game itself uses the [ncurses](https://www.gnu.org/software/ncurses/) library, and the tests rely on [cmocka](https://cmocka.org/). 

## Installation 
The included makefile should be able to compile the c files into a working binary, simply run `make`. To run the tests run `make test`. Once the binary is generated you can move it to somewhere in your path and then execute `c-doku`. Or run it from its directory with `./c-doku`. This has only been tested on Linux, adjustments may need to be made for Windows and OSX. 


## Key bindings
C-doku has vim-like keybindings. See the image or table below. 

![Keybindings](https://i.imgur.com/Fe0yt1t.png)

| Key                           | Action                                                             |
|:------------------------------|:-------------------------------------------------------------------|
|`h`                            | Move left |
|`j`                            | Move down |
|`k`                            | Move up |
|`l`                            | Move right |
|`^`                            | Beginning of row |
|`&`                            | End of row |
|`g`                            | Top of column |
|`G`                            | Bottom of column |
|`tab`                          | Next box |
|`return`                       | 1 box below |
|`G`                            | Bottom of column |
|`w`                            | Save |
|`q`                            | Quit |
|`n`                            | New Grid |
|`x`                            | Clear Grid |
|`s`                            | Solve |
|`v`                            | Validate |
|`u`                            | Undo |
|`r`                            | Redo |
|`d`                            | Delete |
|`c`                            | Complete / Incomplete |
|`m`                            | Multiple solutions |
|`space`                        | Clear message |