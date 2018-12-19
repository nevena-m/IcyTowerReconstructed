#ifndef _GENFUNCS_H_
#define _GENFUNCS_H_

#define ESC 27				/* ASCII kodovi za keyboard input */
#define SPACE 32

/* Redom funkcije:
 * 	special keyboard buttons released
 * 	keyboard input buttons
 * 	special keyboard input buttons
 * 	funkcija koja se poziva non stop kada se nista ne radi
 */

void specialKeyUp(int key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void on_keyboard(unsigned char key, int x, int y);
void special_keys(int key, int x, int y); 
void idle(void);
void calcTime(void); // racuna vreme proteklo izmedju dva frejma
void initKeys(void); // postavlja sve inicijalno na 0 - nema pritisnutih
#endif
