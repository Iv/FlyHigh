/******************************************************************************
 *
 * Author      : Mario Konrad, Mario.Konrad@gmx.net
 * Adapted 2005 by Alex Graf, grafal@sourceforge.net
 *
 * Copyright (C) Mario Konrad
 *
 ******************************************************************************
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 ******************************************************************************/
 
#ifndef Vector_h
#define Vector_h

class Vector2
{
	public:
		Vector2(double x = 0.0, double y = 0.0);
		Vector2(const Vector2 &v);
		
		void set(double a, double b);
		void setCircle(double r, double rad);
		double length() const;
		
		double norm();
		void normalize();
		void rot(double rad);
		
		double x();
		double y();
	
		Vector2& operator = (const Vector2 &v);
		bool operator == (const Vector2 &v);
		Vector2& operator += (const Vector2 &v);
		Vector2& operator -= (const Vector2 & v);
		Vector2& operator *= (double f);
		double operator *= (const Vector2 &v);
		
		friend Vector2 operator + (const Vector2 &w, const Vector2 &v);
		friend Vector2 operator - (const Vector2 &w, const Vector2 &v);
		friend Vector2 operator * (const Vector2 &v, double f);
		friend Vector2 operator * (double f, const Vector2 &v);
		friend double operator * (const Vector2 &a, const Vector2 &b);
		friend double arc(const Vector2 &a, const Vector2 &b);

	private:
		double m_x;
		double m_y;
};

#endif
