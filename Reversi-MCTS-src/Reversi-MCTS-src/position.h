#pragma once

class position
{
public:
	position(int a=-1, int b=-1) {
		x = a;
		y = b;
		memset(dir, 0, sizeof(dir));
		//for (int i = 0; i < 8; i++)
		//	dir[i] = d[i];
	}
	bool operator<(const position &b) const {
		if (x < b.x) return true;
		else if (x > b.x) return false;
		else {
			if (y <= b.y) return true;
			else return false;
		}
	}
	int x;
	int y;
	int dir[8];//check availability on eight directions
};
