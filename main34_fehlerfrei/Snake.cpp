#include "Snake.h"
#include "pitches.h"

int melody6[] = {
  NOTE_E3, NOTE_DS3,NOTE_AS3, NOTE_E3, NOTE_DS3,NOTE_AS3 };
int noteDurations6[] = {
  32,32,32,32,32,32 };

char   str51[]   = "GAME";
char   str52[]   = "over";
char   str53[]   = "score";
char   str54[]   = "WIN";
char   str55[]   = "Name";


Snake::Snake(NESController *controller, Adafruit_WS2801 *strip, RGBmatrixPanel *matrix):Game(controller, strip, matrix){
	segments = NULL;
	moveDirection = XDOWN;
	score = 0;
	highScore=0;
	gameOver=false;
	initialsPos=0;
	lastMoveTime=0;
	growing=0;

	highScore = ((unsigned long)EEPROM.read(7) << 24)+((unsigned long)EEPROM.read(8) << 16)+((unsigned long)EEPROM.read(9) << 8)+((unsigned long)EEPROM.read(10));
	highInitials[0] = EEPROM.read(11);
	highInitials[1] = EEPROM.read(12);
	highInitials[2] = EEPROM.read(13);
	newGame();
	/*EEPROM.write(7,0);
	EEPROM.write(8,0);
	EEPROM.write(9,0);
	EEPROM.write(10,1);
	EEPROM.write(11,'A');
	EEPROM.write(12,'A');
	EEPROM.write(13,'A');*/
}

Snake::~Snake(){
	deleteSnake();
}

void Snake::newGame(){
	randomSeed(analogRead(0));
	
	score=0;
	gameOver=0;
	lastMoveTime = millis();
	moveDirection=XRIGHT;
	deleteSnake();
	segments = new SnakeSegment(4,1,NULL);
	segments = new SnakeSegment(4,2,segments);
	segments = new SnakeSegment(4,3,segments);
	addFood();
	redrawBoard();
	updateDisplay();
}

void Snake::run(){
	if(controller->buttonPressed(controller->BUTTON_SELECT) && !controller->buttonHandled(controller->BUTTON_SELECT)){
		controller->handleButton(controller->BUTTON_SELECT);
		newGame();
	}
	if(gameOver){
		if(score>highScore){
			if(initialsPos>2){
				saveHighScore();
			}else{
				if(controller->buttonPressed(controller->BUTTON_DOWN) && !controller->buttonHandled(controller->BUTTON_DOWN)){
					controller->handleButton(controller->BUTTON_DOWN);
					if(initials[initialsPos]>'A'){
						initials[initialsPos]--;
					}else{
						initials[initialsPos]='Z';
					}
				}
				if(controller->buttonPressed(controller->BUTTON_UP) && !controller->buttonHandled(controller->BUTTON_UP)){
					controller->handleButton(controller->BUTTON_UP);
					if(initials[initialsPos]<'Z'){
						initials[initialsPos]++;
					}else{
						initials[initialsPos]='A';
					}
				}
				if(controller->buttonPressed(controller->BUTTON_A) && !controller->buttonHandled(controller->BUTTON_A)){
					controller->handleButton(controller->BUTTON_A);
					initialsPos++;
				}
		    matrix->fillRect(5+(initialsPos*8), 8, 13+(initialsPos*8), 16, matrix->Color333(0, 0, 0));
                    matrix->setCursor(5+(initialsPos*8), 8);
                    matrix->setTextColor(matrix->Color333(1,5,0));
                    matrix->print(initials[initialsPos]);


                  /*  myGLCD6.setColor(0, 0, 0);
                    myGLCD6.fillRect(100+(initialsPos*20), 60, 120+(initialsPos*20), 100);  // Clear
		    myGLCD6.setColor(245,150,50);
                    myGLCD6.setFont(arial_bold);
                    myGLCD6.print(initials, 100+(initialsPos*20), 60); */
			}
		}
	}else{
		if(controller->buttonPressed(controller->BUTTON_LEFT) && !controller->buttonHandled(controller->BUTTON_LEFT)){
			controller->handleButton(controller->BUTTON_LEFT);
			if(moveDirection==XUP || moveDirection==XDOWN){
				moveDirection=XLEFT;
				moveSnake();
                                tone(8, NOTE_A1, 32);
                                int pauseBetweenNotes = 10;
                                delay(pauseBetweenNotes);
                                noTone(8);
			}
		}
		if(controller->buttonPressed(controller->BUTTON_RIGHT) && !controller->buttonHandled(controller->BUTTON_RIGHT)){
			controller->handleButton(controller->BUTTON_RIGHT);
			if(moveDirection==XUP || moveDirection==XDOWN){
				moveDirection=XRIGHT;
				moveSnake();
                                tone(8, NOTE_A1, 32);
                                int pauseBetweenNotes = 10;
                                delay(pauseBetweenNotes);
                                noTone(8);

			}
		}
		if(controller->buttonPressed(controller->BUTTON_DOWN)){
			controller->handleButton(controller->BUTTON_DOWN);
			if(moveDirection==XLEFT || moveDirection==XRIGHT){
				moveDirection=XDOWN;
				moveSnake();
                                tone(8, NOTE_A1, 32);
                                int pauseBetweenNotes = 10;
                                delay(pauseBetweenNotes);
                                noTone(8);
			}
		}
		if(controller->buttonPressed(controller->BUTTON_UP) && !controller->buttonHandled(controller->BUTTON_UP)){
			controller->handleButton(controller->BUTTON_UP);
			if(moveDirection==XLEFT || moveDirection==XRIGHT){
				moveDirection=XUP;
				moveSnake();
                                tone(8, NOTE_A1, 32);
                                int pauseBetweenNotes = 10;
                                delay(pauseBetweenNotes);
                                noTone(8);
			}
		}
	}
	if(!gameOver && millis() > ((unsigned long)(lastMoveTime+400))){
		moveSnake();
	}
}

void Snake::moveSnake(){
	lastMoveTime = millis();
	int headX=0;
	int headY=0;
        
                  

	/* //Allows looping across the board
	if(moveDirection==XUP){
		headX = (segments->getX()+1)%boardWidth;
		headY = segments->getY();
		segments->setX((segments->getX()+1)%boardWidth);
	}
	if(moveDirection==XDOWN){
		headX = (segments->getX()==0)?(boardWidth-1):(segments->getX()-1);
		headY = segments->getY();
		segments->setX((segments->getX()==0)?(boardWidth-1):(segments->getX()-1));
	}
	if(moveDirection==XLEFT){
		headX = segments->getX();
		headY = (segments->getY()==0)?(boardHeight-1):(segments->getY()-1);
		segments->setY((segments->getY()==0)?(boardHeight-1):(segments->getY()-1));
	}
	if(moveDirection==XRIGHT){
		headX = segments->getX();
		headY = (segments->getY()+1)%boardHeight;
		segments->setY((segments->getY()+1)%boardHeight);
	} */

	if(moveDirection==XUP){
                     
  
		if(segments->getX()>=boardWidth-1){
			setGameOver();
			return;
		}else{
			headX = segments->getX()+1;
			headY = segments->getY();
		}
	}
	if(moveDirection==XDOWN){
		if(segments->getX()==0){
			setGameOver();
			return;
		}else{
			headX = segments->getX()-1;
			headY = segments->getY();
		}
	}
	if(moveDirection==XLEFT){
		if(segments->getY()==0){
			setGameOver();
			return;
		}else{
			headX = segments->getX();
			headY = segments->getY()-1;
		}
	}
	if(moveDirection==XRIGHT){
		if(segments->getY()>=boardHeight-1){
			setGameOver();
			return;
		}else{
			headX = segments->getX();
			headY = segments->getY()+1;
		}
	}

	SnakeSegment * eachSegment = segments;
	while(eachSegment!=NULL){
		if(eachSegment->getNextSegment()!=NULL && eachSegment->getNextSegment()->getNextSegment()==NULL){
			int tailX=0;
			int tailY=0;
			if(growing>0){
				tailX = eachSegment->getNextSegment()->getX();
				tailY = eachSegment->getNextSegment()->getY();
			}
			eachSegment->getNextSegment()->setNextSegment(segments);
			segments = eachSegment->getNextSegment();
			segments->setPosition(headX,headY);
			if(growing>0){
				growing--;
				SnakeSegment * newTail = new SnakeSegment(tailX,tailY,NULL);
				eachSegment->setNextSegment(newTail);
			}else{
				eachSegment->setNextSegment(NULL);
			}
		}
		eachSegment = eachSegment->getNextSegment();
	}

	eachSegment = segments;
	while(eachSegment!=NULL){
		if(eachSegment!=segments && segments->getX()==eachSegment->getX() && segments->getY()==eachSegment->getY()){
			setGameOver();
		}
		eachSegment = eachSegment->getNextSegment();
	}

	if(segments->getX()==foodPosition.getX() && segments->getY()==foodPosition.getY()){
		growing++;
		increaseScore(1);
		addFood();
                 
	}

	redrawBoard();
}

void Snake::setGameOver(){
	gameOver=true;
	if(score<=highScore){
                  matrix->fillScreen(0);
               matrix->setCursor(2, 0);
               matrix->setTextColor(matrix->Color333(7,5,0));
               matrix->print(str51);
               matrix->setCursor(2, 8);
               matrix->setTextColor(matrix->Color333(7,5,0));
               matrix->print(str52);
                  delay(3000);
               matrix->fillScreen(0);
               matrix->setCursor(2, 0);
               matrix->setTextColor(matrix->Color333(1,5,7));
               matrix->print(str53);
               matrix->setCursor(2, 8);
               matrix->setTextColor(matrix->Color333(1,5,0));
               matrix->print(score);
                 /* myGLCD6.setColor(0, 0, 0);
                  myGLCD6.fillRect(1, 110, 318, 224);  // Clear
                  myGLCD6.setColor(245,150,50);
                  myGLCD6.setFont(arial_bold);
                  myGLCD6.print("GAME OVER", CENTER, 60); 
                  myGLCD6.setColor(255,255,0);
                  myGLCD6.printNumI(score, CENTER, 120);*/

	}else{
		 matrix->fillScreen(0);
               matrix->setCursor(6, 1);
               matrix->setTextColor(matrix->Color333(7,5,0));
               matrix->print(str54);
               matrix->setCursor(2, 8);
               matrix->setTextColor(matrix->Color333(4,5,7));
               matrix->print(score);
                  delay(3000);
               matrix->fillScreen(0);
               matrix->setCursor(5, 0);
               matrix->setTextColor(matrix->Color333(1,5,7));
               matrix->print(str55);
               matrix->setCursor(2, 10);
               matrix->setTextColor(matrix->Color333(1,5,0)); 
                /*  myGLCD6.setColor(0, 0, 0);
                  myGLCD6.fillRect(1, 110, 318, 224);  // Clear
                  myGLCD6.setColor(245,150,50);
                  myGLCD6.setFont(arial_bold);
                  myGLCD6.print("RECORD", CENTER, 60); 
                  myGLCD6.setColor(255,255,0);
                  myGLCD6.print("Name: ", 14, 120);*/
                  
		initialsPos=0;
		initials[0] = 'A';
		initials[1] = 'A';
		initials[2] = 'A';
	}
	for (int thisNote = 0; thisNote < 6; thisNote++) {
     int noteDuration6 = 3000/noteDurations6[thisNote];
      tone(8, melody6[thisNote],noteDuration6);
     int pauseBetweenNotes = noteDuration6 * 0.40;
      delay(pauseBetweenNotes);
      noTone(8);     }

}

void Snake::addFood(){
                  int AS3=NOTE_D2;
                  tone(8, AS3, 32);
                  int pauseBetweenNotes = 40;
                  delay(pauseBetweenNotes);
                  noTone(8);
                   int AS4=NOTE_D3;
                  tone(8, AS4, 32);
                  delay(pauseBetweenNotes);
                  noTone(8);                 
	int randX;
	int randY;
	bool positionTaken;
	do{
		randX = random(10);
		randY = random(20);
		positionTaken = false;
		SnakeSegment * eachSegment = segments;
		while(eachSegment!=NULL){
			if(eachSegment->getX()==randX && eachSegment->getY()==randY){
				positionTaken=true;
			}
			eachSegment = eachSegment->getNextSegment();
		}
	}while(positionTaken);
	foodPosition.setPosition(randX,randY);



}

void Snake::redrawBoard(){
	for (int i = 0; i < boardHeight; i++) {
		for (int j = 0; j < boardWidth; j++) {
				strip->setPixelColor(j,i,0x0);
		}
	}
	strip->setPixelColor(foodPosition.getX(),foodPosition.getY(),0xFF0000);
	SnakeSegment * eachSegment = segments;
	while(eachSegment!=NULL){
		strip->setPixelColor(eachSegment->getX(),eachSegment->getY(),0xFFFFFF);
		eachSegment = eachSegment->getNextSegment();
	}
	strip->show();
                  
}

void Snake::increaseScore(unsigned long amount){
	score+=amount;
	updateDisplay();
}

void Snake::resume(){
	redrawBoard();
	updateDisplay();
}

void Snake::updateDisplay(){
       matrix->fillScreen(0);
               matrix->setCursor(1, 0);
               matrix->setTextColor(matrix->Color333(7,5,0));
               matrix->print(str53);
               matrix->setCursor(0, 8);
               matrix->fillRect(0, 8, 32, 16, matrix->Color333(0, 0, 0));
               matrix->setTextColor(matrix->Color333(1,1,7));
               matrix->print(score);
	/*myGLCD6.setColor(0, 0, 0);
        myGLCD6.fillRect(1, 110, 318, 224);  // Clear
        myGLCD6.setColor(245,150,50);
        myGLCD6.setFont(arial_bold);
     // myGLCD6.print("High:", 15, 60); 
     // myGLCD6.printNumI(highScore, 105, 60); 
	myGLCD6.print("High: "+String(highScore)+" @ "+(String(highInitials[0])+String(highInitials[1])+
                    String(highInitials[2])), CENTER, 60); 
        myGLCD6.setColor(255,255,0);
        myGLCD6.print("Score: "+String(score), CENTER, 120);*/

}

void Snake::saveHighScore(){
	highScore = score;
	EEPROM.write(7,(byte)(score>>24));
	EEPROM.write(8,(byte)(score>>16));
	EEPROM.write(9,(byte)(score>>8));
	EEPROM.write(10,(byte) score);
	EEPROM.write(11,(byte) initials[0]);
	EEPROM.write(12,(byte) initials[1]);
	EEPROM.write(13,(byte) initials[2]);
	highInitials[0] = initials[0];
	highInitials[1] = initials[1];
	highInitials[2] = initials[2];
	
	newGame();
}

void Snake::deleteSnake(){
	while(segments!=NULL){
		SnakeSegment * nextSegment = segments->getNextSegment();
		delete segments;
		segments = nextSegment;
	}
}
