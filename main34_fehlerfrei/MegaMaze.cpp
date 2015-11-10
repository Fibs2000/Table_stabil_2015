#include "MegaMaze.h"
#include "pitches.h"


MegaMaze::MegaMaze(NESController *controller, Adafruit_WS2801 *strip, RGBmatrixPanel *matrix):Game(controller, strip, matrix){
	
	newGame();
}

MegaMaze::~MegaMaze(){
}

void MegaMaze::newGame(){
	xpos=0;
	ypos=0;
	level=0;
	updateLevelDisplay();
	updateDisplay();
}

void MegaMaze::run(){
	if(controller->buttonPressed(controller->BUTTON_SELECT) && !controller->buttonHandled(controller->BUTTON_SELECT)){
		controller->handleButton(controller->BUTTON_SELECT);
		newGame();
	}
	if(controller->buttonPressed(controller->BUTTON_LEFT) && !controller->buttonHandled(controller->BUTTON_LEFT)){
		//controller->handleButton(controller->BUTTON_LEFT);
		moveCheck(0,1);
		delay(45);
	}
	if(controller->buttonPressed(controller->BUTTON_RIGHT) && !controller->buttonHandled(controller->BUTTON_RIGHT)){
		//controller->handleButton(controller->BUTTON_RIGHT);
		moveCheck(0,-1);
		delay(45);
	}
	if(controller->buttonPressed(controller->BUTTON_DOWN) && !controller->buttonHandled(controller->BUTTON_DOWN)){
		//controller->handleButton(controller->BUTTON_DOWN);
		moveCheck(1,0);
		delay(45);
	}
	if(controller->buttonPressed(controller->BUTTON_UP) && !controller->buttonHandled(controller->BUTTON_UP)){
		//controller->handleButton(controller->BUTTON_UP);
		moveCheck(-1,0);
		delay(45);
	}
	updateDisplay();
}

void MegaMaze::updateDisplay(){
	for (int i = 0; i < boardHeight; i++) {
		for (int j = 0; j < boardWidth; j++) {
			strip->setPixelColor(j,i,0);
		}
	}
	strip->setPixelColor(centerX,centerY,0x0000FF);
	for(int i = 0; i < getLevelSize();i++){
		int colorPosX = getX(i)+centerX+xpos;
		int colorPosY = getY(i)+centerY+ypos;
		if(colorPosX>=0 && colorPosX<boardWidth && colorPosY>=0 && colorPosY<boardHeight){
			strip->setPixelColor(colorPosX,colorPosY,((i==0)?0x00FF00:levelColors[level]));
		}
	}
	strip->show();
}

void MegaMaze::updateLevelDisplay(){
	
      /*  myGLCD5.setColor(0, 0, 0);
        myGLCD5.fillRect(1, 110, 318, 224);  // Clear
        myGLCD5.setColor(245,150,50);
        myGLCD5.setFont(arial_bold);
        myGLCD5.print("Mega Maze", CENTER, 60); 
        myGLCD5.setColor(255,255,0);
        myGLCD5.print("Level: "+(level+1), CENTER, 120);

	*/



}

void MegaMaze::resume(){
	updateLevelDisplay();
	updateDisplay();
}

bool MegaMaze::moveCheck(int xmove, int ymove){
	bool collision=false;
	for(int i = 0; i < getLevelSize();i++){
		if(getX(i)==-(xpos+xmove) && getY(i)==-(ypos+ymove)){
			if(i==0){
				winLevel();
				return true;
			}else{
				int Note_A4;
                                tone(8, Note_A4, 16);
                                int pauseBetweenNotes = 16 * 1.70;
                                delay(pauseBetweenNotes);
                                noTone(8);

				collision = true;
			}
		}
	}
	if(!collision){
		xpos+=xmove;
		ypos+=ymove;
	}
	return true;
}

void MegaMaze::winLevel(){
	xpos=0;
	ypos=0;
	level++;
	if(level<levelCount-1){
		updateLevelDisplay();
	}else{
		/*myGLCD5.InitLCD();
                myGLCD5.setColor(0, 0, 0);
                myGLCD5.fillRect(1, 110, 318, 224);  // Clear
                myGLCD5.setColor(245,150,50);
                myGLCD5.setFont(arial_bold);
                myGLCD5.print("GEWONNEN!", CENTER, 60); 
                myGLCD5.setColor(255,255,0);
                myGLCD5.print("Bring Michel ein Bier!", CENTER, 120);
                */
	}
int Note_C4;
                  tone(8, Note_C4, 16);
                  int pauseBetweenNotes = 16 * 1.70;
                  delay(pauseBetweenNotes);
                  noTone(8);

                  tone(8, Note_C4, 16);
                  int pauseBetweenNotes3 = 16 * 1.70;
                  delay(pauseBetweenNotes3);
                  noTone(8);
int Note_C6;
                  tone(8, Note_C6, 4);
                  int pauseBetweenNotes2 = 4 * 1.70;
                  delay(pauseBetweenNotes2);
                  noTone(8);

}

int MegaMaze::getX(int pos){
	int curpos=0;
	for(int i=0;i<level;i++){
		curpos+=levelSizes[i];
	}
	return levels[(curpos+pos)*2];
}

int MegaMaze::getY(int pos){
	int curpos=0;
	for(int i=0;i<level;i++){
		curpos+=levelSizes[i];
	}
	return levels[(curpos+pos)*2+1];
}

int MegaMaze::getLevelSize(){
	return levelSizes[level];
}

const int MegaMaze::centerX = 5;
const int MegaMaze::centerY = 9;
const int MegaMaze::levelCount = 5;
const int MegaMaze::levels[] = {
	3,-3, 4,1, 3,1, 2,1, 1,1, 0,1, -1,1, 4,0, -1,0, 4,-1,
		2,-1, 1,-1, 0,-1, -1,-1, 4,-2, 2,-2, 4,-3, 2,-3, 4,-4, 3,-4,
		2,-4,
        

	-4,9, 4,-9, 3,-9, 2,-9, 1,-9, 0,-9, -1,-9, -2,-9, -3,-9, -4,-9,
		-5,-9, 4,-8, -5,-8, 4,-7, 2,-7, 0,-7, -1,-7, -2,-7, -3,-7, -5,-7,
		4,-6, 2,-6, 0,-6, -3,-6, -5,-6, 4,-5, 2,-5, 0,-5, -2,-5, -3,-5,
		-4,-5, -5,-5, 4,-4, 2,-4, -5,-4, 4,-3, 2,-3, 1,-3, 0,-3, -1,-3,
		-2,-3, -3,-3, -5,-3, 4,-2, 2,-2, -1,-2, -5,-2, 4,-1, 2,-1, 1,-1,
		-1,-1, -3,-1, -4,-1, -5,-1, 4,0, -1,0, -5,0, 4,1, 2,1, 1,1,
		0,1, -1,1, -2,1, -3,1, -5,1, 4,2, 2,2, -2,2, -5,2, 4,3,
		2,3, 0,3, -2,3, -4,3, -5,3, 4,4, 2,4, 0,4, -2,4, -5,4,
		4,5, 2,5, 0,5, -2,5, -3,5, -5,5, 4,6, 2,6, 0,6, -1,6,
		-2,6, -5,6, 4,7, 2,7, 0,7, -4,7, -5,7, 4,8, 2,8, 0,8,
		-2,8, -3,8, -4,8, -5,8, 4,9, 0,9, -5,9, 4,10, 3,10, 2,10,
		1,10, 0,10, -1,10, -2,10, -3,10, -4,10, -5,10,
	
	3,0, 6,-8, 5,-8, 4,-8, 3,-8, 2,-8, 1,-8, 0,-8, -1,-8, -2,-8,
		-3,-8, -4,-8, 6,-7, -4,-7, 6,-6, 0,-6, -1,-6, -2,-6, -4,-6, 6,-5,
		4,-5, 3,-5, -2,-5, -4,-5, 6,-4, 3,-4, 2,-4, -1,-4, -2,-4, -4,-4,
		6,-3, 4,-3, 3,-3, -2,-3, -4,-3, 6,-2, 0,-2, -1,-2, -2,-2, -4,-2,
		6,-1, 4,-1, 3,-1, 2,-1, -4,-1, 6,0, 4,0, 2,0, -1,0, -2,0,
		-4,0, 6,1, 4,1, 3,1, 2,1, -4,1, 6,2, 0,2, -1,2, -2,2,
		-4,2, 6,3, 4,3, 3,3, 2,3, 0,3, -4,3, 6,4, 2,4, -1,4,
		-4,4, 6,5, 4,5, 3,5, 2,5, -2,5, -4,5, 6,6, 0,6, -1,6,
		-2,6, -4,6, 6,7, -4,7, 6,8, 5,8, 4,8, 3,8, 2,8, 1,8,
		0,8, -1,8, -2,8, -3,8, -4,8
};
const int MegaMaze::levelSizes[]={21,117,95};
const long MegaMaze::levelColors[]={0xFFFFFF,0xFF34FF,0xFFf4cc,0xFF1111,0xFFFF00};

