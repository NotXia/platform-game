#pragma once
class Position {
	protected:
		int x;
		int y;

	public:
		Position(int x=1, int y=1);

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
};
