using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	/*
	 *	Create a class "PhotoAlbum" with a private attribute "numberOfPages." 
		It should also have a public method "GetNumberOfPages", which will return the number of pages. 
		The default constructor will create an album with 16 pages. There will be an additional constructor, with which we can specify the number of pages we want in the album. 
		Create a class "BigPhotoAlbum" whose constructor will create an album with 64 pages. 
		Create a test class "AlbumTest" to create an album with its default constructor, one with 24 pages, a "BigPhotoAlbum" and show the number of pages that the three albums have. 
	 */

	class PhotoAlbum
	{
		private int m_numberOfPages;

		public PhotoAlbum(
			)
		{
			m_numberOfPages = 16;
		}

		public PhotoAlbum(
			int numberOfPages
			)
		{
			m_numberOfPages = numberOfPages;
		}

		public int GetNumberOfPages()
		{
			return m_numberOfPages;
		}
	}

	class BigPhotoAlbum
		: PhotoAlbum
	{
		public BigPhotoAlbum() 
			: base(64)
		{

		}
	}

	class _0602
	{
		public static void process()
		{
			PhotoAlbum photoAlbum = new PhotoAlbum();
			PhotoAlbum photoAlbum24 = new PhotoAlbum(24);
			BigPhotoAlbum bigPhotoAlbum = new BigPhotoAlbum();

			Console.WriteLine("Number of pages: {0}", photoAlbum.GetNumberOfPages());
			Console.WriteLine("Number of pages: {0}", photoAlbum24.GetNumberOfPages());
			Console.WriteLine("Number of pages: {0}", bigPhotoAlbum.GetNumberOfPages());
		}
	}
}
