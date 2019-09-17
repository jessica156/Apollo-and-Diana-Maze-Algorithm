#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <math.h>

using namespace std;
using namespace boost;

typedef property<edge_name_t, char> Property;
typedef adjacency_list<vecS, vecS, directedS, no_property, Property> Graph;
typedef graph_traits<Graph>::vertex_descriptor Graph_Vertex;

struct node_vertex
{
	int index;
	string vertex_color;
	string vertex_direction;
};

int main()
{
	int r = 0, c = 0;
	char delim1 = ' ';
	ifstream inFile;
	ofstream outFile;
	string line, node;

	inFile.open("input.txt");

	// Check to make sure file open correctly
	if (!inFile)
	{
		cout << "Error: Unable to open input file!";
		return 1;		// Terminate the program with error
	}

	// Read in the number of rows and columns and set r and c respectively
	getline(inFile, line);
	istringstream line1(line);
	line1 >> r >> c;

	vector<vector <node_vertex> > maze(r);

	// Read in input file and create a 2D maze
	for (int i = 0; i < r; ++i)
	{
		getline(inFile, line);
		istringstream line_stream(line);

		for (int j = 0; j < c; ++j)
		{
			getline(line_stream, node, delim1);
			istringstream node_stream(node);

			node_vertex temp;
			temp.index = (i*c) + j;

			string node_color = node.substr(0, 1);
			temp.vertex_color = node_color;

			if (node_color == "O")
			{
				temp.vertex_direction = "Exit";
			}
			else
			{
				string node_direction = node.substr(2);
				node_direction.erase(remove(node_direction.begin(), node_direction.end(), '\r'), node_direction.end());
				node_direction.erase(remove(node_direction.begin(), node_direction.end(), '\n'), node_direction.end());
				temp.vertex_direction = node_direction;
			}

			maze[i].push_back(temp);
		}
	}

	Graph adj_list;
	string color_to;
	string cell_direction;
	int i = 0, j = 0;

	vector< vector<node_vertex> >::iterator row = maze.begin();
	vector<node_vertex>::iterator col = row->begin();

	// Iterate through every row in the maze
	for (row = maze.begin(); row != maze.end(); ++row, ++i)
	{
		j = 0;

		// Iterate through every column in the maze
		for (col = row->begin(); col != row->end(); ++col, ++j)
		{
			if (col->vertex_color == "O")
				break;

			if (col->vertex_color == "R")
				color_to = "B";

			if (col->vertex_color == "B")
				color_to = "R";

			cell_direction = col->vertex_direction;

			// Iterate through all the cells that are in the direction of cell_direction and add edges between vertices 
			for (int curNode_y = i, curNode_x = j; (curNode_y >= 0) && (curNode_y < r) && (curNode_x >= 0) && (curNode_x < c);)
			{
				if ((maze[curNode_y][curNode_x].vertex_color == color_to) || (maze[curNode_y][curNode_x].vertex_color == "O"))
					add_edge(col->index, maze[curNode_y][curNode_x].index, adj_list);

				if (maze[i][j].vertex_direction == "N")
					--curNode_y;

				if (maze[i][j].vertex_direction == "E")
					++curNode_x;

				if (maze[i][j].vertex_direction == "S")
					++curNode_y;

				if (maze[i][j].vertex_direction == "W")
					--curNode_x;

				if (maze[i][j].vertex_direction == "NE")
				{
					++curNode_x;
					--curNode_y;
				}

				if (maze[i][j].vertex_direction == "SE")
				{
					++curNode_x;
					++curNode_y;
				}

				if (maze[i][j].vertex_direction == "SW")
				{
					--curNode_x;
					++curNode_y;
				}

				if (maze[i][j].vertex_direction == "NW")
				{
					--curNode_x;
					--curNode_y;
				}
			}
		}
	}

	// Call BFS on the graph adj_list and store the predecessors of each vertex in a vector
	vector<Graph_Vertex> PredList(r*c);
	breadth_first_search(adj_list, vertex(0, adj_list), visitor(make_bfs_visitor(record_predecessors(&PredList[0], on_tree_edge()))));

	// Get the path taken from the top left square of the maze to the bulls-eye and store it in a stack
	stack<Graph_Vertex> BFS_path;
	BFS_path.push(vertex(r*c - 1, adj_list));
	Graph_Vertex current = *(PredList.end() - 1);
	Graph_Vertex pred = PredList[current];
	for (;;)
	{
		BFS_path.push(current);
		current = pred;
		pred = PredList[current];
		if (current == 0)
			break;
	}
	BFS_path.push(current);

	// Print out path into output file
	outFile.open("output.txt");
	int vertex1, vertex2, row1, col1, row2, col2, distance;
	vertex1 = BFS_path.top();
	BFS_path.pop();
	while (!BFS_path.empty())
	{
		distance = 0;
		vertex2 = BFS_path.top();
		BFS_path.pop();

		// Calculate for the row and column numbers of the two vertices
		row1 = vertex1 / c;
		col1 = vertex1 % c;
		row2 = vertex2 / c;
		col2 = vertex2 % c;

		// Calculate the distance between the two vertices using the row and column numbers
		if (row1 == row2)
			distance = abs(col2 - col1);
		if (col1 == col2)
			distance = abs(row2 - row1);
		if (abs(row2 - row1) == abs(col2 - col1))
			distance = abs(row2 - row1);

		// Checking that the value of distance is valid
		assert(distance != 0);

		outFile << distance << maze[row1][col1].vertex_direction << " ";

		vertex1 = vertex2;
	}

	// Close files
	inFile.close();
	outFile.close();

	return 0;
}