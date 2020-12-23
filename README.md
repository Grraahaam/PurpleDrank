[![Dev Badge](https://img.shields.io/badge/DEV-PurpleTeam-9400D3?style=for-the-badge&logo=discord&logoColor=9400D3)](https://discord.com/login)

# PurpleDrank
A *rage-proof*, and *pixel-perfect* jump game, powered by Raylib framework.
The main character is a remake of Sonic, named Solin, refering to the *Lean* which is the in-game resource.

![PurpleDrank Splashscreen](./res/backgrounds/splash.png "PurpleDrank")

## Story :fr:
*Solin et son Ekip*

Le monde de Lealle est attaqué par un virus mortel, la fièvre Ebolean, causée par le Gob-Lean lorsqu'il se fit quitter par sa copean. Cette rage se fit ressenti lors d'orages violets, les purple rain, rendant le monde accroc à cette substance qu'est la lean.

Dans le monde chaotique de Lealle, vous aiderez Solin et son Ekip, à vaincre leur ennemi juré, à travers une aventure et des mini-jeux d'enfer!

**Récupérer un maximum de seaux de Cristalean afin de pouvoir vaincre le Gob-Lean!**

## Installation :joystick:
To install and play this game, please run the following:
```
sudo apt install -y git
git clone http://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank.git PurpleDrank
cd PurpleDrank && make
./PurpleDrank

# To install permanently
sudo make install

# To uninstall, removing all game files
sudo make uninstall
```

## Contribution :pencil2:
For contribution instructions, please read this [documentation](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/blob/feat/graham/src/README.md)

To see the overall features we worked on and those we're implementing, check the [TODO](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/blob/feat/graham/TODO.md)

To see the list of the contributors and their assignements, read the [PROJECT-OUTLINE.md](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/blob/feat/graham/PROJECT-OUTLINE.md)

## Structure :heavy_check_mark:
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

## Credits
**@** The Purple Team 

(*The project is open source, licensed under [MIT License](https://en.wikipedia.org/wiki/MIT_License)*)
