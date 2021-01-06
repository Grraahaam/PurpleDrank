[![Dev Badge](https://img.shields.io/badge/DEV-PurpleTeam-9400D3?style=for-the-badge&logo=discord&logoColor=9400D3)](https://discord.com/login)

# PurpleDrank
A *rage-proof*, and *pixel-perfect* jump game, powered by Raylib framework.
The main character is a remake of Sonic, named Solin, refering to the *Lean* which is the in-game resource.

![PurpleDrank Splashscreen](./res/PurpleDrank.gif "PurpleDrank Demo")

## Story :fr:
*Solin et son Ekip*

Le monde de Lealle est attaqué par un virus mortel, la fièvre Ebolean, causée par le Gob-Lean lorsqu'il se fit quitter par sa copean. Cette rage se fit ressenti lors d'orages violets, les purple rain, rendant le monde accroc à cette substance qu'est la lean.

Dans le monde chaotique de Lealle, vous aiderez Solin et son Ekip, à vaincre leur ennemi juré, à travers une aventure et des mini-jeux d'enfer!

**Récupérer un maximum de seaux de Cristalean afin de pouvoir vaincre le Gob-Lean!**

## Gameflow
PurpleDrank's adventure mode is made of **5 levels** and **1 bonus stage**. A boss can be met in the third and fifth levels. Don't worry if you do not succeed, you're maybe not good enough yet ;)

PurpleDrank also includes minigames to train your ability for the adventure mode.

## Installation :joystick:
To install and play this game, please run the following:
```bash
# Install git and make
sudo apt install -y git build-essential

# Fetch the game's project
git clone http://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank.git PurpleDrank

# Build it
cd PurpleDrank && make
./PurpleDrank

# To install permanently
sudo make install

# To uninstall, removing all game files
sudo make uninstall
```

## Controls :keyboard:
**QWERTY layout**
The default game controls are listed below, we wanted to go pro and present a customizable controls screen, but you know, kids, family...

| Keys | Description |
|:-:|-|
| <kbd>ESC</kbd> | Exit the game |
| <kbd>ENTER</kbd> | Confirm selection and skip screens/dialogs |
| <div style="display: flex; flex-flow: column nowrap; justify-content: center; align-items: center;"><div style="display: block"><kbd>↑</kbd></div><div style="display: block"><kbd>←</kbd><kbd>↓</kbd><kbd>→</kbd></div></div>  | Controls player's movements |
| <kbd>SPACE</kbd> | Interact with the launchpad/Use the JetLean |
| <kbd>R</kbd> | Player "super" mode (ball, faster) |
| <kbd>Q</kbd> | Throw lean (when possible) |
| <kbd>W</kbd> | Enable the JetLean (when possible) |
| <kbd>E</kbd> | Generate portals (when possible) OR enable Gelano mode (to stop ice-slipping) |
| <kbd>F1</kbd> | Return to the menu (game resets) |
| <kbd>F3</kbd> | Toggle debug mode |

## Contribution :pencil2:
For contribution instructions, please read this [documentation](src/README.md)

To see the overall features we worked on and those we're implementing, check the [TODO](TODO.md)

To see the list of the contributors and their assignements, read the [PROJECT-OUTLINE.md](PROJECT-OUTLINE.md)

The game tries to follow [16:9 viewport ratio](https://en.wikipedia.org/wiki/16:9). Currently using **1024x576** ratio, the in-game's dimensions are relative to the any ratio, we choosed a static one instead of fullscreen mode for better support (our fullscreen auto-scale methods doesn't work with all of our beta-testers)

## Credits
**@** The Purple Team 

(*The project is open source, licensed under [MIT License](https://en.wikipedia.org/wiki/MIT_License)*)
