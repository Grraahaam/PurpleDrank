# Development structure

`main.c`

* Is the main file, where the game start and switch map/screen

`globals.h`

* Contains all the variables, struct, class, enum declarations
* **NO VALUE SHOULD BE DEFINED HERE**

`lib/`

* Contains the libraries we need in order to compile our code

`lib/defines.c`

* Contains the game's constants, values that will never change (e.g. key identifiers, size, config, path)

# Create/Edit

If you create new levels or screens, make sure to include at least the following two includes, at the top :
* `#include "../globals.h"`
* `#include "../lib/defines.c"`

Please try to keep your code clean and understanable, with correct identations and comments

Please try to keep consistency in the file structure, and not create test files within the main branch, or do it in the `/test` directory

# Rules

This version is a **CLEAN, WORKING and STABLE** version. If you wish to provide any improvements and modifications, respect the following :

* Please create a **new branch**, from the current one
	* `git checkout -b [YOUR_BRANCH]`
		* e.g. `git checkout -b your_name`
		* `git branch` to see which branch you're working on`

* Develop your feature, make your improvements

* Make a `make clean && rm solin`, at the project root directory

* Type `make` and check that there's no errors, and that ./solin is working as expected

* If everything's OK, you can commit your changes and merge your work to the main branch
	
	* `git commit -a -m "[COMMIT_MESSAGE]"` (`-a` will add all untracked modified files)
		* e.g. `git commit -a -m "Feature: added physics to level1"`
	
	* `git checkout master` (will take you back to the main branch)
	
	* `git merge [YOUR_BRANCH]` (will merge your branch to the current (main) one)
		* `git merge your_name`

	* `git push` to push the project back online (for others to pull it)

* Then if you're happy with your changes, you can delete your branch (because all changes are on the main now)
	* `git branch -d [YOUR_BRANCH]`
		* e.g. `git branch -d your_name`
