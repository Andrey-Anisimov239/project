#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<cmath>

const double EPS = 1e-6;
const double pi = 2 * acos(0);
using namespace std;

struct point
{
	double x, y;
	point(double xx, double yy) {
		x = xx;
		y = yy;
	}
	point() {
		x = 0;
		y = 0;
	}
};

struct line
{
	double a, b, c;
	line(double aa, double bb, double cc) {
		a = aa;
		b = bb;
		c = cc;
	}
	line() {
		a = 0;
		b = 0;
		c = 0;
	}
};

point operator + (point a, point b) { return point(a.x + b.x, a.y + b.y); };
point operator - (point a, point b) { return point(a.x - b.x, a.y - b.y); };
double operator ^ (point a, point b) { return a.x * b.x + a.y * b.y; };
double operator * (point a, point b) { return a.x * b.y - b.x * a.y; };
point operator * (double x, point a) { return point(x * a.x, x * a.y); };

void print(point a) {
	cout.precision(6);
	cout << a.x << " " << a.y << " ";
}


double len(point a) {
	return sqrt(a.x * a.x + a.y * a.y);
}


bool is_point_on_segment(point a, point b, point x) {
	if ((a - b) * (x - b) > EPS) {
		return 0;
	}
	if (((a - b) ^ (x - b)) < 0 || ((b - a) ^ (x - a)) < 0) {
		return 0;
	}
	return 1;
}

bool is_segments_intersected(point a, point b, point c, point d) {
	if (((c - a) * (b - a)) * ((d - a) * (b - a)) >= 0) {
		return 0;
	}
	if (((a - c) * (d - c)) * ((b - c) * (d - c)) >= 0) {
		return 0;
	}
	return 1;
}

line line_by_2_points(point a, point b) {
	double a1 = a.y - b.y;
	double b1 = b.x - a.x;
	double c1 = -(a1 * a.x + b1 * a.y);
	return line(a1, b1, c1);
}

line line_by_point_and_direction(point a, point v) {
	return line_by_2_points(a, a + v);
}

point perpendicular_vector(point v) {
	return point(-v.y, v.x);
}

point intersection_of_2_lines(line l1, line l2) {
	double y = -(l2.a * l1.c - l1.a * l2.c) / (l2.a * l1.b - l1.a * l2.b);
	double x = -(l2.b * l1.c - l1.b * l2.c) / (l2.b * l1.a - l1.b * l2.a);
	return point(x, y);
}

double angle(point a, point b, point c) {
	return acos(((b - a) ^ (b - c)) / (len(b - a) * len(b - c)));
}

point a, b, c;
point cut_on_bc, cut_on_ac, cut_on_ab;

bool check(double length_of_cuts) {
	point v = perpendicular_vector(a - b);
	point cut_ab1 = cut_on_ab + (length_of_cuts / (2 * len(v))) * v;
	point cut_ab2 = cut_on_ab - (length_of_cuts / (2 * len(v))) * v;
	v = perpendicular_vector(b - c);
	point cut_bc1 = cut_on_bc + (length_of_cuts / (2 * len(v))) * v;
	point cut_bc2 = cut_on_bc - (length_of_cuts / (2 * len(v))) * v;
	v = perpendicular_vector(a - c);
	point cut_ac1 = cut_on_ac + (length_of_cuts / (2 * len(v))) * v;
	point cut_ac2 = cut_on_ac - (length_of_cuts / (2 * len(v))) * v;
	if (is_segments_intersected(cut_ab1, cut_ab2, b, c)) {
		return 0;
	}
	if (is_segments_intersected(cut_ab1, cut_ab2, a, c)) {
		return 0;
	}
	if (is_segments_intersected(cut_bc1, cut_bc2, a, c)) {
		return 0;
	}
	if (is_segments_intersected(cut_bc1, cut_bc2, a, b)) {
		return 0;
	}
	if (is_segments_intersected(cut_ac1, cut_ac2, b, c)) {
		return 0;
	}
	if (is_segments_intersected(cut_ac1, cut_ac2, a, b)) {
		return 0;
	}
	if (is_segments_intersected(cut_ac1, cut_ac2, cut_ab1, cut_ab2)) {
		return 0;
	}
	if (is_segments_intersected(cut_ac1, cut_ac2, cut_bc1, cut_bc2)) {
		return 0;
	}
	if (is_segments_intersected(cut_ab1, cut_ab2, cut_bc1, cut_bc2)) {
		return 0;
	}
	cout << "YES" << endl;
	print(cut_bc1); print(cut_bc2);
	cout << endl;
	print(cut_ac1); print(cut_ac2);
	cout << endl;
	print(cut_ab1); print(cut_ab2);
	cout << endl;
	return 1;


}

bool best_place_for_cuts1(double lenght_of_cuts) {
	double x = tan(angle(a, b, c));
	double y = sin(angle(b, a, c));
	cut_on_ab = (y / (x + y)) * a + (x / (x + y)) * b;
	line l1 = line_by_point_and_direction(cut_on_ab, perpendicular_vector(a - c));
	line l2 = line_by_2_points(a, c);
	cut_on_ac = intersection_of_2_lines(l1, l2);
	l1 = line_by_point_and_direction(cut_on_ab, perpendicular_vector(a - b));
	l2 = line_by_2_points(b, c);
	cut_on_bc = intersection_of_2_lines(l1, l2);
	if (check(lenght_of_cuts)) {
		return 1;
	}
	x = tan(angle(a, b, c));
	y = sin(angle(b, c, a));
	cut_on_bc = (y / (x + y)) * b + (x / (x + y)) * c;
	l1 = line_by_point_and_direction(cut_on_bc, perpendicular_vector(b - a));
	l2 = line_by_2_points(a, b);
	cut_on_ab = intersection_of_2_lines(l1, l2);
	l1 = line_by_point_and_direction(cut_on_ab, perpendicular_vector(a - c));
	l2 = line_by_2_points(a, c);
	cut_on_ac = intersection_of_2_lines(l1, l2);
	if (check(lenght_of_cuts)) {
		return 1;
	}
	return 0;
}

bool is_triangle_obtuse(point a, point b, point c) {
	if (angle(a, b, c) >= pi / 2) {
		return 1;
	}
	if (angle(a, c, b) >= pi / 2) {
		return 1;
	}
	if (angle(b, a, c) >= pi / 2) {
		return 1;
	}
	return 0;
}

bool best_place_for_cuts2(double lenght_of_cuts) {
	double x = len(a - b);
	double y = len(c - b);
	cut_on_ac = (y / (x + y)) * a + (x / (x + y)) * c;
	line l1 = line_by_point_and_direction(cut_on_ac, perpendicular_vector(a - b));
	line l2 = line_by_2_points(a, b);
	cut_on_ab = intersection_of_2_lines(l1, l2);
	l1 = line_by_point_and_direction(cut_on_ac, perpendicular_vector(c - b));
	l2 = line_by_2_points(c, b);
	cut_on_bc = intersection_of_2_lines(l1, l2);
	if (check(lenght_of_cuts)) {
		return 1;
	}
	return 0;
}

vector<vector<int> > s_3 = { {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1},
{3, 1, 2}, {3, 2, 1} };

int main() {
	double x, y;
	vector<point> lst;
	for (int i = 0; i < 3; i++) {
		cin >> x >> y;
		lst.push_back(point(x, y));
	}
	double lenght_of_cuts;
	cin >> lenght_of_cuts;
	for (int i = 0; i < 6; i++) {
		a = lst[s_3[i][0] - 1];
		b = lst[s_3[i][1] - 1];
		c = lst[s_3[i][2] - 1];
		if (!is_triangle_obtuse(a, b, c)) {
			if (best_place_for_cuts1(lenght_of_cuts)) {
				return 1;
			}
			if (best_place_for_cuts2(lenght_of_cuts)) {
				return 1;
			}
		}
		if (angle(a, b, c) >= pi / 2) {
			if (best_place_for_cuts2(lenght_of_cuts)) {
				return 1;
			}
		}
	}
	cout << "NO";
	//print(cut_on_ac); print(cut_on_bc); print(cut_on_ab);
 	return 0;
}
