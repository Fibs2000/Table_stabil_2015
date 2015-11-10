#include "Tetris.h"
#include "pitches.h"


char   str21[]   = "GAME";
char   str22[]   = "over";
char   str23[]   = "score";
char   str24[]   = "WIN";
char   str25[]   = "Name";
char   str26[]   = "BEST";
char   str27[]   = "RAINBOW";
char   str28 = EEPROM.read(4);
char   str29 = EEPROM.read(5);
char   str30 = EEPROM.read(6);










// Melodien:

// notes in the melody:
int melody2[] = {
  NOTE_G4, NOTE_A4,NOTE_C4, NOTE_A4, NOTE_C4, NOTE_G4};
int melody3[] = {
  NOTE_A4, NOTE_C4,NOTE_G4};
int melody5[] = {
  NOTE_E3, NOTE_DS3,NOTE_AS3, NOTE_E3, NOTE_DS3,NOTE_AS3 }; 
  
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations2[] = {
  32,32,32,16,16,8 };
  int noteDurations3[] = {
  32,32,32};
  int noteDurations5[] = {
  32,32,32,32,32,32 };

Tetris::Tetris(NESController *controller, Adafruit_WS2801 *strip, RGBmatrixPanel *matrix):Game(controller, strip, matrix){
	
        
      //  myGLCD3.setColor(0, 0, 0);
      //  myGLCD3.fillRect(1, 110, 318, 224);  // Clear
      matrix->begin();
  
  matrix->setTextSize(1);
  matrix->fillScreen(0);   //CLEAR
  
  
                    matrix->fillRect(0, 1, 32, 3, matrix->Color333(2, 0, 0));
                    matrix->fillRect(0, 13, 32, 15, matrix->Color333(2, 0, 0));
                    matrix->setTextColor(matrix->Color333(2, 5, 3));
                    matrix->setCursor(1, 4);
                    matrix->print("T");
                    matrix->setTextColor(matrix->Color333(0, 5, 0));
                    matrix->setCursor(6, 6);
                    matrix->print("e");matrix->setTextColor(matrix->Color333(2, 0, 3));
                    matrix->setCursor(11, 4);
                    matrix->print("t");matrix->setTextColor(matrix->Color333(0, 5, 3));
                    matrix->setCursor(17, 6);
                    matrix->print("r");matrix->setTextColor(matrix->Color333(2, 5, 0));
                    matrix->setCursor(22, 6);
                    matrix->print("i");matrix->setTextColor(matrix->Color333(0, 0, 3));
                    matrix->setCursor(26, 4);
                    matrix->print("s");
                    delay(2000);
                    /*matrix->fillScreen(0);
                  matrix->setCursor(6, 0);
                  matrix->setTextColor(matrix->Color333(7,5,0));
                  matrix->print(str26);
                  matrix->setCursor(2, 8);
                  matrix->setTextColor(matrix->Color333(4,5,7));
                  matrix->print(highScore);
                  delay(2000);*/
                    
 
        
        


	highScore = ((unsigned long)EEPROM.read(0) << 24)+
                    ((unsigned long)EEPROM.read(1) << 16)+
                    ((unsigned long)EEPROM.read(2) << 8)+
                    ((unsigned long)EEPROM.read(3));
	highInitials[0] = EEPROM.read(4);
	highInitials[1] = EEPROM.read(5);
	highInitials[2] = EEPROM.read(6);
	curPiece=0;
	curRotation=0;
	lastDropTime=0;
	clearingRows=false;
	animateClearRowTimer=0;
	initials[0]='A';
	initials[1]='A';
	initials[2]='A';
	initialsPos=0;
	newGame();
/*	//Reset high score
	EEPROM.write(0,0);
	EEPROM.write(1,0);
	EEPROM.write(2,0);
	EEPROM.write(3,1);
	EEPROM.write(4,'A');
	EEPROM.write(5,'A');
	EEPROM.write(6,'A');  */
}

Tetris::~Tetris(){
}

void Tetris::newGame(){
	randomSeed(analogRead(0));
	
	score=0;
	curLevel=0;
	lines=0;
	gameOver=0;
	gameOverAnimation=0;
	for (int i = 0; i < boardWidth; i++) {
		for (int j = 0; j < boardHeight; j++) {
			 board[i][j].setColor(0x0);
		}
	}
	setPiece();
}

void Tetris::run(){
	if(controller->buttonPressed(controller->BUTTON_SELECT) 
            && !controller->buttonHandled(controller->BUTTON_SELECT))
            {
		controller->handleButton(controller->BUTTON_SELECT);
		newGame();
                }
	if(gameOver){
            if(gameOverAnimation<200){
                if(board[gameOverAnimation%boardWidth]
                        [gameOverAnimation/boardWidth].getColor()==0)
                        {
                        board[gameOverAnimation%boardWidth]
                        [gameOverAnimation/boardWidth].setColor
                        (random(0,255)*255*255+random(0,255)*255+random(0,255));
			}
			gameOverAnimation++;
                                    }               
        if(score>highScore){
            if(initialsPos>2)
            {
            saveHighScore();
            }
            else{
            if(controller->buttonPressed(controller->BUTTON_DOWN) &&
                !controller->buttonHandled(controller->BUTTON_DOWN))
                    {
                    controller->handleButton(controller->BUTTON_DOWN);
                    if(initials[initialsPos]>'A')
                    {
                    initials[initialsPos]--;
                    }
                    else {
                    initials[initialsPos]='Z';
                    }
                }
            if(controller->buttonPressed(controller->BUTTON_UP) && 
                !controller->buttonHandled(controller->BUTTON_UP))
                    {
                    controller->handleButton(controller->BUTTON_UP);
                    if(initials[initialsPos]<'Z')
                    {
                    initials[initialsPos]++;
                    }else {
                    initials[initialsPos]='A';
                    }
                }
            if(controller->buttonPressed(controller->BUTTON_A) && 
                !controller->buttonHandled(controller->BUTTON_A))
                    {
                    controller->handleButton(controller->BUTTON_A);
                    initialsPos++;
                    }
                    //lcd->write(157+initialsPos)
                   //lcd->print(initials[initialsPos])
                   //lcd->write(157+initialsPos)
                    
                  /*  myGLCD3.setColor(0, 0, 0);
                    myGLCD3.fillRect(100+(initialsPos*20), 60, 120+(initialsPos*20), 100);  // Clear
		    myGLCD3.setColor(0,0,255);
                    myGLCD3.setFont(arial_bold);
                    myGLCD3.print(initials, 100+(initialsPos*20), 60);*/
                    matrix->fillRect(5+(initialsPos*8), 8, 13+(initialsPos*8), 16, matrix->Color333(0, 0, 0));
                    matrix->setCursor(5+(initialsPos*8), 8);
                                matrix->setTextColor(matrix->Color333(1,5,0));
                                matrix->print(initials[initialsPos]);
                               // matrix->setCursor(12+initialsPos, 5);
                   
                   
                   
                    }
            }
	}
        
        else if(clearingRows){
		if(millis()>animateClearRowTimer){
			int linesCleared = 0;
			for (int i = 0; i < boardHeight; i++) {
				bool clearingRow=true;
				for (int j = 0; j < boardWidth; j++) {
					if(board[j][i].getColor()==0){
						clearingRow=false;
					}
				}
				if(clearingRow){
					shiftPiecesDown(i);
					linesCleared++;
				}
			}
			lines+=linesCleared;
			if(lines>=10){
				curLevel++;
				lines=lines%10;
			}
			increaseScore(100L*linesCleared*linesCleared);
			clearingRows=false;
			setPiece();
                        }
                    }
        else{
            if(controller->buttonPressed(controller->BUTTON_LEFT) && 
                !controller->buttonHandled(controller->BUTTON_LEFT))
                {
			controller->handleButton(controller->BUTTON_LEFT);
			moveLeft();
		}
		if(controller->buttonPressed(controller->BUTTON_RIGHT) && 
                    !controller->buttonHandled(controller->BUTTON_RIGHT))
                    {
			controller->handleButton(controller->BUTTON_RIGHT);
			moveRight();
                    }
		if(controller->buttonPressed(controller->BUTTON_DOWN))
                    {
			controller->handleButton(controller->BUTTON_DOWN);
			increaseScore(1L+curLevel);
			moveDown();
                    }
		if(controller->buttonPressed(controller->BUTTON_A) && 
                    !controller->buttonHandled(controller->BUTTON_A))
                    {
			controller->handleButton(controller->BUTTON_A);
			spinClockwise();
                    }
		if(controller->buttonPressed(controller->BUTTON_B) && 
                    !controller->buttonHandled(controller->BUTTON_B))
                    {
			controller->handleButton(controller->BUTTON_B);
			spinCounterClockwise();
                    }
	}
        
	if(!gameOver && millis() > 
                ((unsigned long)(lastDropTime+levelDropTimes[curLevel])))
                {
		lastDropTime = millis();
		moveDown();
                }
	for (int i = 0; i < boardHeight; i++) 
                {
		bool clearingRow=false;
		if(clearingRows){
			clearingRow=true;
			for (int k = 0; k < boardWidth; k++) {
				if(board[k][i].getColor()==0){
					clearingRow=false;
				}
			}
		}
		for (int j = 0; j < boardWidth; j++) {
			if(clearingRow && 
                            (millis()-animateClearRowTimer)%50>24)
                            {
					strip->setPixelColor(j,i,blinkColor);
                            }       
                        else{
                            strip->setPixelColor(j,i,board[j][i].getColor());
			}
                    }
                }
                
	if(!clearingRows){
		for(int i=0;i<4;i++)
                {
                int xpos = curPos.getX()+
                            ((i!=3)?pieces[curPiece][curRotation][i][0]:0);
                int ypos = curPos.getY()+
                            ((i!=3)?pieces[curPiece][curRotation][i][1]:0);
                strip->setPixelColor(xpos, ypos, pieceColors[curPiece]);
		}
	}
	strip->show();
}

void Tetris::shiftPiecesDown(int row){
	for(int i=row-1;i>=0;i--){
		for (int j=0; j<boardWidth; j++) {
			board[j][i+1].setColor(board[j][i].getColor());
			if(i==0){
				board[j][0].setColor(0);
			}
		}
	}
}

bool Tetris::clearRows(){
	if(clearingRows){
		return true;
	}
	int fullRows = 0;
	for (int i = 0; i < boardHeight; i++) {
		bool fullRow = true;
		for (int j = 0; j < boardWidth; j++) {
			 if(board[j][i].getColor()==0){
				 fullRow = false;
			 }
		}
		if(fullRow){
			fullRows++;
		}
	}

	if(fullRows==4){
  for (int thisNote = 0; thisNote < 6; thisNote++) {
    int noteDuration2 = 3000/noteDurations2[thisNote];
    tone(8, melody2[thisNote],noteDuration2);
    int pauseBetweenNotes = noteDuration2 * 1.70;
    delay(pauseBetweenNotes);
    noTone(8);
  }

		clearingRows=true;
		animateClearRowTimer=millis()+800;
		return true;
	}else if(fullRows>0){
		
for (int thisNote = 0; thisNote < 3; thisNote++) {
    int noteDuration3 = 3000/noteDurations3[thisNote];
    tone(8, melody3[thisNote],noteDuration3);
    int pauseBetweenNotes = noteDuration3 * 1.70;
    delay(pauseBetweenNotes);
    noTone(8);
  }
		clearingRows=true;
		animateClearRowTimer=millis()+800;
		return true;
	}
	return false;
}

void Tetris::spinClockwise(){
	int nextRotation=(curRotation+1)%4;
	bool collision = false;
	for(int i=0; i<4; i++){
		int pixelX = curPos.getX() + 
                            ((i==3)?0:pieces[curPiece][nextRotation][i][0]);
		int pixelY = curPos.getY() + 
                            ((i==3)?0:pieces[curPiece][nextRotation][i][1]);
		if((pixelY>=boardHeight) || pixelX<0 || pixelX>=boardWidth ||   
                        (pixelY>=0 && board[pixelX][pixelY].getColor()!=0) )
                        {
			collision=true;
                        }
                    }
	if(!collision){
		curRotation=nextRotation;
                int AS3=NOTE_AS3;
                tone(8, AS3, 32);
                delay(45);
                noTone(8);

	}
}

void Tetris::spinCounterClockwise(){
	int nextRotation=curRotation-1;
	if(nextRotation<0){
		nextRotation=3;
	}
	bool collision = false;
	for(int i=0; i<4; i++){
		int pixelX = curPos.getX() + 
                        ((i==3)?0:pieces[curPiece][nextRotation][i][0]);
		int pixelY = curPos.getY() + 
                        ((i==3)?0:pieces[curPiece][nextRotation][i][1]);
		if((pixelY>=boardHeight) || pixelX<0 || pixelX>=boardWidth ||   
                        (pixelY>=0 && board[pixelX][pixelY].getColor()!=0) )
                        {
			collision=true;
                        }
                }
	if(!collision){
		curRotation=nextRotation;
                int AS3= NOTE_AS3;
                tone(8, AS3,32);
                delay(45);
                noTone(8);
		
                }
    }

void Tetris::moveLeft(){
	bool collision = false;
	for(int i=0; i<4; i++){
		int pixelX = curPos.getX() + 
                            ((i==3)?0:pieces[curPiece][curRotation][i][0]);
		int pixelY = curPos.getY() + 
                            ((i==3)?0:pieces[curPiece][curRotation][i][1]);
		if((pixelY<0 && pixelX<=0) || ((pixelY>=0)&& 
                    (pixelX<=0 || board[pixelX-1][pixelY].getColor()!=0)))
                        {
			collision=true;
                        }
                }
	if(!collision){
		curPos.setX(curPos.getX()-1);
		int D3=NOTE_D3;
                tone(8, D3,32);
                delay(45);
                noTone(8);
	}
}

void Tetris::moveRight(){
	bool collision = false;
	for(int i=0; i<4; i++){
		int pixelX = curPos.getX() + 
                        ((i==3)?0:pieces[curPiece][curRotation][i][0]);
		int pixelY = curPos.getY() + 
                        ((i==3)?0:pieces[curPiece][curRotation][i][1]);
		if((pixelY<0 && pixelX>=(boardWidth-1)) || ((pixelY>=0)&& 
                        (pixelX>=(boardWidth-1) || 
                                board[pixelX+1][pixelY].getColor()!=0)))
                                {
                                collision=true;
                            }
	}
	if(!collision){
		curPos.setX(curPos.getX()+1);
		int D3=NOTE_D3;
                tone(8, D3,32);
                delay(45);
                noTone(8);
	}
}

void Tetris::moveDown(){
	bool hitBottom=false;
	for(int i=0; i<4; i++){
		int pixelX = curPos.getX() + 
                        ((i==3)?0:pieces[curPiece][curRotation][i][0]);
		int pixelY = curPos.getY() + 
                        ((i==3)?0:pieces[curPiece][curRotation][i][1]);
		if(pixelY>=boardHeight-1 || 
                            board[pixelX][pixelY+1].getColor()!=0){
			hitBottom=true;
		}
	}
	if(!hitBottom){
		curPos.setY(curPos.getY()+1);
                

	}else
        {
        for(int i=0; i<4; i++){
            int pixelX = curPos.getX() + 
                ((i==3)?0:pieces[curPiece][curRotation][i][0]);
            int pixelY = curPos.getY() + 
                ((i==3)?0:pieces[curPiece][curRotation][i][1]);
            if(pixelY>=0){
                if(!gameOver && board[pixelX][pixelY].getColor()!=0 
                            && curPos.getY()==0)
                {
                gameOver=true;
                if(score<=highScore){
                  
                 /* myGLCD3.setColor(0, 0, 0);
                  myGLCD3.fillRect(1, 110, 318, 224);  // Clear
                  myGLCD3.setColor(245,150,50);
                  myGLCD3.setFont(arial_bold);
                  myGLCD3.print("GAME OVER", CENTER, 60); 
                  myGLCD3.setColor(255,255,0);
                  myGLCD3.printNumI(score, CENTER, 120);*/
                   matrix->fillScreen(0);
                  matrix->setCursor(2, 0);
                  matrix->setTextColor(matrix->Color333(7,5,0));
                  matrix->print(str21);
                  matrix->setCursor(2, 8);
                  matrix->setTextColor(matrix->Color333(7,5,0));
                  matrix->print(str22);
                  delay(3000);
                  matrix->fillScreen(0);
                  matrix->setCursor(2, 0);
                  matrix->setTextColor(matrix->Color333(1,5,7));
                  matrix->print(str23);
                  matrix->setCursor(2, 8);
                  matrix->setTextColor(matrix->Color333(1,5,0));
                  matrix->print(score);

                  
                  
                    
                    }
                else{
                  
                 /* myGLCD3.setColor(0, 0, 0);
                  myGLCD3.fillRect(1, 110, 318, 224);  // Clear
                  myGLCD3.setColor(245,150,50);
                  myGLCD3.setFont(arial_bold);
                  myGLCD3.print("RECORD", CENTER, 60); 
                  myGLCD3.setColor(255,255,0);
                  myGLCD3.print("Name: ", 14, 120);*/
                   matrix->fillScreen(0);
                  matrix->setCursor(6, 1);
                  matrix->setTextColor(matrix->Color333(7,5,0));
                  matrix->print(str24);
                  matrix->setCursor(2, 8);
                  matrix->setTextColor(matrix->Color333(4,5,7));
                  matrix->print(score);
                  delay(3000);
                  matrix->fillScreen(0);
                  matrix->setCursor(5, 0);
                  matrix->setTextColor(matrix->Color333(1,5,7));
                  matrix->print(str25);
                  matrix->setCursor(2, 10);
                  matrix->setTextColor(matrix->Color333(1,5,0));
                  
                  
                  
                    
                    }
					initialsPos=0;
					initials[0] = 'A';
					initials[1] = 'A';
					initials[2] = 'A';

	for (int thisNote = 0; thisNote < 6; thisNote++) {
     int noteDuration5 = 3000/noteDurations5[thisNote];
      tone(8, melody5[thisNote],noteDuration5);
     int pauseBetweenNotes = noteDuration5 * 1.70;
      delay(pauseBetweenNotes);
      noTone(8);     }

                }
                
            board[pixelX][pixelY].setColor(pieceColors[curPiece]);
                }
        }
		if(!clearRows()){
			setPiece();
                  int Note_A3;
                  tone(8, Note_A3, 8);
                  int pauseBetweenNotes = 8 * 1.70;
                  delay(pauseBetweenNotes);
                  noTone(8);
		}
	}
}

void Tetris::setPiece(){
	if(!gameOver){
		curPos.setPosition(4,0);
		lastDropTime = millis();
		random(7);
		curPiece=random(7);
		curRotation=0;
		updateDisplay();
	}
}

void Tetris::increaseScore(unsigned long amount){
	score+=amount;
	updateDisplay();
}

void Tetris::resume(){
	
	updateDisplay();
}

void Tetris::updateDisplay(){
  
   matrix->fillScreen(0);
                  matrix->setCursor(1, 0);
                  matrix->setTextColor(matrix->Color333(7,5,0));
                  matrix->print(str23);
                  matrix->setCursor(0, 8);
                  matrix->fillRect(0, 8, 32, 16, matrix->Color333(0, 0, 0));
                  matrix->setTextColor(matrix->Color333(1,1,7));
                  matrix->print(score);
 
  //matrix->swapBuffers(false);
  /* 
myGLCD3.setColor(0, 0, 0);
myGLCD3.fillRect(1, 110, 318, 224);  // Clear
myGLCD3.setColor(245,150,50);
myGLCD3.setFont(arial_bold);
myGLCD3.print("High: "+String(highScore)+" @ "+(String(highInitials[0])+String(highInitials[1])+
                    String(highInitials[2])), CENTER, 60); 
myGLCD3.setColor(255,255,0);
myGLCD3.print("Score:"+String(score), CENTER, 120);*/

}

void Tetris::saveHighScore(){
	highScore = score;
	EEPROM.write(0,(byte)(score>>24));
	EEPROM.write(1,(byte)(score>>16));
	EEPROM.write(2,(byte)(score>>8));
	EEPROM.write(3,(byte) score);
	EEPROM.write(4,(byte) initials[0]);
	EEPROM.write(5,(byte) initials[1]);
	EEPROM.write(6,(byte) initials[2]);
	highInitials[0] = initials[0];
	highInitials[1] = initials[1];
	highInitials[2] = initials[2];
	
	newGame();
}

//Piece,rotation,pixel,position
const int Tetris::pieces[7][4][3][2] = {
	{{{-1,0},{1,0},{2,0}}, {{0,-1},{0,1},{0,2}}, 
            {{-1,0},{1,0},{2,0}}, {{0,-1},{0,1},{0,2}}},
	{{{-1,0},{1,0},{0,1}}, {{-1,0},{0,-1},{0,1}}, 
            {{-1,0},{0,-1},{1,0}}, {{0,-1},{1,0},{0,1}}},
	{{{-1,0},{-1,1},{0,1}}, {{-1,0},{-1,1},{0,1}}, 
            {{-1,0},{-1,1},{0,1}}, {{-1,0},{-1,1},{0,1}}},
	{{{1,0},{-1,1},{0,1}}, {{0,-1},{1,0},{1,1}}, 
            {{1,0},{-1,1},{0,1}}, {{0,-1},{1,0},{1,1}}},
	{{{-1,0},{0,1},{1,1}}, {{1,-1},{1,0},{0,1}}, 
            {{-1,0},{0,1},{1,1}}, {{1,-1},{1,0},{0,1}}},
	{{{-1,0},{1,0},{-1,1}}, {{-1,-1},{0,-1},{0,1}}, 
            {{-1,0},{1,0},{1,-1}}, {{0,-1},{0,1},{1,1}}},
	{{{-1,0},{1,0},{1,1}}, {{-1,1},{0,1},{0,-1}}, 
            {{-1,-1},{-1,0},{1,0}}, {{0,1},{0,-1},{1,-1}}}
};
const uint32_t Tetris::pieceColors[7] = 
    {0xFF0000,0x00FF00,0x0000FF,0xFFFF00,0x66FFCC,0xFF00FF,0xFF7800};
const int Tetris::levelDropTimes[24] = 
{1000,800,700,550,400,350,300,250,200,1000,140,
        100,75,63,50,40,30,20,10,5,1,1,1,1};
    //{2000,1700,1400,1150,900,750,600,450,300,200,140,
     //   100,75,63,50,40,30,20,10,5,1,1,1,1};
const uint32_t Tetris::blinkColor = 0x990000;
