#include "stdafx.h"
#include <iostream>

void DisplayField(int size, int *field){
	for(int z = 0; z < size * size; z++){
		if(field[z] == 1)
			std::cout << "x";
		else if(field[z] == -1)
			std::cout << "o";
		else std::cout << ".";
		if((z + 1) % size == 0){
			std::cout << "\n";
			continue;
		}
	}
}

int CheckForWinner(int size, int *field){
	int FieldsInARow = 0;
	for(int l = 0; l < size; l++){
		for(int c = 0; c < size; c++)
			FieldsInARow += field[l*size+c];
		if(FieldsInARow == size)
			return 1;
		else if (FieldsInARow == -size)
			return -1;
		FieldsInARow = 0;
	}
	for(int c = 0; c < size; c++){
		for(int l = 0; l < size; l++)
			FieldsInARow += field[l*size+c];
		if(FieldsInARow == size)
			return 1;
		else if (FieldsInARow == -size)
			return -1;
		FieldsInARow = 0;
	}
	for(int d1 = 0; d1 < size * size; d1 += size + 1)
		FieldsInARow += field[d1];
	if(FieldsInARow == size)
		return 1;
	else if (FieldsInARow == -size)
		return -1;
	FieldsInARow = 0;
	for(int d2 = size - 1; d2 < size * size; d2 += size - 1)
		FieldsInARow += field[d2];
	if(FieldsInARow == size)
		return 1;
	else if (FieldsInARow == -size)
		return -1;
	for(int i = 0; i < size * size; i++)
		if(field[i] == 0)
			return 2;
	return 0;
}

int SquareWorth(int size, int *field, int x){
	int Worth = 0;
	for(int a = x - x % size; a < x - x % size + size; a++)
		Worth += field[a];
	for(int b = x % size; b < x % size + size * size; b += size)
		Worth += field[b];
	Worth -= field[x];
	if(x / size == x % size){
		for(int c = 0; c < size * size; c += size + 1)
			Worth += field[c];
		Worth -= field[x];
	}
	if(x / size + x % size == size - 1){
		for(int d = size - 1; d <= size * (size - 1); d += size - 1)
			Worth += field[d];
		Worth -= field[x];
	}
	return Worth;
}

int DiagonalFieldValue(int size, int *field, int index){
	int Value = 0;
	for(int i = 0; i < size * size; i += size + 1)
		if(index == i){
			Value++;
			break;
		}
	for(int j = size - 1; j < size * size; j += size - 1)
		if(index == j){
			Value++;
			break;
		}
	return Value;
}

int SquareCalculation(int size, int *field, int side){
	int EmptyFields = 0;
	for(int k = 0; k < size * size; k++)
		if(field[k] == 0)
			EmptyFields++;
	if(EmptyFields == size * size){
		field[size / 2 * size + (size + 1) / 2 - 1] = side;
		return 0;
	}
	int MaxFields0AI = 0;
	int MaxFieldsAI = 0;
	int FieldNumberAI = 0;
	int FieldWorthAI = 0;
	int MaxFields0Human = 0;
	int MaxFieldsHuman = 0;
	int FieldNumberHuman = 0;
	int FieldWorthHuman = 0;
	bool skip = false;
	for(int l=0;l<size;l++){ //checking all lines
		skip=false;
		for(int h=0;h<size;h++){ //checking each line field
			if(field[l*size+h] == side*(-1)){
				for(int h1=0;h1<size;h1++){
					if(field[l*size+h1] == side){ //if ally and enemy field exists then skip line
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=0;h2<size;h2++){
					if(field[l*size+h2] == side*(-1)) //count enemy fields in line
						MaxFields0Human++;
				}
				if(MaxFields0Human >= MaxFieldsHuman){
					MaxFieldsHuman = MaxFields0Human;
					for(int h3=0;h3<size;h3++){ //count max empty field worth
						if(field[l*size+h3] == 0 && SquareWorth(size, field, l*size+h3) > FieldWorthHuman){
							FieldWorthHuman = SquareWorth(size, field, l*side+h3);
							FieldNumberHuman = l*size+h3;
						}
						else if(field[l*size+h3] == 0 && SquareWorth(size, field, l*size+h3) == FieldWorthHuman && DiagonalFieldValue(size, field, l*size+h3) > DiagonalFieldValue(size, field, FieldNumberHuman)){
							FieldWorthHuman = SquareWorth(size, field, l*side+h3);
							FieldNumberHuman = l*size+h3;
						}
					}
				}
				MaxFields0Human = 0;
				break;
			}
		}
		for(int h4=0;h4<size;h4++){ //check for no enemy fields in line
			if(field[l*size+h4] == side*(-1)){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e = 0; e < size; e++){
			if(field[l*size+e] == side)
				MaxFields0AI++;
		}
		if(MaxFields0AI >= MaxFieldsAI){
			MaxFieldsAI = MaxFields0AI;
			for(int e = 0; e < size; e++){
				if(field[l*size+e] == 0 && SquareWorth(size, field, l*size+e) < FieldWorthAI){
					FieldWorthAI = SquareWorth(size, field, l*size+e);
					FieldNumberAI = l*size+e;
				}
				else if(field[l*size+e] == 0 && SquareWorth(size, field, l*size+e) == FieldWorthAI && DiagonalFieldValue(size, field, l*size+e) > DiagonalFieldValue(size, field, FieldNumberAI)){
					FieldWorthAI = SquareWorth(size, field, l*size+e);
					FieldNumberAI = l*size+e;
				}
			}
		}
		MaxFields0AI = 0;
	}
	for(int c = 0; c < size; c++){ //checking all columns
		skip=false;
		for(int h=0;h<size;h++){
			if(field[c+size*h] == side*(-1)){
				for(int h1=0;h1<size;h1++){
					if(field[c+size*h1] == side){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=0;h2<size;h2++){
					if(field[c+size*h2] == side*(-1))
						MaxFields0Human++;
				}
				if(MaxFields0Human >= MaxFieldsHuman){
					MaxFieldsHuman = MaxFields0Human;
					for(int h3=0;h3<size;h3++){
						if(field[c+size*h3] == 0 && SquareWorth(size, field, c+size*h3) > FieldWorthHuman){
							FieldWorthHuman = SquareWorth(size, field, c+size*h3);
							FieldNumberHuman = c+size*h3;
						}
						else if(field[c+size*h3] == 0 && SquareWorth(size, field, c+size*h3) == FieldWorthHuman && DiagonalFieldValue(size, field, c+size*h3) > DiagonalFieldValue(size, field, FieldNumberHuman)){
							FieldWorthHuman = SquareWorth(size, field, c+size*h3);
							FieldNumberHuman = c+size*h3;
						}
					}
				}
				MaxFields0Human = 0;
				break;
			}
		}
		for(int h4=0;h4<size;h4++){
			if(field[c+size*h4] == side*(-1)){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e=0; e<size; e++)
			if(field[c+size*e] == side)
				MaxFields0AI++;
		if(MaxFields0AI >= MaxFieldsAI){
			MaxFieldsAI = MaxFields0AI;
			for(int e=0; e<size; e++){
				if(field[c+size*e] == 0 && SquareWorth(size, field, c+size*e) < FieldWorthAI){
					FieldWorthAI = SquareWorth(size, field, c+size*e);
					FieldNumberAI = c+size*e;
				}
				else if(field[c+size*e] == 0 && SquareWorth(size, field, c+size*e) == FieldWorthAI && DiagonalFieldValue(size, field, c+size*e) > DiagonalFieldValue(size, field, FieldNumberAI)){
					FieldWorthAI = SquareWorth(size, field, c+size*e);
					FieldNumberAI = c+size*e;
				}
			}
		}
		MaxFields0AI = 0;
	}
	for(int x=0; x<1; x++){ //checking right upper corner to left bottom corner diagonal
		skip=false;
		for(int h=size-1; h<=size*(size-1); h+=size-1){
			if(field[h] == side*(-1)){
				for(int h1=size-1; h1<=size*(size-1); h1+=size-1){
					if(field[h1] == side){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=size-1; h2<=size*(size-1); h2+=size-1){
					if(field[h2] == side*(-1))
						MaxFields0Human++;
				}
				if(MaxFields0Human >= MaxFieldsHuman){
					MaxFieldsHuman = MaxFields0Human;
					for(int h3=size-1; h3<=size*(size-1); h3+=size-1){
						if(field[h3] == 0 && SquareWorth(size, field, h3) > FieldWorthHuman){
							FieldWorthHuman = SquareWorth(size, field, h3);
							FieldNumberHuman = h3;
						}
						else if(field[h3] == 0 && SquareWorth(size, field, h3) == FieldWorthHuman && DiagonalFieldValue(size, field, h3) > DiagonalFieldValue(size, field, FieldNumberHuman)){
							FieldWorthHuman = SquareWorth(size, field, h3);
							FieldNumberHuman = h3;
						}
					}
				}
				MaxFields0Human = 0;
				break;
			}
		}
		for(int h4=size-1; h4<=size*(size-1); h4+=size-1){
			if(field[h4] == side*(-1)){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e=size-1; e<=size*(size-1); e+=size-1)
			if(field[e] == side)
				MaxFields0AI++;
		if(MaxFields0AI >= MaxFieldsAI){
			MaxFieldsAI = MaxFields0AI;
			for(int e=size-1; e<=size*(size-1); e+=size-1){
				if(field[e] == 0 && SquareWorth(size, field, e) < FieldWorthAI){
					FieldWorthAI = SquareWorth(size, field, e);
					FieldNumberAI = e;
				}
				else if(field[e] == 0 && SquareWorth(size, field, e) == FieldWorthAI && DiagonalFieldValue(size, field, e) > DiagonalFieldValue(size, field, FieldNumberAI)){
					FieldWorthAI = SquareWorth(size, field, e);
					FieldNumberAI = e;
				}
			}
		}
		MaxFields0AI = 0;
	}
	for(int x=0; x<1; x++){ //checking left upper corner to right bottom corner diagonal
		skip=false;
		for(int h=0; h<size*size; h+=size+1){
			if(field[h] == side*(-1)){
				for(int h1=0; h1<size*size; h1+=size+1){
					if(field[h1] == side){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=0; h2<size*size; h2+=size+1){
					if(field[h2] == side*(-1))
						MaxFields0Human++;
				}
				if(MaxFields0Human >= MaxFieldsHuman){
					MaxFieldsHuman = MaxFields0Human;
					for(int h3=0; h3<size*size; h3+=size+1){
						if(field[h3] == 0 && SquareWorth(size, field, h3) > FieldWorthHuman){
							FieldWorthHuman = SquareWorth(size, field, h3);
							FieldNumberHuman = h3;
						}
						else if(field[h3] == 0 && SquareWorth(size, field, h3) == FieldWorthHuman && DiagonalFieldValue(size, field, h3) > DiagonalFieldValue(size, field, FieldNumberHuman)){
							FieldWorthHuman = SquareWorth(size, field, h3);
							FieldNumberHuman = h3;
						}
					}
				}
				MaxFields0Human = 0;
				break;
			}
		}
		for(int h4=0; h4<size*size; h4+=size+1){
			if(field[h4] == side*(-1)){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e=0; e<size*size; e+=size+1)
			if(field[e] == side)
				MaxFields0AI++;
		if(MaxFields0AI >= MaxFieldsAI){
			MaxFieldsAI = MaxFields0AI;
			for(int e=0; e<size*size; e+=size+1){
				if(field[e] == 0 && SquareWorth(size, field, e) < FieldWorthAI){
					FieldWorthAI = SquareWorth(size, field, e);
					FieldNumberAI = e;
				}
				else if(field[e] == 0 && SquareWorth(size, field, e) == FieldWorthAI && DiagonalFieldValue(size, field, e) > DiagonalFieldValue(size, field, FieldNumberAI)){
					FieldWorthAI = SquareWorth(size, field, e);
					FieldNumberAI = e;
				}
			}
		}
		MaxFields0AI = 0;
	}
	for(int l=0;l<size;l++){ //checking all lines
		skip=false;
		for(int h=0;h<size;h++){
			if(field[l*size+h] == side){
				for(int h1=0;h1<size;h1++){
					if(field[l*size+h1] == side*(-1)){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=0;h2<size;h2++){
					if(field[l*size+h2] == side)
						MaxFields0AI++;
				}
				if(MaxFields0AI >= MaxFieldsAI){
					MaxFieldsAI = MaxFields0AI;
					for(int h3=0;h3<size;h3++){
						if(field[l*size+h3] == 0 && SquareWorth(size, field, l*size+h3) < FieldWorthAI){
							FieldWorthAI = SquareWorth(size, field, l*size+h3);
							FieldNumberAI = l*size+h3;
						}
						else if(field[l*size+h3] == 0 && SquareWorth(size, field, l*size+h3) == FieldWorthAI && DiagonalFieldValue(size, field, l*size+h3) > DiagonalFieldValue(size, field, FieldNumberAI)){
							FieldWorthAI = SquareWorth(size, field, l*size+h3);
							FieldNumberAI = l*size+h3;
						}
					}
				}
				MaxFields0AI = 0;
				break;
			}
		}
		for(int h4=0;h4<size;h4++){
			if(field[l*size+h4] == side){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e = 0; e < size; e++){
			if(field[l*size+e] == side*(-1))
				MaxFields0Human++;
		}
		if(MaxFields0Human >= MaxFieldsHuman){
			MaxFieldsHuman = MaxFields0Human;
			for(int e = 0; e < size; e++){
				if(field[l*size+e] == 0 && SquareWorth(size, field, l*size+e) > FieldWorthHuman){
					FieldWorthHuman = SquareWorth(size, field, l*size+e);
					FieldNumberHuman = l*size+e;
				}
				else if(field[l*size+e] == 0 && SquareWorth(size, field, l*size+e) == FieldWorthHuman && DiagonalFieldValue(size, field, l*size+e) > DiagonalFieldValue(size, field, FieldNumberHuman)){
					FieldWorthHuman = SquareWorth(size, field, l*size+e);
					FieldNumberHuman = l*size+e;
				}
			}
		}
		MaxFields0Human = 0;
	}
	for(int c = 0; c < size; c++){ //checking all columns
		skip=false;
		for(int h=0;h<size;h++){
			if(field[c+size*h] == side){
				for(int h1=0;h1<size;h1++){
					if(field[c+size*h1] == side*(-1)){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=0;h2<size;h2++){
					if(field[c+size*h2] == side)
						MaxFields0AI++;
				}
				if(MaxFields0AI >= MaxFieldsAI){
					MaxFieldsAI = MaxFields0AI;
					for(int h3=0;h3<size;h3++){
						if(field[c+size*h3] == 0 && SquareWorth(size, field, c+size*h3) < FieldWorthAI){
							FieldWorthAI = SquareWorth(size, field, c+size*h3);
							FieldNumberAI = c+size*h3;
						}
						else if(field[c+size*h3] == 0 && SquareWorth(size, field, c+size*h3) == FieldWorthAI && DiagonalFieldValue(size, field, c+size*h3) > DiagonalFieldValue(size, field, FieldNumberAI)){
							FieldWorthAI = SquareWorth(size, field, c+size*h3);
							FieldNumberAI = c+size*h3;
						}
					}
				}
				MaxFields0AI = 0;
				break;
			}
		}
		for(int h4=0;h4<size;h4++){
			if(field[c+size*h4] == side){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e=0; e<size; e++)
			if(field[c+size*e] == side*(-1))
				MaxFields0Human++;
		if(MaxFields0Human >= MaxFieldsHuman){
			MaxFieldsHuman = MaxFields0Human;
			for(int e=0; e<size; e++){
				if(field[c+size*e] == 0 && SquareWorth(size, field, c+size*e) > FieldWorthHuman){
					FieldWorthHuman = SquareWorth(size, field, c+size*e);
					FieldNumberHuman = c+size*e;
				}
				else if(field[c+size*e] == 0 && SquareWorth(size, field, c+size*e) == FieldWorthHuman && DiagonalFieldValue(size, field, c+size*e) > DiagonalFieldValue(size, field, FieldNumberHuman)){
					FieldWorthHuman = SquareWorth(size, field, c+size*e);
					FieldNumberHuman = c+size*e;
				}
			}
		}
		MaxFields0Human = 0;
	}
	for(int x=0; x<1; x++){ //checking right upper corner to left bottom corner diagonal
		skip=false;
		for(int h=size-1; h<=size*(size-1); h+=size-1){
			if(field[h] == side){
				for(int h1=size-1; h1<=size*(size-1); h1+=size-1){
					if(field[h1] == side*(-1)){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=size-1; h2<=size*(size-1); h2+=size-1){
					if(field[h2] == side)
						MaxFields0AI++;
				}
				if(MaxFields0AI >= MaxFieldsAI){
					MaxFieldsAI = MaxFields0AI;
					for(int h3=size-1; h3<=size*(size-1); h3+=size-1){
						if(field[h3] == 0 && SquareWorth(size, field, h3) < FieldWorthAI){
							FieldWorthAI = SquareWorth(size, field, h3);
							FieldNumberAI = h3;
						}
						else if(field[h3] == 0 && SquareWorth(size, field, h3) == FieldWorthAI && DiagonalFieldValue(size, field, h3) > DiagonalFieldValue(size, field, FieldNumberAI)){
							FieldWorthAI = SquareWorth(size, field, h3);
							FieldNumberAI = h3;
						}
					}
				}
				MaxFields0AI = 0;
				break;
			}
		}
		for(int h4=size-1; h4<=size*(size-1); h4+=size-1){
			if(field[h4] == side){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e=size-1; e<=size*(size-1); e+=size-1)
			if(field[e] == side*(-1))
				MaxFields0Human++;
		if(MaxFields0Human >= MaxFieldsHuman){
			MaxFieldsHuman = MaxFields0Human;
			for(int e=size-1; e<=size*(size-1); e+=size-1){
				if(field[e] == 0 && SquareWorth(size, field, e) > FieldWorthHuman){
					FieldWorthHuman = SquareWorth(size, field, e);
					FieldNumberHuman = e;
				}
				else if(field[e] == 0 && SquareWorth(size, field, e) == FieldWorthHuman && DiagonalFieldValue(size, field, e) > DiagonalFieldValue(size, field, FieldNumberHuman)){
					FieldWorthHuman = SquareWorth(size, field, e);
					FieldNumberHuman = e;
				}
			}
		}
		MaxFields0Human = 0;
	}
	for(int x=0; x<1; x++){ //checking left upper corner to right bottom corner diagonal
		skip=false;
		for(int h=0; h<size*size; h+=size+1){
			if(field[h] == side){
				for(int h1=0; h1<size*size; h1+=size+1){
					if(field[h1] == side*(-1)){
						skip=true;
						break;
					}
				}
				if(skip)
					break;
				for(int h2=0; h2<size*size; h2+=size+1){
					if(field[h2] == side)
						MaxFields0AI++;
				}
				if(MaxFields0AI >= MaxFieldsAI){
					MaxFieldsAI = MaxFields0AI;
					for(int h3=0; h3<size*size; h3+=size+1){
						if(field[h3] == 0 && SquareWorth(size, field, h3) < FieldWorthAI){
							FieldWorthAI = SquareWorth(size, field, h3);
							FieldNumberAI = h3;
						}
						else if(field[h3] == 0 && SquareWorth(size, field, h3) == FieldWorthAI && DiagonalFieldValue(size, field, h3) > DiagonalFieldValue(size, field, FieldNumberAI)){
							FieldWorthAI = SquareWorth(size, field, h3);
							FieldNumberAI = h3;
						}
					}
				}
				MaxFields0AI = 0;
				break;
			}
		}
		for(int h4=0; h4<size*size; h4+=size+1){
			if(field[h4] == side){
				skip=true;
				break;
			}
		}
		if(skip)
			continue;
		for(int e=0; e<size*size; e+=size+1)
			if(field[e] == side*(-1))
				MaxFields0Human++;
		if(MaxFields0Human >= MaxFieldsHuman){
			MaxFieldsHuman = MaxFields0Human;
			for(int e=0; e<size*size; e+=size+1){
				if(field[e] == 0 && SquareWorth(size, field, e) > FieldWorthHuman){
					FieldWorthHuman = SquareWorth(size, field, e);
					FieldNumberHuman = e;
				}
				else if(field[e] == 0 && SquareWorth(size, field, e) == FieldWorthHuman && DiagonalFieldValue(size, field, e) > DiagonalFieldValue(size, field, FieldNumberHuman)){
					FieldWorthHuman = SquareWorth(size, field, e);
					FieldNumberHuman = e;
				}
			}
		}
		MaxFields0Human = 0;
	}
	if(MaxFieldsAI >= MaxFieldsHuman){
		if(field[FieldNumberAI] == 0)
			field[FieldNumberAI] = side;
		return 0;
	}
	if(MaxFieldsAI < MaxFieldsHuman){
		if(field[FieldNumberHuman] == 0)
			field[FieldNumberHuman] = side;
		return 0;
	}
	else{
		for(int z=0; z<=size*size; z++){
			if(field[z] == 0){
				field[z] = side;
				return 0;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[]){
	std::cout << "Welcome to Noughts and Crosses!\n";
	std::cout << "Choose field size\n";
	int size;
	while(true){
		std::cin >> size;
		if(size > 2 || size < 6)
			break;
		std::cout << "Wrong field size\n";
	}
	int field[10000];
	for(int i=0; i<size*size; i++)
		field[i] = 0;
	std::cout << "Choose game mode: 1 - against AI, 2 - two players, 3 - AI vs AI (demo)\n";
	int mode;
	while(true){
		std::cin >> mode;
		if(mode == 1 || mode == 2 || mode == 3)
			break;
		std::cout << "Wrong option\n";
	}
	int side = 0;
	int i = 0; // line
	int j = 0; // column
	switch(mode) {
		case 1:
			std::cout << "Which side you want to play? (1 to play x, -1 to play o)\n";
			while(true){
				std::cin >> side;
				if(side == 1 || side == -1)
					break;
				std::cout << "Incorrect input\n";
			}
			std::cout << "Prepare for a game\n";
			std::cout << "Input i and j\n";
			while(true){
				if(side == 1){
					while(true){
						std::cin >> i >> j;
						if(field[(i-1)*size+j-1] != 0)
							std::cerr << "Wrong move\n";
						else{
							field[(i-1)*size+j-1] = side;
							side *= -1;
							break;
						}
					}
					DisplayField(size, field);
					if(CheckForWinner(size, field) == 1){
						std::cout << "Crosses won!\n";
						return 0;
					}
					else if(CheckForWinner(size, field) == -1){
						std::cout << "Noughts won!\n";
						return 0;
					}
					else if(CheckForWinner(size, field) == 0){
						std::cout << "Draw!\n";
						return 0;
					}
				}
				std::cout << "Let me think...\n";
				SquareCalculation(size, field, side);
				DisplayField(size, field);
				if(CheckForWinner(size, field) == 1){
					std::cout << "Crosses won!\n";
					return 0;
				}
				else if(CheckForWinner(size, field) == -1){
					std::cout << "Noughts won!\n";
					return 0;
				}
				else if(CheckForWinner(size, field) == 0){
					std::cout << "Draw!\n";
					return 0;
				}
				side *= -1;
			}
		case 2:
			side = 1;
			std::cout << "Prepare for a game\n";
			while(true){
				std::cout << "Input i and j\n";
				std::cin >> i >> j;
				if(field[(i-1)*size+j-1] != 0)
					std::cerr << "Wrong move\n";
				else{
					field[(i-1)*size+j-1] = side;
					side *= -1;
					DisplayField(size, field);
					if(CheckForWinner(size, field) == 1){
						std::cout << "Crosses won!\n";
						return 0;
					}
					else if(CheckForWinner(size, field) == -1){
						std::cout << "Noughts won!\n";
						return 0;
					}
					else if(CheckForWinner(size, field) == 0){
						std::cout << "Draw!\n";
						return 0;
					}
				}
			}
		case 3:
			side = 1;
			while(true){
				std::cout << "Let me think...\n";
				SquareCalculation(size, field, side);
				DisplayField(size, field);
				if(CheckForWinner(size, field) == 1){
					std::cout << "Crosses won!\n";
					return 0;
				}
				else if(CheckForWinner(size, field) == -1){
					std::cout << "Noughts won!\n";
					return 0;
				}
				else if(CheckForWinner(size, field) == 0){
					std::cout << "Draw!\n";
					return 0;
				}
				side *= -1;
			}
	}
	return 0;
}