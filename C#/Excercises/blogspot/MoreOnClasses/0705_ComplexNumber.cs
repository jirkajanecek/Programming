using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0705
	{
		class ComplexNumber
		{
			int m_real;
			int m_imaginary;

			public ComplexNumber(
				int real,
				int imaginary
				)
			{
				m_real = real;
				m_imaginary = imaginary;
			}

			public int Real
			{
				get { return m_real ; }
				set { m_real = value; }
			}

			public int Imaginary
			{
				get { return m_imaginary; }
				set { m_imaginary = value; }
			}

			public override string ToString()
			{
				return "(" + m_real.ToString() + ", " + m_imaginary.ToString() + "i)";
			}

			public int GetMagnitude()
			{
				return (m_real * m_real) + (m_imaginary * m_imaginary);
			}

			public ComplexNumber Add(
				ComplexNumber other				
				)
			{
				return new ComplexNumber(this.Real + other.Real, this.Imaginary + other.Imaginary);
			}

			public static ComplexNumber operator +(
				ComplexNumber a,
				ComplexNumber b
				)
			{
				return a.Add(b);
			}
		}
	}
}
