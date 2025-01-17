/*
 *
 * LinearR4.cpp, release 4.0.  August 30, 2019.
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.  Please acknowledge
 *   all use of the software in any publications or products based on it.
 *
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 */

#include "LinearR4.h"

#include <assert.h>

const VectorR4 VectorR4::Zero;
// Deprecated due to unsafeness of global initialization
//const VectorR4 VectorR4::UnitX( 1.0, 0.0, 0.0, 0.0);
//const VectorR4 VectorR4::UnitY( 0.0, 1.0, 0.0, 0.0);
//const VectorR4 VectorR4::UnitZ( 0.0, 0.0, 1.0, 0.0);
//const VectorR4 VectorR4::UnitW( 0.0, 0.0, 0.0, 1.0);
//const VectorR4 VectorR4::NegUnitX(-1.0, 0.0, 0.0, 0.0);
//const VectorR4 VectorR4::NegUnitY( 0.0,-1.0, 0.0, 0.0);
//const VectorR4 VectorR4::NegUnitZ( 0.0, 0.0,-1.0, 0.0);
//const VectorR4 VectorR4::NegUnitW( 0.0, 0.0, 0.0,-1.0);

const Matrix4x4 Matrix4x4::Identity(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);


// ******************************************************
// * VectorR4 class - math library functions			*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **

double VectorR4::MaxAbs() const
{
	register double m;
	m = (x>0.0) ? x : -x;
	if ( y>m ) m=y;
	else if ( -y >m ) m = -y;
	if ( z>m ) m=z;
	else if ( -z>m ) m = -z;
	if ( w>m ) m=w;
	else if ( -w>m ) m = -w;
	return m;
}

// ******************************************************
// * Matrix4x4 class - math library functions			*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **


void Matrix4x4::operator*= (const Matrix4x4& B)	// Matrix product
{
	double t1, t2, t3;		// temporary values
	t1 =  m11*B.m11 + m12*B.m21 + m13*B.m31 + m14*B.m41;
	t2 =  m11*B.m12 + m12*B.m22 + m13*B.m32 + m14*B.m42;
	t3 =  m11*B.m13 + m12*B.m23 + m13*B.m33 + m14*B.m43;
	m14 = m11*B.m14 + m12*B.m24 + m13*B.m34 + m14*B.m44;
	m11 = t1;
	m12 = t2;
	m13 = t3;

	t1 =  m21*B.m11 + m22*B.m21 + m23*B.m31 + m24*B.m41;
	t2 =  m21*B.m12 + m22*B.m22 + m23*B.m32 + m24*B.m42;
	t3 =  m21*B.m13 + m22*B.m23 + m23*B.m33 + m24*B.m43;
	m24 = m21*B.m14 + m22*B.m24 + m23*B.m34 + m24*B.m44;
	m21 = t1;
	m22 = t2;
	m23 = t3;

	t1 =  m31*B.m11 + m32*B.m21 + m33*B.m31 + m34*B.m41;
	t2 =  m31*B.m12 + m32*B.m22 + m33*B.m32 + m34*B.m42;
	t3 =  m31*B.m13 + m32*B.m23 + m33*B.m33 + m34*B.m43;
	m34 = m31*B.m14 + m32*B.m24 + m33*B.m34 + m34*B.m44;
	m31 = t1;
	m32 = t2;
	m33 = t3;

	t1 =  m41*B.m11 + m42*B.m21 + m43*B.m31 + m44*B.m41;
	t2 =  m41*B.m12 + m42*B.m22 + m43*B.m32 + m44*B.m42;
	t3 =  m41*B.m13 + m42*B.m23 + m43*B.m33 + m44*B.m43;
	m44 = m41*B.m14 + m42*B.m24 + m43*B.m34 + m44*B.m44;
	m41 = t1;
	m42 = t2;
	m43 = t3;
}

inline void ReNormalizeHelper ( double &a, double &b, double &c, double &d )
{
	register double scaleF = a*a+b*b+c*c+d*d;		// Inner product of Vector-R4
	scaleF = 1.0-0.5*(scaleF-1.0);
	a *= scaleF;
	b *= scaleF;
	c *= scaleF;
	d *= scaleF;
}

Matrix4x4& Matrix4x4::ReNormalize() {
	ReNormalizeHelper( m11, m21, m31, m41 );	// Renormalize first column
	ReNormalizeHelper( m12, m22, m32, m42 );	// Renormalize second column
	ReNormalizeHelper( m13, m23, m33, m43 );	// Renormalize third column
	ReNormalizeHelper( m14, m24, m34, m44 );	// Renormalize fourth column
	double alpha = 0.5*(m11*m12 + m21*m22 + m31*m32 + m41*m42);	//1st and 2nd cols
	double beta  = 0.5*(m11*m13 + m21*m23 + m31*m33 + m41*m43);	//1st and 3rd cols
	double gamma = 0.5*(m11*m14 + m21*m24 + m31*m34 + m41*m44);	//1st and 4nd cols
	double delta = 0.5*(m12*m13 + m22*m23 + m32*m33 + m42*m43);	//2nd and 3rd cols
	double eps   = 0.5*(m12*m14 + m22*m24 + m32*m34 + m42*m44);	//2nd and 4nd cols
	double phi   = 0.5*(m13*m14 + m23*m24 + m33*m34 + m43*m44);	//3rd and 4nd cols
	double temp1, temp2, temp3;
	temp1 = m11 - alpha*m12 - beta*m13 - gamma*m14;
	temp2 = m12 - alpha*m11 - delta*m13 - eps*m14;
	temp3 = m13 - beta*m11 - delta*m12 - phi*m14;
	m14 -= (gamma*m11 + eps*m12 + phi*m13);
	m11 = temp1;
	m12 = temp2;
	m13 = temp3;
	temp1 = m21 - alpha*m22 - beta*m23 - gamma*m24;
	temp2 = m22 - alpha*m21 - delta*m23 - eps*m24;
	temp3 = m23 - beta*m21 - delta*m22 - phi*m24;
	m24 -= (gamma*m21 + eps*m22 + phi*m23);
	m21 = temp1;
	m22 = temp2;
	m23 = temp3;
	temp1 = m31 - alpha*m32 - beta*m33 - gamma*m34;
	temp2 = m32 - alpha*m31 - delta*m33 - eps*m34;
	temp3 = m33 - beta*m31 - delta*m32 - phi*m34;
	m34 -= (gamma*m31 + eps*m32 + phi*m33);
	m31 = temp1;
	m32 = temp2;
	m33 = temp3;
	temp1 = m41 - alpha*m42 - beta*m43 - gamma*m44;
	temp2 = m42 - alpha*m41 - delta*m43 - eps*m44;
	temp3 = m43 - beta*m41 - delta*m42 - phi*m44;
	m44 -= (gamma*m41 + eps*m42 + phi*m43);
	m41 = temp1;
	m42 = temp2;
	m43 = temp3;
	return *this;
}

// ******************************************************
// * LinearMapR4 class - math library functions			*
// * * * * * * * * * * * * * * * * * * * * * * * * * * **


double LinearMapR4::Determinant () const		// Returns the determinant
{
	double Tbt34C12 = m31*m42-m32*m41;		// 2x2 subdeterminants
	double Tbt34C13 = m31*m43-m33*m41;
	double Tbt34C14 = m31*m44-m34*m41;
	double Tbt34C23 = m32*m43-m33*m42;
	double Tbt34C24 = m32*m44-m34*m42;
	double Tbt34C34 = m33*m44-m34*m43;

	double sd11 = m22*Tbt34C34 - m23*Tbt34C24 + m24*Tbt34C23;	// 3x3 subdeterminants
	double sd12 = m21*Tbt34C34 - m23*Tbt34C14 + m24*Tbt34C13;
	double sd13 = m21*Tbt34C24 - m22*Tbt34C14 + m24*Tbt34C12;
	double sd14 = m21*Tbt34C23 - m22*Tbt34C13 + m23*Tbt34C12;

	return ( m11*sd11 - m12*sd12 + m13*sd13 - m14*sd14 );
}

LinearMapR4 LinearMapR4::Inverse() const			// Returns inverse
{

	double Tbt34C12 = m31*m42-m32*m41;		// 2x2 subdeterminants
	double Tbt34C13 = m31*m43-m33*m41;
	double Tbt34C14 = m31*m44-m34*m41;
	double Tbt34C23 = m32*m43-m33*m42;
	double Tbt34C24 = m32*m44-m34*m42;
	double Tbt34C34 = m33*m44-m34*m43;
	double Tbt24C12 = m21*m42-m22*m41;		// 2x2 subdeterminants
	double Tbt24C13 = m21*m43-m23*m41;
	double Tbt24C14 = m21*m44-m24*m41;
	double Tbt24C23 = m22*m43-m23*m42;
	double Tbt24C24 = m22*m44-m24*m42;
	double Tbt24C34 = m23*m44-m24*m43;
	double Tbt23C12 = m21*m32-m22*m31;		// 2x2 subdeterminants
	double Tbt23C13 = m21*m33-m23*m31;
	double Tbt23C14 = m21*m34-m24*m31;
	double Tbt23C23 = m22*m33-m23*m32;
	double Tbt23C24 = m22*m34-m24*m32;
	double Tbt23C34 = m23*m34-m24*m33;

	double sd11 = m22*Tbt34C34 - m23*Tbt34C24 + m24*Tbt34C23;	// 3x3 subdeterminants
	double sd12 = m21*Tbt34C34 - m23*Tbt34C14 + m24*Tbt34C13;
	double sd13 = m21*Tbt34C24 - m22*Tbt34C14 + m24*Tbt34C12;
	double sd14 = m21*Tbt34C23 - m22*Tbt34C13 + m23*Tbt34C12;
	double sd21 = m12*Tbt34C34 - m13*Tbt34C24 + m14*Tbt34C23;	// 3x3 subdeterminants
	double sd22 = m11*Tbt34C34 - m13*Tbt34C14 + m14*Tbt34C13;
	double sd23 = m11*Tbt34C24 - m12*Tbt34C14 + m14*Tbt34C12;
	double sd24 = m11*Tbt34C23 - m12*Tbt34C13 + m13*Tbt34C12;
	double sd31 = m12*Tbt24C34 - m13*Tbt24C24 + m14*Tbt24C23;	// 3x3 subdeterminants
	double sd32 = m11*Tbt24C34 - m13*Tbt24C14 + m14*Tbt24C13;
	double sd33 = m11*Tbt24C24 - m12*Tbt24C14 + m14*Tbt24C12;
	double sd34 = m11*Tbt24C23 - m12*Tbt24C13 + m13*Tbt24C12;
	double sd41 = m12*Tbt23C34 - m13*Tbt23C24 + m14*Tbt23C23;	// 3x3 subdeterminants
	double sd42 = m11*Tbt23C34 - m13*Tbt23C14 + m14*Tbt23C13;
	double sd43 = m11*Tbt23C24 - m12*Tbt23C14 + m14*Tbt23C12;
	double sd44 = m11*Tbt23C23 - m12*Tbt23C13 + m13*Tbt23C12;


	register double detInv = 1.0/(m11*sd11 - m12*sd12 + m13*sd13 - m14*sd14);

	return( LinearMapR4( sd11*detInv, -sd12*detInv, sd13*detInv, -sd14*detInv,
						 -sd21*detInv, sd22*detInv, -sd23*detInv, sd24*detInv,
						 sd31*detInv, -sd32*detInv, sd33*detInv, -sd34*detInv,
						 -sd41*detInv, sd42*detInv, -sd43*detInv, sd44*detInv ) );
}

LinearMapR4& LinearMapR4::Invert() 			// Converts into inverse.
{
	double Tbt34C12 = m31*m42-m32*m41;		// 2x2 subdeterminants
	double Tbt34C13 = m31*m43-m33*m41;
	double Tbt34C14 = m31*m44-m34*m41;
	double Tbt34C23 = m32*m43-m33*m42;
	double Tbt34C24 = m32*m44-m34*m42;
	double Tbt34C34 = m33*m44-m34*m43;
	double Tbt24C12 = m21*m42-m22*m41;		// 2x2 subdeterminants
	double Tbt24C13 = m21*m43-m23*m41;
	double Tbt24C14 = m21*m44-m24*m41;
	double Tbt24C23 = m22*m43-m23*m42;
	double Tbt24C24 = m22*m44-m24*m42;
	double Tbt24C34 = m23*m44-m24*m43;
	double Tbt23C12 = m21*m32-m22*m31;		// 2x2 subdeterminants
	double Tbt23C13 = m21*m33-m23*m31;
	double Tbt23C14 = m21*m34-m24*m31;
	double Tbt23C23 = m22*m33-m23*m32;
	double Tbt23C24 = m22*m34-m24*m32;
	double Tbt23C34 = m23*m34-m24*m33;

	double sd11 = m22*Tbt34C34 - m23*Tbt34C24 + m24*Tbt34C23;	// 3x3 subdeterminants
	double sd12 = m21*Tbt34C34 - m23*Tbt34C14 + m24*Tbt34C13;
	double sd13 = m21*Tbt34C24 - m22*Tbt34C14 + m24*Tbt34C12;
	double sd14 = m21*Tbt34C23 - m22*Tbt34C13 + m23*Tbt34C12;
	double sd21 = m12*Tbt34C34 - m13*Tbt34C24 + m14*Tbt34C23;	// 3x3 subdeterminants
	double sd22 = m11*Tbt34C34 - m13*Tbt34C14 + m14*Tbt34C13;
	double sd23 = m11*Tbt34C24 - m12*Tbt34C14 + m14*Tbt34C12;
	double sd24 = m11*Tbt34C23 - m12*Tbt34C13 + m13*Tbt34C12;
	double sd31 = m12*Tbt24C34 - m13*Tbt24C24 + m14*Tbt24C23;	// 3x3 subdeterminants
	double sd32 = m11*Tbt24C34 - m13*Tbt24C14 + m14*Tbt24C13;
	double sd33 = m11*Tbt24C24 - m12*Tbt24C14 + m14*Tbt24C12;
	double sd34 = m11*Tbt24C23 - m12*Tbt24C13 + m13*Tbt24C12;
	double sd41 = m12*Tbt23C34 - m13*Tbt23C24 + m14*Tbt23C23;	// 3x3 subdeterminants
	double sd42 = m11*Tbt23C34 - m13*Tbt23C14 + m14*Tbt23C13;
	double sd43 = m11*Tbt23C24 - m12*Tbt23C14 + m14*Tbt23C12;
	double sd44 = m11*Tbt23C23 - m12*Tbt23C13 + m13*Tbt23C12;

	register double detInv = 1.0/(m11*sd11 - m12*sd12 + m13*sd13 - m14*sd14);

	m11 = sd11*detInv;
	m12 = -sd21*detInv;
	m13 = sd31*detInv;
	m14 = -sd41*detInv;
	m21 = -sd12*detInv;
	m22 = sd22*detInv;
	m23 = -sd32*detInv;
	m24 = sd42*detInv;
	m31 = sd13*detInv;
	m32 = -sd23*detInv;
	m33 = sd33*detInv;
	m34 = -sd43*detInv;
	m41 = -sd14*detInv;
	m42 = sd24*detInv;
	m43 = -sd34*detInv;
	m44 = sd44*detInv;

	return ( *this );
}

VectorR4 LinearMapR4::Solve(const VectorR4& u) const	// Returns solution
{												
	// Just uses Inverse() for now.
	return ( Inverse()*u );
}

// Multiply a VectorR3 postion by an affine transformation.
//     The w component of the VectorR3 object is treated as equal to 1.0.
void LinearMapR4::AffineTransformPosition(VectorR3& dest) const
{
    assert(IsAffine());
    double newX = dest.x*m11 + dest.y*m12 + dest.z * m13 + m14;
    double newY = dest.x*m21 + dest.y*m22 + dest.z * m23 + m24;
    double wInv = 1.0 / m44;
    dest.z = dest.x*m31 + dest.y*m32 + dest.z * m33 + m34;
    dest.z *= wInv;
    dest.x = newX * wInv;
    dest.y = newY * wInv;
}

// Multiply a VectorR3 direction vector by an affine transformation.
//     The w component of the VectorR3 object is treated as equal to 0.0.
void LinearMapR4::AffineTransformDirection(VectorR3& dest) const
{
    assert(IsAffine());
    double newX = dest.x*m11 + dest.y*m12 + dest.z * m13;
    double newY = dest.x*m21 + dest.y*m22 + dest.z * m23;
    dest.z = dest.x*m31 + dest.y*m32 + dest.z * m33;
    dest.x = newX;
    dest.y = newY;
}

// glOrtho, glFrustum, gluPerspective, gluLookAt functions
//  reproduce OpenGL functionality for the Projection/ModelView Matrices

LinearMapR4& LinearMapR4::Set_glOrtho(double left, double right,
	double bottom, double top,
	double near, double far)
{
	double bottomMinusTopInv = 1.0 / (bottom - top);
	double leftMinusRightInv = 1.0 / (left - right);
	double farMinusNearInv = 1.0 / (far - near);
	this->m11 = -2.0*leftMinusRightInv;
	this->m22 = -2.0*bottomMinusTopInv;
	this->m33 = -2.0*farMinusNearInv;
	this->m14 = (left+right)*leftMinusRightInv;
	this->m24 = (bottom+top)*bottomMinusTopInv;
	this->m34 = (near+far)*farMinusNearInv;
	this->m44 = 1.0;
	this->m21 = this->m31 = this->m41 = 0;
	this->m12 = this->m32 = this->m42 = 0;
	this->m13 = this->m23 = this->m43 = 0;
	return *this;
}

// Various scale, translation and rotation matrices,
//   to reproduce OpenGL ModelView matrix functionality
//   The "Mult" routines multiply on the right. (Like legacy OpenGL.)
//   The "Set" routines replace the matrix contents.
//   All routines return the *this matrix.
//   Most of them are inlined in the header file


LinearMapR4& LinearMapR4::Set_glRotate(double costheta, double sintheta, double x, double y, double z)
{
	double normSq = x * x + y * y + z * z;
	assert(normSq > 0.0);
	double normInv = 1.0 / sqrt(normSq);
	x *= normInv;
	y *= normInv;
	z *= normInv;
	double omC = 1 - costheta;
	double omCx = omC * x;
	double omCy = omC * y;
	double omCz = omC * z;
	m11 = omCx * x + costheta;
	m21 = omCx * y + sintheta * z;
	m31 = omCx * z - sintheta * y;
	m12 = omCy * x - sintheta * z;
	m22 = omCy * y + costheta;
	m32 = omCy * z + sintheta * x;
	m13 = omCz * x + sintheta * y;
	m23 = omCz * y - sintheta * x;
	m33 = omCz * z + costheta;
	m41 = m42 = m43 = m14 = m24 = m34 = 0.0;
	m44 = 1.0;
	return *this;
}


LinearMapR4& LinearMapR4::Set_glFrustum(double left, double right, double bottom, double top, double near, double far)
{
    double topMinusBottomInv = 1.0 / (top - bottom);
    double rightMinusLeftInv = 1.0 / (right - left);
    double nearMinusFarInv = 1.0 / (near - far);
    double twoN = 2.0*near;
    m11 = twoN * rightMinusLeftInv;
    m22 = twoN * topMinusBottomInv;
    m13 = (right + left) * rightMinusLeftInv;
    m23 = (top + bottom) * topMinusBottomInv;
    m33 = (far + near) * nearMinusFarInv;
    m43 = -1.0;
    m34 = far * twoN * nearMinusFarInv;
    m21 = m31 = m41 = m12 = m32 = m42 = m14 = m24 = 0.0;
    return *this;
}

LinearMapR4& LinearMapR4::Set_gluPerspective(double fieldofview_y_Radians, double aspectRatio, double zNear, double zFar)
{
    double upDown = zNear * tan(0.5*fieldofview_y_Radians);
    double leftRight = aspectRatio * upDown;
    return Set_glFrustum(-leftRight, leftRight, -upDown, upDown, zNear, zFar);
}

LinearMapR4& LinearMapR4::Set_gluLookAt(const VectorR3& eyePos, const VectorR3& lookAtPos, const VectorR3& upDir)
{
    VectorR3 toDir(eyePos);
    toDir -= lookAtPos;                                  // Vector from center to eye
    toDir.Normalize();
    m31 = toDir.x;
    m32 = toDir.y;
    m33 = toDir.z;

    VectorR3 upDirOrtho(upDir);
    upDirOrtho.AddScaled(toDir, -(upDir^toDir));   // Perpindicular to displace
    upDirOrtho.Normalize();
    m21 = upDirOrtho.x;
    m22 = upDirOrtho.y;
    m23 = upDirOrtho.z;
    VectorR3 rightDir(upDirOrtho);
    rightDir *= toDir;
    assert(rightDir.NormSq() > 1.0 - 1.0e10 && rightDir.NormSq() < 1.0 + 1.0e10);
    rightDir.ReNormalize();                             // Right-hand direction
    m11 = rightDir.x;
    m12 = rightDir.y;
    m13 = rightDir.z;

    m14 = -(eyePos^rightDir);
    m24 = -(eyePos^upDirOrtho);
    m34 = -(eyePos^toDir);
    m44 = 1.0;
    m41 = m42 = m43 = 0.0;
    return *this;
}



// ***************************************************************
// * 4-space vector and matrix utilities						 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// Returns u * v^T
LinearMapR4 TimesTranspose( const VectorR4& u, const VectorR4& v)
{
	LinearMapR4 result;
	TimesTranspose( u, v, result );
	return result;
}




// *********************************************************************
// Rotation routines												   *
// *********************************************************************

// Rotate unit vector x in the direction of "dir": length of dir is rotation angle.
//		x must be a unit vector.  dir must be perpindicular to x.
VectorR4& VectorR4::RotateUnitInDirection ( const VectorR4& dir)
{	
	assert ( this->Norm()<1.0001 && this->Norm()>0.9999 &&
				(dir^(*this))<0.0001 && (dir^(*this))>-0.0001 );

	double theta = dir.NormSq();
	if ( theta==0.0 ) {
		return *this;
	}
	else {
		theta = sqrt(theta);
		double costheta = cos(theta);
		double sintheta = sin(theta);
		VectorR4 dirUnit = dir/theta;
		*this = costheta*(*this) + sintheta*dirUnit;
		// this->NormalizeFast();
		return ( *this );
	}
}


// ***************************************************************
//  Stream Output Routines										 *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

ostream& operator<< ( ostream& os, const VectorR4& u )
{
	return (os << "<" << u.x << "," << u.y << "," << u.z << "," << u.w << ">");
}


