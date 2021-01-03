# Build :hammer_and_wrench:

The `/Makefile` (located at the root level for building simplicity) only redirect `make` actions to `/src/Makefile` which defines the building rules to generate the application's executable

If you are adding a new file to the project, that need to be compiled (e.g. file containing functions), and that isn't directly in `/src` nor `/src/screens`, then you might want to add your new location to the `/src/Makefile` with this template line :

```
YOUR_OBJECTS = $(patsubst %.c, %.o, $(wildcard your/objects/path/*.c))
```

If you're adding `*.c` files directly into `/src` or `/src/screens`, it'll be automatically detected by the make process, it's all taken care of.

Default building and debugging (*at the project's root*)
```
$ make
# Original mode
$ ./PurpleDrank

# Debugging mode (add hitboxes, coordinates, player's properties and console verbose)
$ ./PurpleDrank debug
OR in game toggling by pressing <F3>

```

# Structure :heavy_check_mark:
*The structure tries to follow git/dev best practices*

* `/lib`
    * Contains external libraries (e.g. raylib)
* `/src`
    * Contains our source code and header file (*.c and *.h)
* `/src/lib`
    * Contains libraries used for the game and the game's `#define`
* `/test`
    * Contains ALL the tests, keep them separate THERE
* `/scripts`
    * Automatization scripts, for custom operations (raylib installation, environment setup)
* `/res`
    * Contains all the static ressources, as images and sounds

# Keys files/directories

`main.c`
* Is the main file, where the game start and switch map/screen

`globals.h`
* Contains all the variables, struct, class, enum declarations
* **NO VALUE SHOULD BE DEFINED HERE**

`gameplay.c`
* Contains all global game and player control related functions

`lib/`
* Contains the libraries we need in order to compile our code

`lib/defines.c`
* Contains the game's constants, values that will never change (e.g. key identifiers, size, config, path)
* **ALL** `#define` should be declared here (except for exceptions)

`src/screens`
* Contains the game's screens files

# Create/Edit

If you create new levels or screens (`/src/screens`), make sure to include at least the following includes, at the top :
```
#include "../globals.h"
#include "../gameplay.h"
#include "../lib/defines.c"
```

Please try to keep your code clean and understanable, with correct identations (4 spaces), meaningfull comments and matching naming convention

Please try to keep consistency in the file structure, and not create test files within the main branch, or do it in the `/test` directory

# Rules

This version is a **CLEAN, WORKING and STABLE** version. If you wish to provide any improvements and modifications, respect the following :

* Please create a **new branch**, from the current one
	* `git checkout -b [YOUR_BRANCH] [BASE_BRANCH]`
		* e.g. `git checkout -b feat/foo master`
		* `git branch` to see which branch you're working on`

* Develop your feature, make your improvements

* Make a `make purge`, at the project root directory

* Type `make` and check that there's no errors, and that the executable is working as expected

* If everything's OK, you can commit your changes and request to merge your work to the main branch (via the git's Web UI)
	
	* `git commit -am "[COMMIT_MESSAGE]"` (`-a` will add all untracked modified files)
		* e.g. `git commit -am "feat: added physics to level1"`

	* `git push` will push your local changes to the remote project repo (for others to pull it)

* Once your merge request have been approved and changes have been merged to the main branch, if you do not plan to work further on, you can delete your branch (local/remote)
	* `git branch -d [YOUR_BRANCH] && git push origin --delete [YOUR_BRANCH]`
		* e.g. `git branch -d feat/bar && git push origin --delete feat/bar`
