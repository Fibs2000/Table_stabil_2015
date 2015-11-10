#include "Rainbow.h"

 ///***********************  MATRIX

#define OE  9

char   str11[]   = "PRG";
char   str12[]   = "Mode";

// **********************   MATRIX end

               
                 

int r_color=0;
int g_color=0;
int b_color=255;
int r_background=120;
int g_background=80;
int b_background=0;



Rainbow::Rainbow(NESController *controller, Adafruit_WS2801 *strip, RGBmatrixPanel *matrix):Game(controller, strip, matrix){
	resume();
	red=128;
	green=255;
	blue=0;
	colorMode=1;
        geschw=10;
       matrix->begin();
       matrix->setTextSize(1);
       matrix->fillScreen(0);
       matrix->setCursor(1, 1);
                  matrix->setTextColor(matrix->Color333(2,3,0));
                  matrix->print(str11);
                  matrix->setCursor(2, 9);
                  matrix->print(str12);
                  matrix->setCursor(20, 1);
                  matrix->setTextColor(matrix->Color333(1,1,3));
                  matrix->print(geschw);
                  matrix->setCursor(27, 9);
                  matrix->setTextColor(matrix->Color333(1,1,3));
                  matrix->print(colorMode);
}

Rainbow::~Rainbow(){
   
}

void Rainbow::run(){
        if(controller->buttonPressed(controller->BUTTON_UP) && 
                    !controller->buttonHandled(controller->BUTTON_UP))
                    {
			controller->handleButton(controller->BUTTON_UP);
			geschw++;
                        if(geschw>50){geschw=1;}
                        
                        matrix->setCursor(20, 1);
                        matrix->fillRect(20, 1, 32, 8, matrix->Color333(0, 0, 0));
                        matrix->setTextColor(matrix->Color333(1,1,4));
                        matrix->print(geschw);


                    }
		if(controller->buttonPressed(controller->BUTTON_DOWN) && 
                    !controller->buttonHandled(controller->BUTTON_DOWN))
                    {
			controller->handleButton(controller->BUTTON_DOWN);
			geschw--;
                        if(geschw<1){geschw=50;}
                        
                        matrix->setCursor(20, 1);
                        matrix->fillRect(20, 1, 32, 8, matrix->Color333(0, 0, 0));
                        matrix->setTextColor(matrix->Color333(1,1,4));
                        matrix->print(geschw);
                        
                    }
        
        if(controller->buttonPressed(controller->BUTTON_A) && 
                    !controller->buttonHandled(controller->BUTTON_A))
                    {
			controller->handleButton(controller->BUTTON_A);
			TIMSK1 &= ~_BV(TOIE1); // Disable Timer1 interrupt
                        digitalWrite(OE, HIGH); // Disable LED output
                    }
		if(controller->buttonPressed(controller->BUTTON_B) && 
                    !controller->buttonHandled(controller->BUTTON_B))
                    {
			controller->handleButton(controller->BUTTON_B);
			TIMSK1 |= _BV(TOIE1); // Enable Timer1 interrupt
                    }
  
  
	if(controller->buttonPressed(controller->BUTTON_LEFT) && !controller->buttonHandled(controller->BUTTON_LEFT)){
		controller->handleButton(controller->BUTTON_LEFT);
		colorMode--;
		if(colorMode<1){
			colorMode=6;}

                  matrix->setCursor(27, 9);
                  matrix->fillRect(27, 9, 32, 16, matrix->Color333(0, 0, 0));
                  matrix->setTextColor(matrix->Color333(1,1,4));
                  matrix->print(colorMode);


		
	}
	if(controller->buttonPressed(controller->BUTTON_RIGHT) && !controller->buttonHandled(controller->BUTTON_RIGHT)){
		controller->handleButton(controller->BUTTON_RIGHT);
		colorMode++;
		if(colorMode>6){
			colorMode=1;}

                  matrix->setCursor(27, 9);
                  matrix->fillRect(27, 9, 32, 16, matrix->Color333(0, 0, 0));
                  matrix->setTextColor(matrix->Color333(1,1,4));
                  matrix->print(colorMode);
		
	}
	if(colorMode==1){
		if(blue==255 && green==0 && red<255){
			red = min(255,red+1);
		}else if(green==255 && red>0 && blue==0){
			red = max(0,red-1);
		}else if(red==255 && blue==0 && green<255){
			green = min(255,green+1);
		}else if(blue==255 && green>0 && red==0){
			green = max(0,green-1);
		}else if(green==255 && red==0 && blue<255){
			blue = min(255,blue+1);
		}else if(red==255 && blue>0 && green==0){
			blue = max(0,blue-1);
		}
		for(int i=0;i<20;i++){
			for(int j=0;j<10;j++){
				uint32_t pixelColor = ((uint32_t)red)*0x10000+((uint32_t)green)*0x100+((uint32_t)blue)*0x1;
				strip->setPixelColor(j,i,pixelColor);
			}
		}
	}else if(colorMode==2){
          for(int i=0;i<5;i++){
                board[i][0].setColor(0x009900);
		board[i][7].setColor(0x222222);
		board[i][9].setColor(0x129912);
          }
                
          for (int i = 0; i < boardHeight; i++) {
		for (int j = 0; j < boardWidth; j++) {
			strip->setPixelColor(j,i,board[j][i].getColor());
		}
	}
      

  
		
	}else if(colorMode==3){
		if(blue==255 && green==0 && red<255){
			red = min(255,red+15);
		}else if(green==255 && red>0 && blue==0){
			red = max(0,red-15);
		}else if(red==255 && blue==0 && green<255){
			green = min(255,green+15);
		}else if(blue==255 && green>0 && red==0){
			green = max(0,green-15);
		}else if(green==255 && red==0 && blue<255){
			blue = min(255,blue+15);
		}else if(red==255 && blue>0 && green==0){
			blue = max(0,blue-15);
		}
		int redTemp = red;
		int greenTemp = green;
		int blueTemp = blue;
		for(int i=0;i<20;i++){
			for(int j=0;j<10;j++){
				if(blueTemp==255 && greenTemp==0 && redTemp<255){
					redTemp = min(255,redTemp+15);
				}else if(greenTemp==255 && redTemp>0 && blueTemp==0){
					redTemp = max(0,redTemp-15);
				}else if(redTemp==255 && blueTemp==0 && greenTemp<255){
					greenTemp = min(255,greenTemp+15);
				}else if(blueTemp==255 && greenTemp>0 && redTemp==0){
					greenTemp = max(0,greenTemp-15);
				}else if(greenTemp==255 && redTemp==0 && blueTemp<255){
					blueTemp = min(255,blueTemp+15);
				}else if(redTemp==255 && blueTemp>0 && greenTemp==0){
					blueTemp = max(0,blueTemp-15);
				}
				uint32_t pixelColor = ((uint32_t)redTemp)*0x10000+((uint32_t)greenTemp)*0x100+((uint32_t)blueTemp);
				strip->setPixelColor(j,i,pixelColor);
			}
		}
	}
        else if(colorMode==4){
		if(blue==255 && green==0 && red<255){
			red = min(255,red+2);
		}else if(green==255 && red>0 && blue==0){
			red = max(0,red-2);
		}else if(red==255 && blue==0 && green<255){
			green = min(255,green+2);
		}else if(blue==255 && green>0 && red==0){
			green = max(0,green-2);
		}else if(green==255 && red==0 && blue<255){
			blue = min(255,blue+2);
		}else if(red==255 && blue>0 && green==0){
			blue = max(0,blue-2);
		}
		int redTemp = red;
		int greenTemp = green;
		int blueTemp = blue;
		for(int i=0;i<20;i++){
			for(int j=0;j<10;j++){
				if(blueTemp==255 && greenTemp==0 && redTemp<255){
					redTemp = min(255,redTemp+2);
				}else if(greenTemp==255 && redTemp>0 && blueTemp==0){
					redTemp = max(0,redTemp-2);
				}else if(redTemp==255 && blueTemp==0 && greenTemp<255){
					greenTemp = min(255,greenTemp+2);
				}else if(blueTemp==255 && greenTemp>0 && redTemp==0){
					greenTemp = max(0,greenTemp-2);
				}else if(greenTemp==255 && redTemp==0 && blueTemp<255){
					blueTemp = min(255,blueTemp+2);
				}else if(redTemp==255 && blueTemp>0 && greenTemp==0){
					blueTemp = max(0,blueTemp-2);
				}
				uint32_t pixelColor = ((uint32_t)redTemp)*0x10000+((uint32_t)greenTemp)*0x100+((uint32_t)blueTemp);
				strip->setPixelColor(j,i,pixelColor);
			}
		}}
          else if(colorMode==5){
		if(blue==255 && green==0 && red<255){
			red = min(255,red+15);
		}else if(green==255 && red>0 && blue==0){
			red = max(0,red-15);
		}else if(red==255 && blue==0 && green<255){
			green = min(255,green+15);
		}else if(blue==255 && green>0 && red==0){
			green = max(0,green-15);
		}else if(green==255 && red==0 && blue<255){
			blue = min(255,blue+15);
		}else if(red==255 && blue>0 && green==0){
			blue = max(0,blue-15);
		}
		int redTemp = red;
		int greenTemp = green;
		int blueTemp = blue;


		int i=random(20);
		int j=random(10);
				if(blueTemp==255 && greenTemp==0 && redTemp<255){
					redTemp = min(255,redTemp+15);
				}else if(greenTemp==255 && redTemp>0 && blueTemp==0){
					redTemp = max(0,redTemp-15);
				}else if(redTemp==255 && blueTemp==0 && greenTemp<255){
					greenTemp = min(255,greenTemp+15);
				}else if(blueTemp==255 && greenTemp>0 && redTemp==0){
					greenTemp = max(0,greenTemp-15);
				}else if(greenTemp==255 && redTemp==0 && blueTemp<255){
					blueTemp = min(255,blueTemp+15);
				}else if(redTemp==255 && blueTemp>0 && greenTemp==0){
					blueTemp = max(0,blueTemp-15);
				}
				uint32_t pixelColor = ((uint32_t)redTemp)*0x10000+((uint32_t)greenTemp)*0x100+((uint32_t)blueTemp);
				strip->setPixelColor(j,i,pixelColor);

			}else if(colorMode==6){
  
  
                
  
		if(blue==255 && green==0 && red<255){
			red = min(255,red+geschw);
		}else if(green==255 && red>0 && blue==0){
			red = max(0,red-geschw);
		}else if(red==255 && blue==0 && green<255){
			green = min(255,green+geschw);
		}else if(blue==255 && green>0 && red==0){
			green = max(0,green-geschw);
		}else if(green==255 && red==0 && blue<255){
			blue = min(255,blue+geschw);
		}else if(red==255 && blue>0 && green==0){
			blue = max(0,blue-geschw);
		}
		int redTemp = red;
		int greenTemp = green;
		int blueTemp = blue;
		for(int i=0;i<20;i++){
			for(int j=0;j<10;j++){
				if(blueTemp==255 && greenTemp==0 && redTemp<255){
					redTemp = min(255,redTemp+geschw);
				}else if(greenTemp==255 && redTemp>0 && blueTemp==0){
					redTemp = max(0,redTemp-geschw);
				}else if(redTemp==255 && blueTemp==0 && greenTemp<255){
					greenTemp = min(255,greenTemp+geschw);
				}else if(blueTemp==255 && greenTemp>0 && redTemp==0){
					greenTemp = max(0,greenTemp-geschw);
				}else if(greenTemp==255 && redTemp==0 && blueTemp<255){
					blueTemp = min(255,blueTemp+geschw);
				}else if(redTemp==255 && blueTemp>0 && greenTemp==0){
					blueTemp = max(0,blueTemp-geschw);
				}
				uint32_t pixelColor = ((uint32_t)redTemp)*0x10000+((uint32_t)greenTemp)*0x100+((uint32_t)blueTemp);
				strip->setPixelColor(j,i,pixelColor);
			}
		}
	}
		


	strip->show();
}

void Rainbow::resume(){
	
                


}








