#include "spiel_main.h"

// Starte Spiel
void startGame()
{
 String webstring;
 String gamestring;
#ifdef DEBUG_GAME
  Serial.print("Start Game: ");
  Serial.println(webServer.arg("game"));
#endif
  aktgame = webServer.arg("game").toInt();
  if ( aktgame == SNAKE )
    gamestring = F("SNAKE");
  if ( aktgame == TETRIS )
    gamestring = F("TETRIS");
  if ( aktgame == BRICKS )
    gamestring = F("BRICKS");
  if ( aktgame == VIERGEWINNT )
    gamestring = F("VIERGEWINNT");
  if ( aktgame == TIERMEMORY )
    gamestring = F("MEMORY");
  if ( aktgame == MUSIKMEMORY )
    gamestring = F("MEMORY");
  if ( aktgame == ABBAMEMORY )
    gamestring = F("MEMORY");
  aktscore = 0;
  webstring = F("<!doctype html><html><head><script>window.onload=function(){window.location.replace('/web/GameControl.html?game=");
  webstring += gamestring;
  webstring += F("&highscore=");
  webstring += String(highscore[aktgame]);
  #ifdef AUDIO_SOUND
    webstring += F("&sound=") + String(gamesound);
  #else
    webstring += F("&sound=-1");
    gamesound = 0;
  #endif
  if (aktgame == VIERGEWINNT) 
  {
    webstring += F("&level=");
    webstring += String(gamelevel);
  }
  if (aktgame == TIERMEMORY || aktgame == MUSIKMEMORY || aktgame == ABBAMEMORY ) 
  {
    webstring += F("&size=");
    webstring += String(gamesize);
  }
   
  webstring += F("');}</script></head></html>");
  webServer.send(200, "text/html",webstring);
#ifdef AUDIO_SOUND
      AUDIO_FILENR = ANSAGEBASE + 180 + aktgame;
      if ( gamesound < 3 ) Play_MP3(AUDIO_FILENR,false,33*gamesound + 20);
      else Play_MP3(AUDIO_FILENR,false,33*gamesound);
#endif
  curControl= BTN_NONE;  
  handle_Webserver(__LINE__);
  delay(0);
  anzPlayer = 1;
}

void handleGameControl()
{
  bool buttonret;
  String webreturn;
  uint8_t playerret;
  uint8_t playerbuttonadd = 0;
  if ( gameisrunning) playerret = anzPlayer; else playerret = 0;
  
  if ( webServer.arg("button") == "start" ) 
  {

#ifdef DEBUG_GAME
      webreturn += "1#" + String(aktgame) + "#" + String(anzPlayer) + "#" + String(highscore[aktgame]) + "#" + String(aktscore) + "#" + String(debugval);
#else
      webreturn += "1#" + String(aktgame) + "#" + String(anzPlayer) + "#" + String(highscore[aktgame]) + "#" + String(aktscore) + "#0";
#endif 

    webServer.send(200, "text/plain", webreturn); 
    handle_Webserver(__LINE__);
    delay(0);
    ButtonClear();
    setMode(MODE_GAME);
    if ( aktgame == SNAKE       ) runSnake();
    if ( aktgame == TETRIS      ) runTetris();
    if ( aktgame == BRICKS      ) runBricks();
    if ( aktgame == VIERGEWINNT ) runViergewinnt();
    if ( aktgame == TIERMEMORY )  runMemory();
    if ( aktgame == MUSIKMEMORY ) runMemory();
    if ( aktgame == ABBAMEMORY ) runMemory();
    for ( uint8_t pip=0;pip<4;pip++)
    {
      PlayerIP[pip]="";
    }
    handle_Webserver(__LINE__);
    delay(0);
    ButtonClear();
#ifdef DEBUG_GAME
  Serial.println(F("Game over"));
#endif
    gameisrunning = false;
    delay(500);

// sichern der Higshcore Werte
    if (  highscore[SNAKE] > settings.mySettings.highscore[SNAKE] || highscore[TETRIS] > settings.mySettings.highscore[TETRIS] 
       || highscore[BRICKS] > settings.mySettings.highscore[BRICKS] || highscore[VIERGEWINNT] > settings.mySettings.highscore[VIERGEWINNT] ) 
    {
#ifdef DEBUG_GAME
  Serial.println(F("sichere Highscore in EEPROM"));
#endif
      settings.mySettings.highscore[SNAKE] = highscore[SNAKE];
      settings.mySettings.highscore[TETRIS] = highscore[TETRIS];
      settings.mySettings.highscore[BRICKS] = highscore[BRICKS];
      settings.mySettings.highscore[VIERGEWINNT] = highscore[VIERGEWINNT];
      settings.saveToEEPROM();
    }
    delay(500);
    webServer.handleClient();
    if (curControl != BTN_STOP && aktgame != VIERGEWINNT && aktgame != TIERMEMORY && aktgame != MUSIKMEMORY && aktgame != ABBAMEMORY )
    {
      delay(800);
#ifdef AUDIO_SOUND
      AUDIO_FILENR = ANSAGEBASE + 180;
      if (gamesound) Play_MP3(AUDIO_FILENR,false,33*gamesound);
#endif
      delay(200);
      webServer.handleClient();
      uint8_t gor = random(3);
      if ( gor == 0 ) {
#ifdef DEBUG_GAME
  Serial.println(F("Game over Animation"));
#endif
         animation = "GAME_OVER";
         mode = MODE_TIME;
         playanimation = true;
         screenBufferNeedsUpdate = true;
         setMode(MODE_TIME);
      }
      if ( gor == 1 ) {
#ifdef DEBUG_GAME
  Serial.println(F("Game over Pixel"));
#endif
        ledDriver.clear();
        ledDriver.setPixel(10,1,YELLOW,brightness);
        ledDriver.show();
        delay(200);
        ledDriver.setPixel(8,3,YELLOW,brightness);
        ledDriver.show();
        delay(200);
        ledDriver.setPixel(6,5,YELLOW,brightness);
        ledDriver.show();
        delay(200);
        ledDriver.setPixel(4,8,YELLOW,brightness);
        ledDriver.show();
        delay(200);
        webServer.handleClient();
        ledDriver.setPixel(1,3,RED,brightness);
        ledDriver.show();
        delay(200);
        ledDriver.setPixel(4,2,RED,brightness);
        ledDriver.show();
        delay(200);
        ledDriver.setPixel(5,4,RED,brightness);
        ledDriver.show();
        delay(200);
        ledDriver.setPixel(7,2,RED,brightness);
        ledDriver.show();
        delay(1000);
        webServer.handleClient();
        delay(1000);
        webServer.handleClient();
        screenBufferNeedsUpdate = true;
        setMode(MODE_TIME);
      }
      if ( gor == 2 ) {
#ifdef DEBUG_GAME
  Serial.println(F("Game over Feed"));
#endif
          feedColor = MAGENTA;
          feedText = F("  Game Over   ");
          feedPosition = 0;
          renderer.clearScreenBuffer(matrix);
          renderer.clearScreenBuffer(matrixOld);
          setMode(MODE_FEED);
      }
    }
    else
    {
      screenBufferNeedsUpdate = true;
      setMode(MODE_TIME);
    }
    anzPlayer = 1;
#ifdef DEBUG_GAME
  Serial.println(F("zurück zur Uhr..."));
#endif
  }
  else
  {
    if ( webServer.arg(F("button")) == "stop" ) {
      curControl= BTN_STOP;
      buttonret = ButtonIn(BTN_STOP);
    }
    
    if ( webServer.arg(F("button")) == F("sound0") ) gamesound = 0;
    if ( webServer.arg(F("button")) == F("sound1") ) gamesound = 1;
    if ( webServer.arg(F("button")) == F("sound2") ) gamesound = 2;
    if ( webServer.arg(F("button")) == F("sound3") ) gamesound = 3;
    if ( webServer.arg(F("button")) == F("sound1") || webServer.arg(F("button")) == F("sound2") || webServer.arg(F("button")) == F("sound3") )
    {
#ifdef AUDIO_SOUND
      if (gamesound) Play_MP3(700,false,33*gamesound);
#endif
    }
    playerbuttonadd = 0;
    if ( webServer.arg(F("player")) == "2" ) playerbuttonadd = 10;
    if ( webServer.arg(F("player")) == "3" ) playerbuttonadd = 20;
    if ( webServer.arg(F("player")) == "4" ) playerbuttonadd = 30;
    if ( webServer.arg("gb") == F("up") ) buttonret = ButtonIn(BTN_UP + playerbuttonadd);
    if ( webServer.arg("gb") == F("down") ) buttonret = ButtonIn(BTN_DOWN + playerbuttonadd);
    if ( webServer.arg("gb") == F("left") ) buttonret = ButtonIn(BTN_LEFT + playerbuttonadd);
    if ( webServer.arg("gb") == F("right") ) buttonret = ButtonIn(BTN_RIGHT + playerbuttonadd);
    if ( webServer.arg("gb") == F("middle") ) buttonret = ButtonIn(BTN_MIDDLE + playerbuttonadd);

    if ( webServer.arg(F("level")).length() > 0) 
    {
      gamelevel = webServer.arg("level").toInt();
#ifdef DEBUG_GAME
      Serial.printf("GameLevel: %i\n",gamelevel);
#endif      
    }
    
    if ( webServer.arg("size").length() > 0) 
    {
      gamesize = webServer.arg("size").toInt();
#ifdef DEBUG_GAME
      Serial.printf("GameSize: %i\n",gamesize);
#endif      
    }
    
    if ( webServer.arg("button") == "back" )
    {
#ifdef DEBUG_GAME
      webreturn += "0#" + String(aktgame) + "#" + String(anzPlayer) + "#" + String(highscore[aktgame]) + "#" + String(aktscore) + "#" + String(debugval);
#else
      webreturn += "0#" + String(aktgame) + "#" + String(anzPlayer) + "#" + String(highscore[aktgame]) + "#" + String(aktscore) + "#0";
#endif

      webServer.send(200, "text/plain", webreturn); 
      buttonret = ButtonIn(BTN_EXIT);
      curControl= BTN_NONE;
      setMode(MODE_TIME);
      callRoot();
    }
    if ( webServer.arg("poll") == "poll" ) 
    {
       playerret = anzPlayer;
#ifdef DEBUG_WEB
      Serial.println(F("Web Poll"));
      Serial.printf("Debugval: %i\n",debugval);
#endif
    }
//################################################################
    if ( webServer.arg(F("newplayer")) == F("init") ) 
    {  
       if (aktgame == TIERMEMORY || aktgame == MUSIKMEMORY || aktgame == ABBAMEMORY )
       {
         if ( gameisrunning ) 
         {
           playerret = 0; 
           for ( uint8_t pip=0;pip<4;pip++)
           {
             if (PlayerIP[pip] == webServer.client().remoteIP().toString() )  // Spieler mit der gleichen IP bekommen wieder die gleiche Spielernummer/Farbe
             {
               playerret = pip+1;
  #ifdef DEBUG_GAME
               Serial.print(F("Player refresh: "));
               Serial.println(playerret);
               Serial.print(F(" IP: "));
               Serial.println(PlayerIP[pip]);
  #endif
               if ( random(0,2) == 0 ) break;
             }
           }
         }
         else
         {
           if ( anzPlayer < 4 ) 
           {
             PlayerIP[anzPlayer] = webServer.client().remoteIP().toString();
             anzPlayer++;            // Spieler hinzu
             playerret = anzPlayer;
  #ifdef DEBUG_GAME
             Serial.print(F("Neuer Player: "));
             Serial.println(anzPlayer);
             Serial.print(F(" IP: "));
             Serial.println(PlayerIP[anzPlayer-1]);
  #endif
            }
            else
            {
              playerret = 0;          // kein neuer Spieler mehr möglich
            }
          }
       }
       else
       {
         playerret = 0;              // nur bei Mehrspieler Modus ist ein neuer Spieler möglich
       }
    }

    
    if ( gameisrunning ) 
    {
      webreturn = "1#"; 
    }
    else 
    {
      webreturn = "0#";
    }
#ifdef DEBUG_GAME
    webreturn += String(aktgame) + "#" + String(playerret) + "#" + String(highscore[aktgame]) + "#" + String(aktscore)+ "#" + String(debugval);
#else
    webreturn += String(aktgame) + "#" + String(playerret) + "#" + String(highscore[aktgame]) + "#" + String(aktscore) + "#0";
#endif
    webServer.send(200, "text/plain", webreturn);
    
  }
}

//###########################################################################
//##################            IR-Receiver Start          ##################
//###########################################################################
#ifdef IR_RECEIVER_GAME
  // Look for IR commands
void readIRButton()
{
  bool buttonret;
  if (irrecv.decode(&irDecodeResult))
  {
#ifdef DEBUG_IR
    Serial.print(F("IR signal: 0x"));
    serialPrintUint64(irDecodeResult.value, HEX);
    Serial.println();
#endif

    switch (irDecodeResult.value)
    {
      case IR_CODE_LEFT:
        buttonret = ButtonIn(BTN_LEFT);
        break;
      case IR_CODE_RIGHT:
        buttonret = ButtonIn(BTN_RIGHT);
        break;
      case IR_CODE_UP:
        buttonret = ButtonIn(BTN_UP);
        break;
      case IR_CODE_DOWN:
        buttonret = ButtonIn(BTN_DOWN);
        break;
      case IR_CODE_MIDDLE:
        buttonret = ButtonIn(BTN_MIDDLE);
        break;
      case IR_CODE_STOP:
        curControl= BTN_STOP;
        buttonret = ButtonIn(BTN_STOP);
        break;
    }
    irrecv.resume();
  }
}
#endif
//###########################################################################
//##################           IR-Receiver  Ende           ##################
//###########################################################################

// IO-Buffer zum Einlesen der Buttons
#define CURCONTROL_BUFFER_SIZE 8
#define CURCONTROL_BUFFER_MASK (CURCONTROL_BUFFER_SIZE-1)
struct ButtonBuffer {
  uint8_t data[CURCONTROL_BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
} buttonbuffer = {{}, 0, 0};

bool ButtonIn(uint8_t buttonvalue)
{
#ifdef DEBUG_GAME
  Serial.printf("Button write: value: %i, write %i read %i\n", buttonvalue, buttonbuffer.write, buttonbuffer.read);
#endif 
  uint8_t next = ((buttonbuffer.write + 1) & CURCONTROL_BUFFER_MASK);

  if (buttonbuffer.read == next)
    return false; // voll

  buttonbuffer.data[buttonbuffer.write] = buttonvalue;
  // buttonbuffer.data[buttonbuffer.write & CURCONTROL_BUFFER_MASK] = buttonvalue; // absolut Sicher
  buttonbuffer.write = next;

  return true;
}

bool readButton()
{
  if (buttonbuffer.read == buttonbuffer.write)
  {
    curControl = BTN_NONE;
    return false;
  }
  
#ifdef DEBUG_GAME
  Serial.printf("Button read: value: %i, write %i read %i\n", buttonbuffer.data[buttonbuffer.read], buttonbuffer.write, buttonbuffer.read);
#endif 
  curControl = buttonbuffer.data[buttonbuffer.read];

  buttonbuffer.read = (buttonbuffer.read+1) & CURCONTROL_BUFFER_MASK;

  return true;
}

void ButtonClear()
{
  for(uint8_t i=0; i<CURCONTROL_BUFFER_SIZE; i++) {
    buttonbuffer.data[i]=0;
  }
  buttonbuffer.read = 0;
  buttonbuffer.write = 0;
  curControl= BTN_NONE;
}
