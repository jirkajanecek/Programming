using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPFEvents
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();
			button.Click +=	Click_SecondBehavior;
		}

		private void Click_SecondBehavior(object sender, RoutedEventArgs e)
		{
			Console.WriteLine("Invisible handler clicked");
			return;
		}

		private void Click_ClickMeButton(object sender, RoutedEventArgs e)
		{
			myLabel.Content = "Hello world.";
			return;
		}
	}
}
