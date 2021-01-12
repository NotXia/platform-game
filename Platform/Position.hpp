#pragma once
class Position {
	protected:
		int x;
		int y;

	public:
		Position(int x=0, int y=0);

		/*
			Imposta x (rispettando i limiti)
		*/
		void setX(int x);

		/*
			Imposta y (rispettando i limiti)
		*/
		void setY(int y);

		int getX();
		int getY();

		/*
			Prende in input un oggetto Position
			Restituisce true se il parametro è uguale all'oggetto su cui è chiamato
		*/
		bool equals(Position p);
};
