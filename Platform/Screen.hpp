#include <windows.h>

// Imposta il template del gioco
class Screen {
	protected:
		HANDLE console;

		/*
			Prende in input un intero (es. FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE)
			Imposta il colore dei caratteri successivi in quello indicato come parametro
		*/
		void setColor(int color);

		/*
			Imposta il colore dei caratteri successivi a quello di default della console
		*/
		void resetColor();

		/*
			Prende in input due interi che rappresentano le coordinate
			Muove il cursore a tali coordinate
		*/
		void moveCursor(int x, int y);

	public:
		Screen();

		/*
			Inizializza il template del gioco
		*/
		void init();
};