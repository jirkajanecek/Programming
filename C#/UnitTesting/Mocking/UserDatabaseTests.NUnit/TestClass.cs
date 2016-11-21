using NUnit.Framework;
using Moq;
using UserDatabase;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UserDatabaseTests.NUnit
{
	[TestFixture]
	public class TestClass
	{
		[Test]
		public void UserController_Add()
		{
			var mockDatabase = new Mock<IDatabase>();
			long userIdInit = 0;			
			mockDatabase.Setup(db => db.AddUser(It.IsAny<string>())).Returns(() => userIdInit).Callback(() => userIdInit++);

			UserController uController = new UserController(mockDatabase.Object);
			long id = uController.AddUser("John");
			Assert.AreEqual(id,0);
			id = uController.AddUser("Eve");
			Assert.AreEqual(id,1);
			id = uController.AddUser("Gogo");
			Assert.AreEqual(id, 2);
			id = uController.AddUser("Fofo");
			Assert.AreEqual(id, 3);
		}

		[Test]
		public void UserController_Remove()
		{
			var mockDatabase = new Mock<IDatabase>();			
			mockDatabase.Setup(db => db.CheckUser(It.IsInRange<long>(0, 3, Range.Inclusive))).Returns(true);

			UserController uController = new UserController(mockDatabase.Object);
			Assert.Throws(Is.InstanceOf<Exception>(), () => uController.RemoveUser(4));
			Assert.Throws(Is.InstanceOf<Exception>().And.Message.EqualTo("User not exists"), delegate { uController.RemoveUser(4); });
			Assert.DoesNotThrow(delegate { uController.RemoveUser(3); });
		}
	}
}
