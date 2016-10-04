#include <vector>
#include <algorithm>
#include <limits>

void flood_fill::initialize_flood_fill(int n)
{
	int size = n * n;

	int x[size];
	int y[size];
	bool has_left[size];
	bool has_right[size];
	bool has_down[size];
	bool has_up[size];

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			x[i + n*(j-1)] = i+1;
			y[i + n*(j-1)] = j+1;

			if (i > 1)
			{
				has_left[i+n*(j-1)] = true;
			}
			if( i == 1)
			{
				has_left[i+n*(j-1)] = false;
			}

			if (i < n)
			{
				has_right[i+n*(j-1)] = true;
			}
			if( i == n)
			{
				has_left[i+n*(j-1)] = false;
			}

			if (j > 1)
			{
				has_down[i+n*(j-1)] = true;
			}
			if( j == 1)
			{
				has_down[i+n*(j-1)] = false;
			}

			if (j < n)
			{
				has_up[i+n*(j-1)] = true;
			}
			if( j == n)
			{
				has_up[i+n*(j-1)] = false;
			}
		}
	}

	//int island_sums;
	bool explored[size];// board_top = game.explored, //game.board_top

	int min_x;
	int max_x;
	int min_y;
	int max_y;
	int sum;

	bool p1_rw = false;
	bool p2_rw = false;
}

int flood_fill(int j, int player, vector<vector<pair<int, char>>> state, vector<bool>* explored)
{
	int sum;
	if (!(*explored)[j] && (state[j].back().first == player) && (state[j].back().second == 'F' || state[j].back().second == 'C'))
	{
		(*explored)[j] = true;
		sum = sum + 1;

		min_x = std::min(x[j],min_x);
		max_x = std::max(x[j],max_x);
		min_y = std::min(y[j],min_y);
		max_y = std::max(y[j],max_y);

		if (has_left[j] && !(explored[j-1]))
		{
			sum = sum + flood_fill(j - 1, player, state, explored);
		}

		if (has_right[j] && !(*explored)[j+1])
		{
			sum = sum + flood_fill(j + 1, player, state, explored);
		}

		if (has_down[j] && !(*explored)[j - n])
		{
			sum = sum + flood_fill(j - n, player, state, explored);
		}

		if (has_up[j] && !(*explored)[j + n])
		{
			sum = sum + flood_fill(j+n,player)
		}
	}
  return sum;
}

int eval_calculator(bool isMax)
{
	int dim1 = 0;
	int dim2 = 0;
	int dimsum1 = 0;
	int dimsum2 = 0;
	vector<int> p1_isles;
	vector<int> p2_isles;
	double strength_player1 = 0;
  double strength_player2 = 0;

	for (int i = 0; i < size; i++)
	{
		if !(explored[i])
		{
			if ((state[i].back().first == player) && (state[i].back().second == 'F' || state[i].back().second == 'C'))
			{
				int sum = 0;
				min_x = x[i];
				max_x = x[i];
				min_y = y[i];
				max_y = y[i];
				flood_fill(i,player);
				p1_isles.push_back(sum);/// resize
				strength_player1 = strength_player1 + Math.pow(sum,1.1);
				dim1 = std::max(max_y - min_y, dim1);
				dim1 = std::max(max_x - min_x, dim1);
				dimsum1 = dimsum1 + dim1;
			}
			if ((state[i].back().first == (1- player)) && (state[i].back().second == 'F' || state[i].back().second == 'C'))
			{
				sum = 0
				min_x = x[i];
				max_x = x[i];
				min_y = y[i];
				max_y = y[i];
				flood_fill(i,1-player);
				p2_isles.push_back(sum); //
				strength_player2 = strength_player2 + Math.pow(sum,1.1);
				dim2 = std::max(max_y - min_y, dim2);
				dim2 = std::max(max_x - min_x, dim2);
				dimsum2 = dimsum2 + dim2;
			}
		}
	}
	double eval1 = strength_player1 + 3*this->player_flats[player] + this->player_pieces[1-player] - 0.01*this->player_caps[player];
	double eval2 = strength_player2 + 3*this->player_flats[1-player] + this->player_pieces[player] - 0.01*this->player_caps[2-player];
	double evaluation = util_value_1 - util_value_2;

	if(dim1 == n - 1)
	{
		p1_rw = true;
		util_value_1 = 400 - ply;
		util_value_2 = 0;
	}
	if(dim2 == n - 1)
	{
		p2_rw = true;
		util_value_2 = 400 - ply;
		util_value_1 = 0;
	}

	if(maxplayeris == player)
	{
		return util_value;
	}

	if(maxplayeris == 1-player)
	{
		return -1*util_value;
	}

	if(!dim && !dim2)
	{
		return util_value;
	}
}
