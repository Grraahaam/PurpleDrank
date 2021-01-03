

# PurpleDrank - Project's outline

This document lists the project's components and their assigned developers.

PurpleDrank aims to become a KISS (Keep It Stupid Simple) game in its developement. We're creating our customized game-engine on top of the Raylib existing engine.

The development is organized over several units listed below from the most mandatory to the less important:

# Table of content

- [Project Framing](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/feat/graham/PROJECT_OUTLINE.md#unit-1-project-framing)
- [Background and assets design](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/feat/graham/PROJECT_OUTLINE.md#unit-2-background-and-assets-design)
- [Level design](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/feat/graham/PROJECT_OUTLINE.md#unit-3-level-design)
- [Refactoring](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/feat/graham/PROJECT_OUTLINE.md#unit-4-refactoring)
- [User Interface](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/feat/graham/PROJECT_OUTLINE.md#unit-5-user-interface)
- [Immersion & Diverity](https://gvipers.imt-lille-douai.fr/fatus/purple-drank-2020/purple-drank/feat/graham/PROJECT_OUTLINE.md#unit-6-immersion-diversity)

## Unit-1 Project Framing

```
 @MainDeveloper:        Graham
 @SecondaryDeveloper:   Milan

```

Creating the base structure and giving developers the tools to get their jobs done. (*This functionality is sometimes closely linked with the Unit-4*)

- Developers can communicate with others (Discord)
- A developer can share a working directory (GitLab)
- Tests programmes can be completed and executed.
- Everyone can refer to documentation.
- A realize by merging contribution could be generated.
- A debug mode is implemented to speed up issue tracking.
- Make sure that the whole project is coherent (both design and code)

## Unit-2 Background and assets design

```
 @MainDeveloper: Liam

```

PurpleDrank is a game played in an environment based on our first idea which was the lean. So, game, levels and menus design must be coherent and purple-ized.

- Create level backgrounds of mixed complexity, allowing level designers to add multiple functionnalities
- Create spritecheets allowing us to animate the main character and the game's assets to reduce loading time and improve game's UX
- Create multiple screen for better immersion and UX (menu, gameover, victory, character selection, controls, credits,...)


## Unit-3 Level Design

```
 @MainDeveloper:        Hugo
 @SecondaryDeveloper:   Milan

```

PurpleDrank is a "Die and Retry" game. In this purpose, levels should be designed to anger the player.

- Create levels with increasing difficulty.
- Add traps and ennemies so that player can die.
- Add physics body to interact with the gravity (jump and player movements).
- Add bonuses and powers so that player can beat ennemies
- Create levels that can be finished (even for the hardest ones)

## Unit-4 Refactoring

```
 @MainDevelopers:       Graham
 @SecondaryDevelopers:  Milan

```

Make sure that the whole code respect the same chart and the same organisation.


- Each screen had its own file (structured within documented directories).
- Make sure variables and structures definitions/uses follows the good pratices.
- Create structures to get an object oriented programming.
- Split repetitive operations into functions to optimize the developement and reduce overall code complexity


## Unit-5 User Interface

```
 @MainDevelopers: Graham

```

This functionality focus on the possibility for the user to access all levels and functionnality.

- The game starts on a menu screen with different possibilities.
- The game's screens aims to follow the team's artistic direction.
- Player can customize his own key binding but also his own character (Solean) to play the game.
- Make sure the player can get informations about the game's key shortcuts
- Make the positioning relative to improve screen scalability (fullscreen, adaptive screen)

## Unit-6 Immersion & Diversity

```
 @MainDevelopers: Bilal & Najib

```

This functionality focus on the addition of immersion and diversity to our game.

- Player can play to mini-games if they succeeded the Story mode.
- Music is played in the background, for improved immersion.
- Sound effect are applied to the game's events (fire, ennemy hit, portal, ...)

