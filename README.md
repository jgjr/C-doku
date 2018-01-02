# C-doku

![Preview](https://i.imgur.com/6NUzs6Z.gif)

A command line sudoku game written in C. It can generate new games, solve games, validate games, and tell if there is a single solution to a game (thus validating new games). The game itself uses the [ncurses](https://www.gnu.org/software/ncurses/) library, and the tests rely on [cmocka](https://cmocka.org/). 

## Installation 
The included makefile should be able to compile the c files into a working binary, simply run `make`. To run the tests run `make test`. Once the binary is generated you can move it to somewhere in your path and then execute `c-doku`, or run it from its directory with `./c-doku`. This has only been tested on Linux, adjustments may be needed for it to work on Windows and OSX. 

## Usage
There are no mandatory arguments when running c-doku, however there is an optional argument for difficulty level. By defauly c-doku opens with a difficulty level of medium, but easy and hard can also be specified.

| Command                       | Pre-filled squares                                                 |
|:------------------------------|:-------------------------------------------------------------------|
| `c-doku easy`                 | 40                 |
| `c-doku medium`               | 35                 |
| `c-doku hard`                 | 30                 |

When saving, the game will be stored in the ~/.grid.cdoku file. When reopening the application, C-doku will try to find your saved grid in this file, and if there is no grid present, it will start with a new grid. 
The -h and --help arguments will display a short help message.

## Keybindings
C-doku has vim-like keybindings. See the image or table below. 

![Keybindings](https://i.imgur.com/qrV2Hm0.png)

| Key                           | Action                                                             |
|:------------------------------|:-------------------------------------------------------------------|
|`h / ←`                        | Move left |
|`j / ↓`                        | Move down |
|`k / ↑`                        | Move up |
|`l / →`                        | Move right |
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
|`e`                            | All squares editable |
|`o`                            | All filled squares locked |
|`space`                        | Clear message |
