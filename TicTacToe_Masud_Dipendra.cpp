// TicTacToe.cpp : Defines the entry point for the console application.
//
/*
 Code by Dipendra & Masud
 * 
 */

#include<iostream>
#include<sstream>

#include<string>

using namespace std;


class Board {
	int squares[3][3];

public:
	Board();
	string toString();
	void play_square(int, int, int);
	int get_square(int, int);
	int winner();
	bool full_board();
};

Board::Board() {
	for(int i=0; i<3;i++)
		for(int j=0; j<3; j++)
			squares[i][j] = 0;
}

string Board::toString() {
	stringstream s;
	char cforvalplusone[] = {'O', '_', 'X'};
	s << " _ _ _" << endl;
	for(int i=0; i<3;i++) {
		s << '|';
		for(int j=0; j<3; j++)
			s << cforvalplusone[squares[i][j]+1] << '|';
		s << endl;
	}
	return s.str();
}

void Board::play_square(int row, int col, int val) {
	squares[row-1][col-1] = val;
}

bool Board::full_board() {
	for(int i=0; i<3;i++)
		for(int j=0; j<3; j++)
			if(squares[i][j]==0)
				return false;
	return true;
}

//returns winner or zero if none
int Board::winner() {
	//check rows:
	for(int row=0; row<3; row++)
		if(squares[row][0]!=0 && squares[row][0]==squares[row][1] && squares[row][0]==squares[row][2])
			return squares[row][0];
	//check cols:
	for(int col=0; col<3; col++)
		if(squares[0][col]!=0 && squares[0][col]==squares[1][col] && squares[0][col]==squares[2][col])
			return squares[0][col];
	//check diagonals:
	if(squares[0][0]!=0 && squares[0][0]==squares[1][1] && squares[0][0]==squares[2][2])
		return squares[0][0];
	if(squares[2][0]!=0 && squares[2][0]==squares[1][1] && squares[2][0]==squares[0][2])
		return squares[2][0];
	return 0;
}

int Board::get_square(int row, int col) {
	return squares[row-1][col-1];
}

bool cpu_move(Board, int);
int *min_decision(int**);
int *max_decision(int**);
int *alphabeta_min_decision(int**);
int *alphabeta_max_decision(int**);
int min_value(int**, int, int);
int max_value(int**, int, int);
bool terminal(int**);
int utility(int**);


bool cpu_move(Board * b, int cpuval) {
int **state, *action, flag=0;	
    
state = new int*[3];
for(int i=0; i<3;i++) {

state[i] = new int[3];
}

/*
 * Storing the state from board to the state array
 */
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			{
				state[i][j]=b->get_square(i+1,j+1);
				if(b->get_square(i+1,j+1)==0) flag=1;
			}
/*
 * Make move till board is not completely filled
 */		
	if(flag)
	{
		//cout<<"Am here"<<endl;
		if(cpuval==1) 
		action = alphabeta_max_decision(state);
		else
		action = alphabeta_min_decision(state);

		b->play_square(action[0]+1, action[1]+1, cpuval);
                return true;
	}

	return false;
}

/*
 * It returns the decision for cpu based on maximising from all values returned by min_value for all the non-filled slots
 * based on alpha-beta pruning
 */
int *alphabeta_max_decision(int** state)
{
int *action = new int[2];
action[0]=action[1]=-1;
int max = -100, min_val, a =-200, b =200;
for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                        {
                        if(state[i][j] ==0)
                                { 
                                state[i][j] = 1;
                                min_val = min_value(state, a, b);
                                state[i][j] = 0;
                                if(max<min_val)
                                        {
                                        max = min_val;
                                        action[0] = i;
                                        action[1] = j;
                                        }
                                }
                        }

return action;
}

/*
 * It returns the decision for cpu based on minimising from all values returned by max_value for all the non-filled slots
 * based on alpha-beta pruning
 */

int *alphabeta_min_decision(int** state)
{
int *action = new int[2];
action[0]=action[1]=-1;
int min = 100, max_val, a = -200, b = 200;
for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                        {
                        if (state[i][j]==0)
                                {
                                state[i][j]= -1;
                                max_val = max_value(state, a, b);
                                state[i][j]=  0;
                                if(min>max_val)
                                         {
                                        min = max_val;
                                        action[0] = i;
                                        action[1] = j;
                                         }
                                }
                        }
return action;
}


/*
 * It recursively finds the maximum from all values returned by min_value for all non-filled slots
 */
int max_value(int** state, int a, int b)
{
//cout<<"alpha is "<<a<<"and beta is "<<b<<endl;
int max= -100, min_val;
if (terminal(state) || utility(state) !=0)
	return utility(state);
//cout<<"In max val"<<endl;
for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                        {
				if(state[i][j] ==0)
				{
					state[i][j] = 1;
					min_val= min_value(state, a, b);
					if(max<min_val)
						max = min_val;	
					state[i][j] = 0;
					
					if(max>=b) return max;
					if(a<max) a=max;
				}	
			}	

return max;

}


/*
 * It recursively finds the minimum from all values returned by max_value for all non-flled slots
 */

int min_value(int** state, int a, int b)
{

int min = 100, max_val;
if (terminal(state) || utility(state) !=0 )
        return utility(state);
//cout<<"In min val"<<endl;
//cout<<"alpha is "<<a<<"and beta is "<<b<<endl;

for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                        {
                                if(state[i][j] ==0)
                                {
                                        state[i][j] = -1;
					max_val = max_value(state, a, b);
                                        if(min>max_val)
                                                min = max_val;
					state[i][j] =  0;
					
					 //if(b>min) b=min;	
					if(min<=a) return min;
                                       if(b>min) b=min;
                                }
                        }
return min;
}

/*
 * It returns whether we have reached a terminal state
 */
bool terminal(int** state)
{
int flag = 1;
for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                        {   
				if(state[i][j] == 0) flag = 0;
			}
if (flag) 
	return true; 
else
	return false;
}


/*
*  Calculates the utility based on the heuristic
*  0 for draws and 1 for when cpu wins and -1 for when human wins
*/
int utility(int** state)
{
//cout<<"In utility"<<endl;
	//check rows
for(int row=0; row<3; row++)

        if(state[row][0]!=0 && state[row][0]==state[row][1] && state[row][0]==state[row][2])
        return state[row][0];


        //check cols:
for(int col=0; col<3; col++)
        if(state[0][col]!=0 && state[0][col]==state[1][col] && state[0][col]==state[2][col])
        
	return state[0][col];

        //check diagonals:
        if(state[0][0]!=0 && state[0][0]==state[1][1] && state[0][0]==state[2][2])
                return state[0][0];
        if(state[2][0]!=0 && state[2][0]==state[1][1] && state[2][0]==state[0][2])
                return state[2][0];
        return 0;


}

void play() {
	Board * b = new Board();
	int humanPlayer = 1;
	int cpuPlayer = -1;
	int choice = 0;

	cout << b->toString();

	cout << "Do you want to play first (Enter 1) or let computer make the first move (Enter 2) "<<endl;
	cin >> choice;

	if(choice!=1) 	
	{
		humanPlayer = -1;
		cpuPlayer = 1;
		cpu_move(b, cpuPlayer);
                cout << b->toString();
	}

	while(!b->full_board()&&b->winner()==0) {
		int row, col;
		cout << "Your move row (1-3): ";
		cin >> row;
		cout << "Your move col (1-3): ";
		cin >> col;

		if(b->get_square(row, col)!=0) {
			cout << "Square already taken." << endl;
			continue;
		}
		else {
			b->play_square(row, col, humanPlayer);
			if(b->full_board() || b->winner()!=0)
				break;
			else {
				cout << b->toString() << "..." << endl;
				cpu_move(b, cpuPlayer);
				cout << b->toString();
			}
		}
	}
	if(b->winner()==0)
		cout << "Cat game." << endl;
	else if(b->winner()==cpuPlayer)
		cout << "Computer wins." << endl;
	else if(b->winner()==humanPlayer)
		cout << "You win." << endl;
	char a;
	cin >> a;
}

int main(int argc, char * argv[])
{
	play();
	return 0;
}

