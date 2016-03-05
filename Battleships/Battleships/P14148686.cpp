/*  Program: Battleships Game
Programmer: Jegor Kharlamov
PNumber: P14148686
Date: 19/11/2014 */

#include <string>
#include <iostream>
#include <time.h>

using namespace std;

const int g_kiGridSize = 10; // A constant holding size of all arrays

int g_iCurrentPlayer = 1;

char g_acPlayerGrid[g_kiGridSize][g_kiGridSize]; // Holds player ship locations
char g_acPlayerHits[g_kiGridSize][g_kiGridSize]; // Holds players hit locations

char g_acEnemyGrid[g_kiGridSize][g_kiGridSize]; // Holds Enemy ship locations
char g_acEnemyHits[g_kiGridSize][g_kiGridSize]; // Holds Enemy ship locations

string g_sPlayer1Name; // Holds player 1's name
string g_sPlayer2Name; // Holds player 2's name

int g_iGameMode; // Holds data on if the player is playing a human or AI opponent

bool g_bGameEnd = false; // Runs as long as the game is not won.

int g_aiP1ShipHealth[5] = {2,3,3,4,5}; // Holds the ship health for the sub,cruiser,destroyer,battleship and air craft carrier

int g_aiP2ShipHealth[5] = {2,3,3,4,5}; // Holds the ship health for the sub,cruiser,destroyer,battleship and air craft carrier for player 2

void clearBoard(char acArray[10][10]); // Clears the board
void drawBoard(char acWhichBoard[10][10]); // Draws a board and places a given array inside of it.
void PlaceShips(); // Places the ships
void playerShooting(char acBoard[10][10], char acHitBoard[10][10],char acDmgBoard[10][10]); // Allows the players to shoot at eachothers ships
void playGame(); // Overall game loop
int switchCurrentPlayer(); // Switches the current player to the other player
int mainMenu(); // Draws the main menu
void shipDestroyed(char cHitChar); // If a ship is hit this function counts which ship was hit and if it has been destroyed.
void SetShipType(int &iCurrentShip, int &iShipBeingPlaced, string &sShipName, char &cShipChar); // 'For' statment which cycles through my games ships for placement.

int main()
{

	int iChoice = 0; // clears iChoice

	iChoice = mainMenu(); //Recives result from MainMenu function

	if (iChoice == 3) // If player selected exit...
	{
		return 0; // End program
	}
	else 
	{
		// Otherwise clear both grids
		clearBoard(g_acPlayerGrid);
		clearBoard(g_acEnemyGrid);
	}

	if (iChoice == 1)  // AI
	{
		g_sPlayer2Name = "BOT";
		g_iGameMode = 1; // Single Player
		drawBoard(g_acPlayerGrid);
		PlaceShips(); // Players place ships
		playGame();
		return 0; // Game has finished end the game.
	}

	if (iChoice == 2)
	{
		g_iGameMode = 2; // Multiplayer
		system("CLS");
		cout << g_sPlayer1Name << " will place his ships first." << endl;
		system("PAUSE");
		system("CLS");
		drawBoard(g_acPlayerGrid); // Draws both grids
		PlaceShips(); // Players place ships
		playGame();
		return 0;


	}
	system("PAUSE");
}

void drawBoard(char acWhichBoard[10][10])
{
	cout << "  "; // Inital spacing
	for( int iX=0; iX < g_kiGridSize; iX++)
	{
		cout << " " << iX + 1 << "  "; // Top Numbers
	}
	cout << endl;
	cout << " "; 
	for(int iX=0; iX < g_kiGridSize; iX++)
	{
		cout << "+---"; // Top line
	}
	cout << "+" << endl; // Final plus
	for(int iY=0; iY < g_kiGridSize; iY++)
	{
		cout << char(iY+'A'); // Displays side numbers
		for(int iX=0; iX < g_kiGridSize; iX++)
		{
			cout << "| " << acWhichBoard[iY][iX] << " "; // Places array inside grid
		}
		cout << "|" << endl;
		cout << " ";
		for(int iX=0; iX < g_kiGridSize; iX++)
		{
			cout << "+---"; // Draws last lines
		}
		cout << "+" << endl; // Final plus
	}
	cout << endl;
}


void clearBoard(char acArray[10][10])
{
	for(int iY=0; iY < g_kiGridSize; iY++) // Go though all values...
	{
			for(int iX=0; iX < g_kiGridSize; iX++)
			{
				acArray[iX][iY] = ' '; // And reset them to empty
			}
	}
}

int mainMenu()
{
	// Draws a main menu user interface
	int iMenuChoice;
	cout << "**************************************************" << endl;
	cout << "                    Battleships                   " << endl;
	cout << "**************************************************" << endl << endl;

	cout << "Welcome! What would you like to play?" << endl << endl;

	cout << "1.) Single Player" << endl;
	cout << "2.) Multiplayer" << endl;
	cout << "3.) Exit Game" << endl;
	
	cin >> iMenuChoice; // Asks for user's choice

	if (iMenuChoice == 1) // If single player is selected then ask for player's name
	{
		cout << "Enter Player Name: ";
		cin >> g_sPlayer1Name;
	}

	if (iMenuChoice == 2) // If multiplayer selected then ask for both players names
	{
		cout << "Enter Player 1 Name: ";
		cin >> g_sPlayer1Name;
		cout << "Enter Player 2 Name: "; 
		cin >> g_sPlayer2Name;
	}

	if (iMenuChoice == 3) // If exit selected send message
	{
		cout << "Thanks for playing" << endl;
		system("PAUSE");
	}

	system("CLS");
	return iMenuChoice;
}

void PlaceShips()
{
	string sShipName = "Submarine"; // Holds name for current ship

	int iCurrentShip = 1; // Holds the current selected ship amd changes to player can place all ships

	int iShipBeingPlaced = 1; // Stores the current ship being places and increments by 1 when a ship is placed.
	int iBlankCheck = 0; // Checks all spaces are empty before placing ship
	
	int iRandRot; // Stroes random directon for AI

	char cY; // Character value for grid

	int iX; // X grid value
	int iY; // Y grid value

	char cShipChar = ' '; // Holds the character which represents the ship in the grid
	int iWhichDir; // Holds the direction the player wants to place a ship

	int iDirection = 1; // Allows the user to pick ship rotation
	bool bPlacingShips = true; // While the player is placing ships

	while (bPlacingShips) // While the player is placing ships
	{
		{
			if (g_iCurrentPlayer == 1) // Let player 1 cycle through ships
			{
				SetShipType(iCurrentShip, iShipBeingPlaced, sShipName, cShipChar);
			}
			if (g_iCurrentPlayer == 2)
			{
				if (g_iGameMode == 1) // AI
				{
					srand (time(NULL)); // Initialise random
					for (iCurrentShip = 1; iCurrentShip < 6; iCurrentShip++) // For all ships
					{
						iX = rand() % 10 + 1; // Random number between 1-10
						iY = rand() % 10 + 1; // Random number between 1-10
						iRandRot = rand() % 4 +1;  // Random number between 1-4

						if (g_acEnemyGrid[iY-1][iX -1] == ' ') // If not already picked
						{
							SetShipType(iCurrentShip, iShipBeingPlaced, sShipName, cShipChar);

							g_acEnemyGrid[iY-1][iX-1] = cShipChar; // Mark location with currect character to represent ship
							iBlankCheck = 1; // Holds value that is used to check that the required grid space for a ship is clear.
								switch (iRandRot)
								{
									case 1: // Places ship upwards
										for (int j = 0; j < iShipBeingPlaced; j++) // For the length of the ship...
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ') // if the ship is within the grid and not intercepting another ship
											{
												iBlankCheck++; // Increment blank check...
											}
											iY -= 1; // Check next space
										}
											if (iBlankCheck == iShipBeingPlaced) // If all the spaces the ship needs are valid...
												{
													iY += iShipBeingPlaced; // Return iY to starting value
													for (int j = 0; j < iShipBeingPlaced; j++) // For the length of the current ship...
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar; // Place the ships character is spaces
														iY -= 1;
													}

												}

											else // If invalid location selected...
											{
												iY += iShipBeingPlaced; // Return iY to the starting place
												g_acEnemyGrid[iY-1][iX-1] = ' '; // Empty it
												iCurrentShip--; // Return ship to last ship.
											}
										
										
										break;
										
									case 2: // Places ship downwards
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iY += 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iY -= iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar;
														iY += 1;
													}

												}

											else
											{
												iY -= iShipBeingPlaced;
												g_acEnemyGrid[iY-1][iX-1] = ' ';
												iCurrentShip--;
											}

										break;
									case 3: // Places ship facing toward the right
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iX += 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iX -= iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar;
														iX += 1;													
													}

												}

											else
											{
												iX -= iShipBeingPlaced;
												g_acEnemyGrid[iY-1][iX-1] = ' ';
												iCurrentShip--;
											}

										break;

									case 4: // Places ship facing toward the left
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iX -= 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iX += iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar;
														iX -= 1;
													}

												}

											else
											{
												iX += iShipBeingPlaced;
												g_acEnemyGrid[iY-1][iX-1] = ' ';
												iCurrentShip--;
											}

										break;
							
									default:
										break;
								
								}
					}
					else
				{
					iCurrentShip -=1; // If ship location invalid. Try that ship again.
				}
				}
			}
			}
			SetShipType(iCurrentShip, iShipBeingPlaced, sShipName, cShipChar);

			if (iCurrentShip > 5)
			{
				return; // Returns when all 5 ships have been placed.
			}
			
			//Take input
			cout << "Please select the X,Y coordinates for your " << sShipName <<" : " << endl; 
			cout << "X: ";
			
			cin >> iX;
			cout << endl;
			
			if (iX - 1 > 9 || iX - 1 < 0)
			{
				cout << "That position is invalid. Please try again" << endl;
			}
			
			cout << "Y: ";
			cin >> cY; // Enter character X value

			iY = toupper(cY); // Convert to upper value			
			iY -= 64;

			//cout << iX;
			if (iY - 1 > 9 || iY - 1 < 0 )
			{
				cout << "That position is invalid. Please try again" << endl;
			}

			else // If postions are valid...
			{
					if (g_iCurrentPlayer == 1)
					{
						if (g_acPlayerGrid[iY - 1][iX - 1] == ' ') // and grid is location is empty
						{
							system("CLS");
							g_acPlayerGrid[iY-1][iX-1] = cShipChar; // Places character to represent ship
							drawBoard(g_acPlayerGrid);
							cout << "Which direction would you like it to face?" << endl;
							cout << "1.) UP" << endl << "2.) DOWN" << endl << "3.) RIGHT" << endl << "4.) LEFT" << endl;
							cin >> iWhichDir; // Asks for direction to be entered
						
							
								iBlankCheck = 1;
								switch (iWhichDir)
								{
									case 1: // Places ship upwards
										for (int j = 0; j < iShipBeingPlaced; j++) // For the length of the ship...
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 <= g_kiGridSize && g_acPlayerGrid[iY-1][iX-1] == ' ') // if the ship is within the grid and not intercepting another ship
											{
												iBlankCheck ++; // Increment blank check...
											}
											iY -= 1; // Check next space
										}
											if (iBlankCheck == iShipBeingPlaced) // If all the spaces the ship needs are valid...
												{
													iY += iShipBeingPlaced; // Return iY to starting value
													for (int j = 0; j < iShipBeingPlaced; j++) // For the length of the current ship...
													{
														
														g_acPlayerGrid[iY-1][iX-1] = cShipChar; // Place the ships character is spaces
														iY -= 1;
													}

												}

											else // If invalid location selected...
											{
												system("CLS"); 
												cout << "Invalid Location." << endl;
												iY += iShipBeingPlaced; // Return iY to the starting place
												g_acPlayerGrid[iY-1][iX-1] = ' '; // Empty it
												drawBoard(g_acPlayerGrid); 
												iCurrentShip--; // Return ship to last ship.
												system("PAUSE");
											}
										
										
										break;
										
									case 2: // Places ship downwards
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 <= g_kiGridSize && g_acPlayerGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iY += 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iY -= iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acPlayerGrid[iY-1][iX-1] = cShipChar;
														iY += 1;
													}

												}

											else
											{
												system("CLS");
												cout << "Invalid Location." << endl;
												iY -= iShipBeingPlaced;
												g_acPlayerGrid[iY-1][iX-1] = ' ';
												drawBoard(g_acPlayerGrid);
												iCurrentShip--;
												system("PAUSE");
											}

										break;
									case 3: // Places ship facing toward the right
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acPlayerGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iX += 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iX -= iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acPlayerGrid[iY-1][iX-1] = cShipChar;
														iX += 1;													
													}

												}

											else
											{
												system("CLS");
												cout << "Invalid Location." << endl;
												iX -= iShipBeingPlaced;
												g_acPlayerGrid[iY-1][iX-1] = ' ';
												drawBoard(g_acPlayerGrid);
												iCurrentShip--;
												system("PAUSE");
											}

										break;

									case 4: // Places ship facing toward the left
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 <= g_kiGridSize && g_acPlayerGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iX -= 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iX += iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acPlayerGrid[iY-1][iX-1] = cShipChar;
														iX -= 1;
													}

												}

											else
											{
												system("CLS");
												cout << "Invalid Location." << endl;
												iX += iShipBeingPlaced;
												g_acPlayerGrid[iY-1][iX-1] = ' ';
												drawBoard(g_acPlayerGrid);
												iCurrentShip--;
												system("PAUSE");
											}

										break;
							
									default:
										break;
								
								}
							system("CLS");
							drawBoard(g_acPlayerGrid);
							iCurrentShip++; // Increments value so next ship is placed.
							
						}

					}
					if (g_iCurrentPlayer == 2 && g_iGameMode == 2) // If human player 2 is playing
					{
						if (g_acEnemyGrid[iY-1][iX-1] == ' ')
						{
							system("CLS");
							g_acEnemyGrid[iY-1][iX-1] = cShipChar; // Places character to represent ship
							drawBoard(g_acEnemyGrid);
							cout << "Which direction would you like it to face?" << endl;
							cout << "1.) UP" << endl << "2.) DOWN" << endl << "3.) RIGHT" << endl << "4.) LEFT" << endl;
							cin >> iWhichDir;
						
							
								iBlankCheck = 1;
								switch (iWhichDir)
								{
									case 1: // Places ship upwards
										for (int j = 0; j < iShipBeingPlaced; j++) // For the length of the ship...
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ') // if the ship is within the grid and not intercepting another ship
											{
												iBlankCheck ++; // Increment blank check...
											}
											iY -= 1; // Check next space
										}
											if (iBlankCheck == iShipBeingPlaced) // If all the spaces the ship needs are valid...
												{
													iY += iShipBeingPlaced; // Return iY to starting value
													for (int j = 0; j < iShipBeingPlaced; j++) // For the length of the current ship...
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar; // Place the ships character is spaces
														iY -= 1;
													}

												}

											else // If invalid location selected...
											{
												system("CLS"); 
												cout << "Invalid Location." << endl;
												iY += iShipBeingPlaced; // Return iY to the starting place
												g_acEnemyGrid[iY-1][iX-1] = ' '; // Empty it
												drawBoard(g_acEnemyGrid); 
												iCurrentShip--; // Return ship to last ship.
												system("PAUSE");
											}
										
										
										break;
										
									case 2: // Places ship downwards
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iY += 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iY -= iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar;
														iY += 1;
													}

												}

											else
											{
												system("CLS");
												cout << "Invalid Location." << endl;
												iY -= iShipBeingPlaced;
												g_acEnemyGrid[iY-1][iX-1] = ' ';
												drawBoard(g_acEnemyGrid);
												iCurrentShip--;
												system("PAUSE");
											}

										break;

									case 3: // Places ship facing toward the right
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iX += 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iX -= iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar;
														iX += 1;													
													}

												}

											else
											{
												system("CLS");
												cout << "Invalid Location." << endl;
												iX -= iShipBeingPlaced;
												g_acEnemyGrid[iY-1][iX-1] = ' ';
												drawBoard(g_acEnemyGrid);
												iCurrentShip--;
												system("PAUSE");
											}

										break;


									case 4: // Places ship facing toward the left
										for (int j = 0; j < iShipBeingPlaced; j++)
										{
											if (iY - 1 >= 0 && iY -1 <= g_kiGridSize && iX - 1 >= 0 && iX - 1 < g_kiGridSize && g_acEnemyGrid[iY-1][iX-1] == ' ')
											{
												iBlankCheck ++;
											}
											iX -= 1;
										}
										if (iBlankCheck == iShipBeingPlaced)
												{
													iX += iShipBeingPlaced;
													for (int j = 0; j < iShipBeingPlaced; j++)
													{
														
														g_acEnemyGrid[iY-1][iX-1] = cShipChar;
														iX -= 1;
													}

												}

											else
											{
												system("CLS");
												cout << "Invalid Location." << endl;
												iX += iShipBeingPlaced;
												g_acEnemyGrid[iY-1][iX-1] = ' ';
												drawBoard(g_acEnemyGrid);
												iCurrentShip--;
												system("PAUSE");
											}

										break;
							
									default:
										break;
								
								}
									system("CLS");
									drawBoard(g_acEnemyGrid);
									iCurrentShip++;
						}
			}
		}
		
	}
	}
}

void playerShooting(char acBoard[10][10], char acHits[10][10], char acDmgBoard[10][10])
{ 
	
		char cHitChar = ' '; // Character that is hit
		char cY; // Shot value
		int ShootX; 
		int ShootY;
		bool bHit = true; // While the last shot was a hit
	

	while (bHit == true)
	{
		system("CLS");
		if (g_iCurrentPlayer == 1 || g_iCurrentPlayer == 2 && g_iGameMode == 2) // If its player 1 or a human player 2
		{
			drawBoard(acBoard);
			drawBoard(acHits);
		}
		if (g_iGameMode == 2 || g_iCurrentPlayer == 1) // If its player 1 and single player
		{
			cout << "Where would you like to shoot? (X,Y)" << endl;
			cout << "X: ";
		

			cin >> ShootY;
			cout << endl;
			if (ShootY > 10 || ShootY < 1) // Invalid location entered
			{
				cout << "That position is invalid. Please try again" << endl;
			}
		

		cout << "Y: ";
		cin >> cY; // Enter character X value

		ShootX = toupper(cY); // Convert to upper value			
		ShootX -= 64;

		if (ShootX > 10 || ShootX < 1 ) // Invalid location entered
		{
			cout << "That position is invalid. Please try again" << endl;
		}
		}
		if (g_iGameMode == 1 && g_iCurrentPlayer == 2) // Shoots randomly if AI opponent chosen.
		{
			ShootX = rand() % 10 + 1;
			ShootY = rand() % 10 + 1;
		}
		if (acHits[ShootX-1][ShootY-1] != 'X' && acHits[ShootX-1][ShootY-1] != 'M') // If player have not already shot this place before
		{
			if (acDmgBoard[ShootX-1][ShootY-1] != ' ') // Player hits then...
			{
				cHitChar = acDmgBoard[ShootX-1][ShootY-1]; // Find out which ship has been hit
				acHits[ShootX-1][ShootY-1] = 'X'; // Mark hit with an 'X'
				acDmgBoard[ShootX-1][ShootY-1] = 'X'; // Mark hit players ship with an 'X'
				system("CLS");
				if (g_iCurrentPlayer == 1 || g_iCurrentPlayer == 2 && g_iGameMode == 2)
				{
					drawBoard(acBoard);
				}
				drawBoard(acHits);
				shipDestroyed(cHitChar); // Minus health from the hit ship
				cout << "HIT!!" << endl;
				system("PAUSE");
			}
			else 
			{
				system("CLS"); // If player misses
				acHits[ShootX-1][ShootY-1] = 'M'; // Mark miss location with an 'M'
				if (g_iCurrentPlayer == 1 || g_iCurrentPlayer == 2 && g_iGameMode == 2) // If against human player
				{
					drawBoard(acBoard);
				}
				drawBoard(acHits);
				if (g_iCurrentPlayer == 1) // Player 1 missed
				{
				cout << endl << g_sPlayer1Name <<" missed" << endl;
				}
				else if (g_iCurrentPlayer == 2) // Player 2 missed
				{
					cout << endl << g_sPlayer2Name <<" missed" << endl;
				}
				system("PAUSE");
				system("CLS");
				bHit = false;
				return;
			}
		}
	}

}

int switchCurrentPlayer() // Switches the current player
{
	if (g_iCurrentPlayer == 1)
	{
	g_iCurrentPlayer = 2;
	}

	else if (g_iCurrentPlayer == 2)
	{
	g_iCurrentPlayer = 1;
	}

	return g_iCurrentPlayer;
}

void playGame()
{
	while (g_bGameEnd == false) // While game has not ended
	{
	switchCurrentPlayer(); // Switch Player
	cout << g_iCurrentPlayer << endl;
	if (g_iCurrentPlayer == 1) // Run routine for coresponding player
	{
		system("CLS");
		cout << g_sPlayer1Name << "'s turn" << endl;
		system("PAUSE");
		playerShooting(g_acPlayerGrid,g_acEnemyHits, g_acEnemyGrid);
	}
	if (g_iCurrentPlayer == 2)
	{
		system("CLS");
		cout << g_sPlayer2Name << "'s turn" << endl;
		system("PAUSE");
		playerShooting(g_acEnemyGrid,g_acPlayerHits, g_acPlayerGrid);
	}
	}
}

void shipDestroyed(char cHitsChar)
{
	if (g_iCurrentPlayer == 1) // If player 1's ships are hit.
	{
		switch (cHitsChar)
		{
			case 'S': // If sub hit
				g_aiP1ShipHealth[0]--; // Decrease Sub health
				if (g_aiP1ShipHealth[0] == 0) // If health is 0
				{
					cout << endl << "Submarine has been sunk." << endl; // Show sunk message
					system("PAUSE");
				}
			
				break;

			case 'D': // Destroyer
				g_aiP1ShipHealth[1]--;
				if (g_aiP1ShipHealth[1] == 0)
				{
					cout << endl << "Destoyer has been sunk." << endl;
					system("PAUSE");
				}
			
				break;

			case 'C': // Cruiser
				g_aiP1ShipHealth[2]--;
				if (g_aiP1ShipHealth[2] == 0)
				{
					cout << endl << "Cruiser has been sunk." << endl;
					system("PAUSE");
				}
			
				break;

			case 'B': // Battleship
				g_aiP1ShipHealth[3]--;
				if (g_aiP1ShipHealth[3] == 0)
				{
					cout << endl << "Battleship has been sunk." << endl;
					system("PAUSE");
				}

				break;

			case 'A': // Aircraft carrier
				g_aiP1ShipHealth[4]--;
				if (g_aiP1ShipHealth[4] == 0)
				{
					cout << endl << "Aircraft Carrier has been sunk." << endl;
					system("PAUSE");
				}
			
				break;
		}
	}

	else if (g_iCurrentPlayer == 2) // Same as above for player 2
	{
		switch (cHitsChar)
		{
			case 'S':
				g_aiP2ShipHealth[0]--;
				if (g_aiP2ShipHealth[0] == 0)
				{
					cout << endl << "Submarine has been sunk." << endl;
					system("PAUSE");
				}
			
				break;

			case 'D':
				g_aiP2ShipHealth[1]--;
				if (g_aiP2ShipHealth[1] == 0)
				{
					cout << endl << "Destoyer has been sunk." << endl;
					system("PAUSE");
				}
			
				break;

			case 'C':
				g_aiP2ShipHealth[2]--;
				if (g_aiP2ShipHealth[2] == 0)
				{
					cout << endl << "Cruiser has been sunk." << endl;
					system("PAUSE");
				}
			
				break;

			case 'B':
				g_aiP2ShipHealth[3]--;
				if (g_aiP2ShipHealth[3] == 0)
				{
					cout << endl << "Battleship has been sunk." << endl;
					system("PAUSE");
				}

				break;

			case 'A':
				g_aiP2ShipHealth[4]--;
				if (g_aiP2ShipHealth[4] == 0)
				{
					cout << endl << "Aircraft Carrier has been sunk." << endl;
					system("PAUSE");
				}
			
				break;
		}
	}

	if (g_aiP1ShipHealth[0] == 0 && g_aiP1ShipHealth[1] == 0 && g_aiP1ShipHealth[2] == 0 && g_aiP1ShipHealth[3] == 0 && g_aiP1ShipHealth[4] == 0) // All a player 1's ships have been destroyed.
	{
		system("CLS");
		cout << g_sPlayer1Name <<" Wins." << endl;
		system("PAUSE");
	}

	if (g_aiP2ShipHealth[0] == 0 && g_aiP2ShipHealth[1] == 0 && g_aiP2ShipHealth[2] == 0 && g_aiP2ShipHealth[3] == 0 && g_aiP2ShipHealth[4] == 0) // All player 2's ships have been destroyed.
	{
		system("CLS");
		cout << g_sPlayer2Name <<" Wins." << endl;
		system("PAUSE");
	}

}

void SetShipType(int &iCurrentShip, int &iShipBeingPlaced, string &sShipName, char &cShipChar) // Cycles through all ships
{
	// Constants holding lengths of different ships
	const int ikSubmarine = 2;
	const int ikDestroyer = 3;
	const int ikCruiser = 3;
	const int ikBattleship = 4;
	const int ikAirCraftCarrier = 5;

	switch (iCurrentShip)
	{
		case 1: // Sub
			iShipBeingPlaced = ikSubmarine;
			sShipName = "Submarine";
			cShipChar = 'S';
			
			break;

		case 2: // Destroyer
			iShipBeingPlaced = ikDestroyer;
			sShipName = "Destroyer";
			cShipChar = 'D';

			break;

		case 3: // Cruiser
			iShipBeingPlaced = ikCruiser;
			sShipName = "Cruiser";
			cShipChar = 'C';

			break;

		case 4: // BattleShip
			iShipBeingPlaced = ikBattleship;
			sShipName = "Battleship";
			cShipChar = 'B';

			break;

		case 5: // Air Craft Carrier
			iShipBeingPlaced = ikAirCraftCarrier;
			sShipName = "AirCraft Carrier";
			cShipChar = 'A';

			break;

		default: // Runs correct default depending on if players are human or AI

			system("CLS");
			if (g_iCurrentPlayer == 1)
			{
				if (g_iGameMode == 1)
				{
					cout << "Your ships have been placed." << endl;
					system("PAUSE");
					system("CLS");
					g_iCurrentPlayer = 2;
						
				}

				if (g_iGameMode == 2)
				{
					cout << "Ships placed. Pass the game to " << g_sPlayer2Name << " so they may place ships." << endl;
					system("PAUSE");
					system("CLS");
					g_iCurrentPlayer = 2;
					drawBoard(g_acEnemyGrid);
				}

				iCurrentShip = 1;
				break;
			}
			
			if (g_iCurrentPlayer == 2)
			{
				cout << g_sPlayer2Name << " has finished placing his ships." << endl;
				cout << g_sPlayer1Name << " will take the first shot" << endl;
				system("PAUSE");
				g_iCurrentPlayer = 2;
				return;
			}

			break;
			
	}
}