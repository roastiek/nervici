#ifndef __NERVICI_H__
#define __NERVICI_H__

void clearPlayerground ();
void clearStatus ();
void endGame ();
int findFreeStart ();
int getSpeed ();
int livePlsCount ();
void nextRound ();
void playMusic (int type);
int getRound ();
void setSemafor (int state);
void setSpeed (int speed);
void setTimer (int time);
void stopMusic ();
void gameWait (int time);
void waitForSpace ();

void clearPl (int plid);
void cutPlAtLength (int plid, int length);
void decPlMaxLength (int plid, unsigned int delta);
void decPlScore (int plid, int delta);
void fastClearPl (int plid);
int getPlLength (int plid);
int getPlMaxLength (int plid);
int getPlScore (int plid);
void givePlStart (int plid, int start);
void incPlMaxLength (int plid, unsigned int delta);
void incPlScore (int plid, int delta);
int isPlHuman (int plid);
int isPlJumping (int plid);
int isPlLive (int plid);
void killPl (int plid);
void setPlMaxLenght (int plid, unsigned int length);
void setPlScore (int plid, int score);
void setPlTimer (int plid, int time);
void startPl (int plid);


#endif // __NERVICI_H__
