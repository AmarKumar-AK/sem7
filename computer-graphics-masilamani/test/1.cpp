#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
// width and height of screen
int width = 1200, height = 600;
// Number of vertices
int num_vertices;
// Structure for edge constructed between 2 (x1, y1), (x2, y2)
/* ymax would be highest y coordinate of the edge
ymin is the lowest y coordinate of the edge
xmin would be the x coordinate of point whose y was ymin
xmin will change according to where we are now on the scanline
orxmin will be original value of xmin and doesn't change
delx - absolute difference of x1 and x2
dely - absolute difference of y1 and y2
sign_of_slope - Either 1 or -1 based on whether the line is like "/" or "\"
*/
struct edge {
	int ymax, xmin, ymin, orxmin;
	int delx, dely;
	int sign_of_slope;
};
// scanline[i] - Is a vector/array which contains all the edges whose ymin = i
vector<edge> scanline[645];
// highest y coordinate and lowest y coordinate in the polygon
int lowest_y = 100000;
int highest_y = -100000;

// Initialization of all gl parameters
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}

// given float point round it
int roundPt(float x) {
	return floor(x + 0.5);
}

// Display all the edges at scanline[index]
void display_edges(int index) {

	vector<edge> temp = scanline[index];
	if(temp.size() == 0)
		return;
	cout << "\nIndex " << index << "\n";
	for (int i = 0; i < temp.size(); i++) {
		cout << "\n\nymax = " << temp[i].ymax;
		cout << "\nxmin = " << temp[i].xmin;
		cout << "\ndelx = " << temp[i].delx;
		cout << "\ndely = " << temp[i].dely;
		cout << "\nsign = " << temp[i].sign_of_slope;
	}
	cout << "\n";
}

// Display the active edge table
void display_aet(vector<edge> &e) {

	if(e.size() == 0)
		return;
	
	for (int i = 0; i < e.size(); i++) {
		cout << "\n\nymax = " << e[i].ymax;
		cout << "\nxmin = " << e[i].xmin;
		cout << "\ndelx = " << e[i].delx;
		cout << "\ndely = " << e[i].dely;
		cout << "\nsign = " << e[i].sign_of_slope << "\n";
	}
}

void DrawPoint(int x, int y) {
	glBegin(GL_POINTS);
		glVertex3i(x, y, 0);
	glEnd();
	glFlush();
}

// returns the least y coordinate among 2 points p1 and p2
// int get_ymin(pair<int, int> p1, pair<int, int> p2) {
// 	if (p1.second < p2.second)
// 		return p1.second;
// 	else 
// 		return p2.second;
// }
// Utility function to create edge from points p1 and p2 where each
// point is a pair and has x coordinate value and y coordinate value
edge create_edge(pair<int, int> p1, pair<int, int> p2) {
	int x1 = p1.first;
	int y1 = p1.second;
	int x2 = p2.first;
	int y2 = p2.second;
	// assign parameters of edges given the points
	int tymax = max(y1, y2);
	int txmin = (y1<y2) ? x1 : x2;
	int sign_of_slope = 1;
	edge temp;

	temp.xmin = txmin;
	temp.ymax = tymax;
	
		// if line is vertical the sign of slope doesn't matter
		// since slope is just for updating the x coordinate 
		// which doesn't change for vectical lines
		if(x1 != x2) temp.sign_of_slope = ((float(y2 - y1) / float(x2 - x1)) < 0) ? -1 : 1;
		// if(x1 !=x2) {
		// 	// find sign of slope
		// 	// temp.sign_of_slope = ((float(y2 - y1) / float(x2 - x1)) < 0) ? -1 : 1;
		// 	if ((float(y2 - y1) / float(x2 - x1)) < 0) {
		// 		temp.sign_of_slope = -1;
		// 	}
		// 	else
		// 		temp.sign_of_slope = 1;
		// }
		temp.delx = abs(x2 - x1);
		temp.dely = abs(y2 - y1);

	temp.ymin = min(y1, y2);
	temp.orxmin = temp.xmin;
	return temp;

}

// given 2 points with equal y coordinate draw a horizontal line
void display_line(float x0, float y0, float x1, float y1) {
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}	
	int x = roundPt(x0);
	int y = roundPt(y0);

	for(int i = x0; i < x1; ++i) DrawPoint(i, y);

}

// Scanline starts
void driver() {
	cout << "\n Running driver function\n";
	// aet is the Active edge table comprising of active edges at any given time while traversing
	// from lowest y coordinate to highest y coordinate
	vector<edge> aet;
	
	for (int i = 0; i < 640; i++) {
		// Sort each scanline[i] edges based on xmin coordinate
		sort(scanline[i].begin(), scanline[i].end(), [](edge A,edge B){
			return A.xmin < B.xmin;
		});
	}
	
	// or i would be the current y coordinate which we are scanning at
	for (int i = lowest_y; i <= highest_y; i++) {
		// At each i update the active edge table
		for (int j = 0; j < aet.size(); j++) {
			// For all edges in aet we have finished drawing, remove them
			if (aet[j].ymax <= i) {
				aet.erase(aet.begin() + j);
				j--;
			}
			// If the edge is a vertical line no need to update xmin

			if (aet[j].delx == 0) {
				;
			}
			// Update the xmin using the line equation and the current y coordinate we are on
			// using x = x1 + (y1 - y)/m
			else {
				aet[j].xmin  = aet[j].orxmin + aet[j].sign_of_slope*(i - aet[j].ymin) / (float(aet[j].dely)/float(aet[j].delx));
			}
		}
		
		
		// At each y coordinate we are on, we have to insert all the new edges 
		// that have started from the current i, obtained from scanline[i]
		aet.insert(aet.end(), scanline[i].begin(), scanline[i].end());
		
		// Insertion might change the order of aet, so we sort it again based on
		// their x coordinates again
		sort(aet.begin(), aet.end(), [](edge A,edge B){
			return A.xmin < B.xmin;
		});

		// If there are atleast 2 edges in the active edge table, only then draw
		if (aet.size() >=2) {
			// Draw lines alternatively by skipping the next line 
			// we have to draw
			for (int l = 0; l < aet.size() - 1; l+=2) {
				int ptx1, pty1, ptx2, pty2;
				ptx1 = aet[l].xmin;
				ptx2 = aet[l+1].xmin;
				pty1 = i;
				pty2 = i;				
				display_line(ptx1, pty1, ptx2, pty2);

			}
		}
	}
	cout << "\n Finished\n";
 
}

// To clear and call driver function
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	driver();
	glEnd();
	glFlush();
}

int main(int argc, char** argv){
	// reading vertices
	freopen("input.txt", "r", stdin);
	int temp1, temp2, tempymin;
	edge temp;
	// storing points
	vector<pair<int, int>> points;
	// reading number of vertices from input file
	cin >> num_vertices;
	for (int i = 0; i < num_vertices; i++) {
		cin >> temp1 >> temp2;
		points.push_back(make_pair(temp1, temp2));
		// update the highest y value and lowest y value encountered till now
		lowest_y = min(temp2, lowest_y);
		highest_y = max(temp2, highest_y);
	}
	// Take points i and (i - 1) and create edge between them
	for (int i = 1; i < num_vertices; i++) {
		// Check if they have different y coordinates
		// not necessary to create horizontal edges as they would be draw 
		// because their end points would be a part of other edges
		if ((points[i].second - points[i-1].second)) {
			cout<<"points[i] : "<<points[i].first<<" , "<<points[i].second<<"  points[i-1] : "<<points[i-1].first<<" , "<<points[i-1].second<<endl;
			temp = create_edge(points[i], points[i-1]);
			cout<<"xmin : "<<temp.xmin<<"  ymax : "<<temp.ymax<<" delx : "<<temp.delx<<" dely : "<<temp.dely<<" ymin : "<<temp.ymin<<" orxmin : "<<temp.orxmin<<endl;
			// find the lowest y coordinate among the points 
			// insert that edge at the scanline at that point
			tempymin = min(points[i].second,points[i-1].second);
			cout<<"tepymin : "<<tempymin<<endl;
			// insert this edge to the scanline with index of least y
			scanline[tempymin].push_back(temp);
		}
	}
	// Seperate case for creating edge between 0 and (n - 1)
	if ((points[0].second - points[num_vertices-1].second)) {
		temp = create_edge(points[0], points[num_vertices-1]);
		tempymin = min(points[0].second,points[num_vertices-1].second);
		scanline[tempymin].push_back(temp);
	}

	
	// cout << "\nGlobal ymax " <<  highest_y << "\n";
	// cout << "\nGlobal ymin " << lowest_y << "\n";
	
	
	// gl initialisations
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("1st question");
	init();
	glutDisplayFunc(display);
	
	glutMainLoop();
	return 0;
}