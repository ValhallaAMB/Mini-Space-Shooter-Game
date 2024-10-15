// Includes the necessary libraries for OLED display and graphics
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions
const int buttonUpPin = 12;
const int buttonDownPin = 11;
const int buttonFirePin = 3;
const int buzzerPin = 9;

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// I2C address for the display
#define SCREEN_ADDRESS 0x3C

// Game Constants
const int playerWidth = 16;
const int playerHeight = 16;
const int playerStartPos = 24;
const int playerBulletSpeed = 8;
const int enemyBulletSpeedStart = 2;
const int enemyStartDiameter = 12;
const int playerStartLives = 5;
const int levelUpInterval = 10; // 10 points

// Game State Variables
int playerPos = playerStartPos;
int playerBulletX = 0;
int playerBulletY = 0;
bool playerBulletFired = false;

int enemyPosY = 8;
int enemyDirection = 0;
int enemyDiameter = enemyStartDiameter;
int enemyBulletSpeed = enemyBulletSpeedStart;
int enemySpeedAttribute = 6;
int numEnemyBullets = 0;

int score = 0;
int level = 1;
int lives = playerStartLives;
unsigned long inGameTime = 0;
unsigned long levelUpScore = 0;
unsigned long lastBulletLaunchTime = 0;
unsigned long gameStartTime = 0;

bool gameRunning = true;
bool gameOver = false;

// Enemy Bullets
struct Bullet {
  int x, y;
  int diameter;
};

Bullet enemyBullets[4];

// Define musical notes frequencies
const int f = 1396;   // F
const int a = 1865;   // A
const int cH = 2093;  // High C

// Player sprite (bitmap)
const uint8_t dioda16[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x3F, 0xF0, 0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0x7F, 0xFF,
0x7F, 0xFF, 0xFF, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x1F, 0xF0, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Bitmap loading screen
const unsigned char storm [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x00, 0x07, 0x80, 0x01, 0xE0, 0x00, 0x00, 0x0C,
0x00, 0x00, 0x20, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 0x04, 0x00,
0x00, 0x20, 0x00, 0x00, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00, 0x04, 0x00, 0x00, 0x60, 0x00, 0x00,
0x02, 0x00, 0x00, 0x40, 0x00, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40,
0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x01, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x01, 0x00,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xD7, 0xFF, 0xFF,
0xE1, 0x00, 0x01, 0xBF, 0xFC, 0x1F, 0xFA, 0x80, 0x01, 0xBF, 0xF1, 0xCF, 0xFA, 0x80, 0x01, 0x3F,
0xC2, 0x37, 0xF7, 0x80, 0x01, 0xEF, 0x9C, 0x01, 0xE7, 0xC0, 0x01, 0xE0, 0x70, 0x06, 0x06, 0x80,
0x01, 0xE0, 0xC0, 0x03, 0x06, 0x80, 0x01, 0xFF, 0x80, 0x01, 0xFF, 0x80, 0x01, 0xF8, 0x00, 0x00,
0x1D, 0xC0, 0x03, 0x70, 0x00, 0x80, 0x0C, 0x60, 0x05, 0xB0, 0x07, 0xF0, 0x08, 0x90, 0x09, 0x10,
0x1F, 0xF8, 0x09, 0xD0, 0x0B, 0x90, 0x1F, 0x7C, 0x03, 0xF0, 0x0F, 0xC0, 0xFC, 0x0F, 0x07, 0x90,
0x0D, 0x43, 0xC0, 0x03, 0x07, 0x90, 0x05, 0x64, 0x00, 0x00, 0xCF, 0x10, 0x07, 0xFC, 0x00, 0x00,
0x26, 0x10, 0x01, 0x80, 0x00, 0x00, 0x10, 0x20, 0x01, 0x00, 0x00, 0x00, 0x0E, 0x40, 0x01, 0x80,
0x07, 0xF0, 0x01, 0x80, 0x00, 0x80, 0x07, 0xC8, 0x00, 0x80, 0x00, 0x80, 0x0B, 0xE8, 0x00, 0x80,
0x00, 0x87, 0x97, 0xE9, 0xE0, 0x80, 0x00, 0x87, 0xDF, 0xEF, 0xA0, 0x80, 0x00, 0x4B, 0xFF, 0xFF,
0xA0, 0x80, 0x00, 0x6B, 0xDF, 0xFB, 0xA3, 0x00, 0x00, 0x24, 0x97, 0xE8, 0x24, 0x00, 0x00, 0x1E,
0x1F, 0xC0, 0x2C, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00
};

// Main setup function
void setup() {
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;); // Don't proceed, loop forever
  }
  display.display();

  // Clear the buffer.
  display.clearDisplay();

  // Display bitmap image
  display.drawBitmap(40, 9, storm, 48, 48, WHITE);
  display.display();

  // Play startup melody
  beep(a, 500);
  beep(a, 500);    
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(1500);
  display.clearDisplay();

  // Initialize pins for buttons and buzzer
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonFirePin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // Initialize game
  resetGame();
}

// Main loop function
void loop() {
  if (gameRunning) {
    inGameTime = millis() / 1000;
    updateGame();
    drawGame();
  } else if (gameOver) {
    displayGameOver();
  }
}

// Function to update game state
void updateGame() {
  updatePlayer();
  updateBullets();
  updateEnemy();
  checkCollisions();
  checkLevelUp();
}

// Function to draw game on the OLED display
void drawGame() {
  display.clearDisplay();
  drawStars();
  drawPlayer();
  drawEnemy();
  drawBullets();
  drawHUD();
  display.display();
}

// Function to update player's position and bullet firing
void updatePlayer() {
  // Handle player movement and firing
  if (digitalRead(buttonUpPin) == LOW && playerPos >= 2) {
    playerPos -= 2;
  }
  if (digitalRead(buttonDownPin) == LOW && playerPos <= SCREEN_HEIGHT - playerHeight - 2) {
    playerPos += 2;
  }
  if (digitalRead(buttonFirePin) == LOW && !playerBulletFired) {
    playerBulletFired = true;
    playerBulletX = playerWidth;
    playerBulletY = playerPos + 8;
    beep(1200, 20); // Play firing sound
  }
  if (playerBulletFired) {
    playerBulletX += playerBulletSpeed;
    if (playerBulletX > SCREEN_WIDTH) {
      playerBulletFired = false;
    }
  }
}

// Function to update enemy bullets and launch new bullets
void updateBullets() {
  for (int i = 0; i < numEnemyBullets; ++i) {
    enemyBullets[i].x -= enemyBulletSpeed;
    if (enemyBullets[i].x < 0) {
      enemyBullets[i] = enemyBullets[--numEnemyBullets];
    }
  }
  if (millis() - lastBulletLaunchTime > random(400, 1200)) {
    launchEnemyBullet();
  }
}

// Function to update enemy's position and behavior
void updateEnemy() {
  if (enemyDirection == 0) {
    enemyPosY += enemyDiameter / enemySpeedAttribute;
  } else {
   enemyPosY -= enemyDiameter / enemySpeedAttribute;
  }
  // Bottom of the screen
  if (enemyPosY >= SCREEN_HEIGHT - enemyDiameter) {
    enemyDirection = 1;
  }
  // Top of the screen
  if (enemyPosY <= 13) {
    enemyDirection = 0;
  }
}

// Function to check if player leveled up and adjust game parameters
void checkLevelUp() {
  if ((levelUpScore + 1) % 10 == 0) {
    levelUpScore = 0;
    level++;
    enemyBulletSpeed++;
    if (level % 2 == 0) {
      enemyDiameter = max(1, enemyDiameter - 1);
      enemySpeedAttribute = max(1, enemySpeedAttribute - 1);
    }
  }
}

// Function to draw background stars
void drawStars() {
  int starCoords[18][2] = {
    {50, 30}, {30, 17}, {60, 18}, {55, 16}, {25, 43},
    {100, 43}, {117, 52}, {14, 49}, {24, 24}, {78, 36},
    {80, 57}, {107, 11}, {150, 11}, {5, 5}, {8, 7},
    {70, 12}, {10, 56}, {70, 25}
  };
  for (int i = 0; i < 18; ++i) {
    display.drawPixel(starCoords[i][0], starCoords[i][1], WHITE);
  }
}

// Function to draw player's spaceship
void drawPlayer() {
  display.drawBitmap(4, playerPos, dioda16, playerWidth, playerHeight, WHITE);
  if (playerBulletFired) {
    display.drawLine(playerBulletX, playerBulletY, playerBulletX + 4, playerBulletY, WHITE);
  }
}

// Function to draw enemy's spaceship
void drawEnemy() {
  display.fillCircle(SCREEN_WIDTH - 15, enemyPosY, enemyDiameter, WHITE);
  display.fillCircle(SCREEN_WIDTH - 13, enemyPosY + 3, enemyDiameter / 3, BLACK);
}

// Function to draw bullets on the display
void drawBullets() {
  for (int i = 0; i < numEnemyBullets; ++i) {
    display.drawCircle(enemyBullets[i].x, enemyBullets[i].y, enemyBullets[i].diameter, WHITE);
  }
}

// Function to draw HUD (score, lives, level, time) on the display
void drawHUD() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(33, 57);
  display.println("Score:");
  display.setCursor(68, 57);
  display.println(score);
  
  display.setCursor(33, 0);
  display.println("Lives:");
  display.setCursor(68, 0);
  display.println(lives);
  
  display.setCursor(110, 0);
  display.println("L:");
  display.setCursor(122, 0);
  display.println(level);

  display.setCursor(108, 57);
  display.println(inGameTime);
}

// Function to check collisions (player with enemy bullets and vice versa)
void checkCollisions() {
  // Check player bullet collision with enemy
  if (playerBulletFired && 
      playerBulletY >= enemyPosY - enemyDiameter && playerBulletY <= enemyPosY + enemyDiameter &&
      playerBulletX >= SCREEN_WIDTH - 10 - enemyDiameter && playerBulletX <= SCREEN_WIDTH - 10 + enemyDiameter) {
    playerBulletFired = false;
    score++;
    levelUpScore++;
    beep(500, 20);
  }

  // Check enemy bullets collision with player
  for (int i = 0; i < numEnemyBullets; ++i) {
    int bulletY = playerPos + 8;
    if (enemyBullets[i].y >= bulletY - 8 && enemyBullets[i].y <= bulletY + 8 &&
        enemyBullets[i].x >= 4 && enemyBullets[i].x <= 12) {
      lives--;
      if (lives <= 0) {
        gameOver = true;
        gameRunning = false;
      }
      enemyBullets[i] = enemyBullets[--numEnemyBullets];
      beep(100, 20);
    }
  }
}

// Function to display Game Over and score
void displayGameOver() {
  // Play game over melody
  if (lives == 0) {
    tone(9, 200, 300);
    delay(300);
    tone(9, 250, 200);
    delay(200);
    tone(9, 300, 300);
    delay(300);
    lives = playerStartLives; // Reset lives to initial value
  }

  display.clearDisplay(); // Clear the OLED display
  display.setTextSize(2); // Set text size to 2x
  display.setCursor(14, 10); // Set cursor position for "Game Over"
  display.println("Game Over"); // Display "Game Over"
  display.setTextSize(1); // Set text size back to 1x
  display.setCursor(30, 34); // Set cursor position for "Final Score:"
  display.println("Final Score:"); // Display "Final Score:"
  display.setCursor(70, 46); // Set cursor position for the actual score value
  display.println(score); // Display the current score
  display.display(); // Update the display
  
  // Restart game if button is pressed
  if (digitalRead(buttonFirePin) == LOW) {
    tone(9, 280, 300);
    delay(300);
    tone(9, 250, 200);
    delay(200);
    tone(9, 370, 300);
    delay(300);
    resetGame(); // Reset the game state
  }
}

void resetGame() {
  // Reset player position and bullet state
  playerPos = playerStartPos;
  playerBulletX = 0;
  playerBulletY = 0;
  playerBulletFired = false;

  // Reset enemy parameters
  enemyPosY = 8;
  enemyDirection = 0;
  enemyDiameter = enemyStartDiameter;
  enemyBulletSpeed = enemyBulletSpeedStart;
  numEnemyBullets = 0;

  // Reset game score, level, and time
  score = 0;
  level = 1;
  levelUpScore = 0;
  lastBulletLaunchTime = 0;
  gameStartTime = 0;
  inGameTime = 0;

  // Reset game state flags
  gameRunning = true;
  gameOver = false;
}

void launchEnemyBullet() {
  int diameter = random(2, 7); // Generates a random size between 7 and 12 for bullet size
  if (numEnemyBullets < 3) { // Only launch bullet if there are less than 3 bullets already
    enemyBullets[numEnemyBullets++] = {SCREEN_WIDTH - 10, enemyPosY, diameter}; // Add bullet at enemy position
    lastBulletLaunchTime = millis(); // Record the launch time
  }
}

void beep(int note, int duration) {
  tone(buzzerPin, note, duration); // Generate a tone for specified duration
  delay(duration); // Delay for the specified duration
  noTone(buzzerPin); // Stop the tone
  delay(50); // Small delay after the tone
}

long readVcc() {
  // Determine the appropriate ADC settings for reading Vcc based on the microcontroller type
  #if defined(_AVR_ATmega32U4) || defined(AVR_ATmega1280) || defined(AVR_ATmega2560_)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (_AVR_ATtiny24) || defined(AVR_ATtiny44) || defined(AVR_ATtiny84_)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (_AVR_ATtiny25) || defined(AVR_ATtiny45) || defined(AVR_ATtiny85_)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // Wait for measurement

  uint8_t low  = ADCL; // Read ADCL first to lock ADCH
  uint8_t high = ADCH; // Read ADCH

  long result = (high << 8) | low; // Combine high and low bytes
  result = 1125300L / result; // Calculate Vcc (in mV)
  return result; // Return Vcc in millivolts
}