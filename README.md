# Mini Space Shooter Game Microcontroller

## Overview
The Mini Space Shooter Game is a retro-style space shooter game designed to be played on an Arduino Nano microcontroller. It offers a fun and engaging experience suitable for players of all ages, whether experienced or beginners. The game features a start screen, a game screen, and a game-over screen, all displayed on a 128x64 OLED screen.

The player controls a spaceship, fires bullets at enemies, and tries to survive while accumulating points. The game keeps track of the player's lives, elapsed time, score, and level, displayed on the OLED screen.

This project was heavily inspired by [Volos Projects](https://www.youtube.com/@VolosProjects) 

## Features
- **Start Screen & Game Over Screen**: Shows up on the OLED screen at the beginning and end of the game.
- **Player Control**: The player can move the ship up and down and fire bullets using physical buttons.
- **Score Tracking**: The game displays the score and final results after each session.
- **Sound Effects**: Includes sound effects for firing bullets and interactions using a buzzer.
- **Dynamic Difficulty**: The game increases in difficulty as the player progresses through levels.

## Components
- **Arduino Nano**: The microcontroller that runs the game.
- **128x64 I2C OLED Display**: Displays the game's user interface and game elements.
- **Push Buttons x3**: Used to control the ship's movements and fire bullets.
- **Buzzer**: Plays sound effects during the game.
- **Wires & Breadboard**: Connects all the components together.

## Wiring Diagram
- **Button 1 (Fire)**: Connected to D3 and GND.
- **Button 2 (Move Up)**: Connected to D12 and GND.
- **Button 3 (Move Down)**: Connected to D13 and GND.
- **OLED Display**: 
  - GND → Arduino GND
  - VDD → Arduino 5V
  - SCL → Arduino A5
  - SDA → Arduino A4
- **Buzzer**: Connected to D9 and GND.

## Code Structure
The project is programmed in C++ using the Arduino IDE, with libraries such as `Adafruit_GFX` and `Adafruit_SSD1306` for controlling the OLED display. The code is structured into various functions:
- **setup()**: Initializes the display, buttons, and game settings.
- **loop()**: Continuously runs the game, checking for user input, updating the game state, and rendering to the display.
- **resetGame()**: Resets the game variables to start a new game.
- **updateGame()**: Updates player positions, bullets, and enemy behaviors.
- **drawGame()**: Draws the game objects, including the player, enemy, bullets, and game HUD on the OLED screen.
- **checkCollisions()**: Handles bullet collisions and updates the player's lives and score.

## Challenges Faced
- **Wiring**: Arranging the small components and wires on a breadboard was challenging due to space constraints.
- **Screen Resolution**: Adapting the game’s dimensions to fit the small OLED display required fine-tuning and trial and error.
- **Sound Quality**: Adjusting the frequency and timing of the buzzer to ensure that the music played correctly without delays.

## Simulation and Testing
The project was successfully simulated using the online tool [Wokwi](https://wokwi.com/), allowing for testing and debugging without the need for physical hardware.

## Future Enhancements
The project can be further expanded by:
- Adding different enemy types.
- Incorporating power-ups and special abilities.
- Improving game graphics by adding more complex sprites and animations.

## References
- Ismailov A. S. & Jo‘Rayev Z. B. (2022). Study of Arduino Microcontroller Board. *Science and Education*, 3(3), 172-179.
- [Arduino Nano Datasheet](https://www.mantech.co.za/datasheets/products/a000005-6s.pdf)
- [OLED Screen Documentation](https://sensorkit.arduino.cc/sensorkit/module/lessons/lesson/10-the-oled-screen)
- [Arduino Star Wars Game - how to make](https://www.youtube.com/watch?v=lOz_GuME63E)
