/**************************************************************************
    Lighspark, a free flash player implementation

    Copyright (C) 2009  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <list>
#include <iostream>
#include <vector>

struct Numeric_Edge
{
	int a,b;
	int len;
	Numeric_Edge(int x, int y,int num)
	{
		if(((x-y+num)%num)<((y-x+num)%num))
		{
			a=y;
			b=x;
			len=((x-y+num)%num);
		}
		else
		{
			a=x;
			b=y;
			len=((y-x+num)%num);
		}
		if(len<2)
		{
			LOG(ERROR,"Edge should be at least of length 2");
		}
	}
	bool operator<(const Numeric_Edge& e) const
	{
		return len<e.len;
	}
};

class Path;
class Vector2;
class Shape;

class FilterIterator
{
private:
	std::vector<Vector2>::const_iterator it;
	std::vector<Vector2>::const_iterator end;
	int filter;
public:
	FilterIterator(const std::vector<Vector2>::const_iterator& i, const std::vector<Vector2>::const_iterator& e, int f);
	FilterIterator operator++(int);
	bool operator==(FilterIterator& i);
	bool operator!=(FilterIterator& i);
	const Vector2& operator*();
};

class Vector2
{
	friend bool pointInTriangle(const Vector2& P,const Vector2& A,const Vector2& B,const Vector2& C);
	friend class Edge;
	friend class Shape;
	friend int crossProd(const Vector2& a, const Vector2& b);
	friend std::ostream& operator<<(std::ostream& s, const Vector2& p);
	friend bool pointInPolygon(FilterIterator start, FilterIterator end, const Vector2& point);
	friend void TessellatePath(Path& path, Shape& shape);
	friend void TessellatePathSimple(Path& path, Shape& shape);
	friend void TriangulateMonotone(const std::list<Vector2>& monotone, Shape& shape);
	friend void SplitIntersectingPaths(std::vector<Path>& paths);
	int x,y;
public:
	int index;
	int chain;
	Vector2(int a, int b, int i):x(a),y(b),index(i){}
	Vector2(int a, int b):x(a),y(b),index(-1){}
	bool operator==(const Vector2& v){return v.x==x && v.y==y;}
	bool operator==(int i){return index==i;}
	bool operator<(const Vector2& v) const {return (y==v.y)?(x < v.x):(y < v.y);}
	bool operator<=(const Vector2& v) const {return y<=v.y;}
	bool operator>=(const Vector2& v) const {return y>=v.y;}
	bool operator<(int i) const {return index<i;}
	const Vector2 operator-(const Vector2& v)const { return Vector2(x-v.x,y-v.y);}
	const Vector2 operator+(const Vector2& v)const { return Vector2(x+v.x,y+v.y);}
	const Vector2 operator*(int p)const { return Vector2(x*p,y*p);}
	Vector2& operator/=(int v) { x/=v; y/=v; return *this;}
	int dot(const Vector2& r) const { return x*r.x+y*r.y;}

};

class Triangle
{
public:
	Vector2 v1,v2,v3;
	Triangle(Vector2 a,Vector2 b, Vector2 c):v1(a),v2(b),v3(c){}
};

class Edge
{
	friend class Shape;
private:
	int x1,x2;
	int y1,y2;
public:
	int index;
	Edge(const Vector2& a,const Vector2& b, int i):index(i)
	{
		y1=a.y;
		y2=b.y;
		x1=a.x;
		x2=b.x;
	}
	bool yIntersect(int y,int32_t& d,int x);
	bool xIntersect(int x,int32_t& d);
	bool edgeIntersect(const Edge& e);
	bool operator==(int a)
	{
		return index==a;
	}

};

class Graphic
{
public:
	bool filled0;
	bool filled1;
	bool stroked;
	RGBA color0;
	RGBA color1;
	RGBA stroke_color;
};

class Shape
{
public:
	std::vector<Triangle> interior;
	std::vector<Vector2> outline;
	Graphic graphic;
	bool closed;
	int winding;
	int id;

	//DEBUG
	//std::vector<Edge> edges;

	void Render() const;
};

#endif
