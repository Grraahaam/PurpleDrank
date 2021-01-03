
# Achievement rate

The 1.0 version is achieved at around **80%**!

```
total tasks: ~ 47
remaining tasks: ~ 9
```

# TODO

- [ ] **keyboard binding**
    - [x] bind a key to return to the menu
    - [ ] create a custom key binding menu
- [x] **positioning & dimensioning**
    - [x] create a function returning the pixel value of each percent of width/height (positioning purposes)
    - [x] convert fixed positions to responsive ones where possible
- [ ] **sounds**
    - [ ] implement game music
    - [x] implement sound effect on actions (jump, super, portal, hit, damaged)
- [x] **sprites**
	- [x] clean up "gob walkking"
	- [x] group menu screens into a single spritecheet
- [ ] **screens**
	- [x] remove text on screen images
	- [x] set corresponding text and instructions (with effect) on non-game screens
	- [x] harmonize screens with the game artistic direction?
	- [x] implement player selection screen
	- [ ] implement controls explanantion screen
	- [x] set up automatic image ratio scaling to fit the whole window
	- [ ] handle fullscreen toggling
- [x] **menu**
	- [x] create a menu navigation box
		- [x] *play game*
		- [x] *select player*
		- [x] *minigames*
		- [x] *controls*
		- [x] *credits*
		- [x] *quit*
- [ ] **gameplay**
	- [x] centralized game controls functions
	- [x] implement screen fading effects
	- [x] implement external font drawing
	- [x] implement out-of-screen asset detection
	- [x] implement static/moving asset frame animation
	- [ ] set timeout at each level start where the player can't move (fading time, 2s)
- [x] **level 1**
	- [x] level design
	- [x] level assets and physic bodies definition
- [x] **level 2**
	- [x] level design
	- [x] level assets and physic bodies definition
	- [x] animate the portal and place/re-size it a little (more on the right)
- [ ] **level 3**
	- [x] level design
	- [x] level assets and physic bodies definition
        - [x] don't decrement gob amount if not killed
        - [x] set gobs maximum speed limit
	- [ ] replace "Dark Solean lifes" with a lives progress bar with the dark solean's head
	- [x] add fire launched by dark solin
	- [ ] make the portal disappear after fade effect timeout
- [x] **level 4**
	- [x] level design
	- [x] level assets and physic bodies definition
	- [x] alter player's movements on ice
- [x] **level bonus**
	- [x] level design
	- [x] level assets and physic bodies definition
	- [x] process asset/bonus collisions
- [x] **level 5** (boss ultimate battle, goblean)
	- [x] level design
	- [x] level assets and physic bodies definition
	- [x] set up different boss attacks
- [x] **installation**
	- [x] create a purpledrank.desktop file (/usr/share/applications/purpledrank.desktop)
	- [x] create icons (or single SVG) and link it (/usr/share/icons/hicolor/scalable/icon.svg)
	- [x] create an uninstall script to remove the above files
- [x] `project-outline.md`
	- [x] edit it to match our project's main components and units
