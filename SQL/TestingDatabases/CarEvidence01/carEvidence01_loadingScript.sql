/*
Post-Deployment Script Template							
--------------------------------------------------------------------------------------
 This file contains SQL statements that will be appended to the build script.		
 Use SQLCMD syntax to include a file in the post-deployment script.			
 Example:      :r .\myfile.sql								
 Use SQLCMD syntax to reference a variable in the post-deployment script.		
 Example:      :setvar TableName MyTable							
               SELECT * FROM [$(TableName)]					
--------------------------------------------------------------------------------------
*/

USE CarEvidence01;

/* c_color table */

DELETE c_color;

SET IDENTITY_INSERT c_color ON;

INSERT INTO c_color (Id, Color) VALUES
(1,'White'),(2,'Black'),(3,'Red'),(4,'Blue'),(5,'Green'), (6,'Yellow');

SET IDENTITY_INSERT c_color OFF;

/* cars table */

DELETE cars;

SET IDENTITY_INSERT cars ON;

INSERT INTO cars (Id, Vin, id_color, DoorCount) VALUES
(1, '0000000000', 1, 5),
(2, '0000000001', 2, 5),
(3, '0000000002', 3, 3),
(4, '0000000003', 4, 3),
(5, '0000000004', 5, 5),
(6, '0000000005', 6, 5),
(7, '0000000006', 2, 5),
(8, '0000000007', 3, 5),
(9, '0000000008', 3, 3),
(10,'0000000009', 6, 3),
(11, '0000000010', 4, 5),
(12, '0000000011', 4, 5);

INSERT INTO cars (Id, Vin, id_color, DoorCount) VALUES (13, '0000000012', 4, 5);

SET IDENTITY_INSERT cars OFF;

/* owners table */

DELETE owners;

SET IDENTITY_INSERT owners ON;

INSERT INTO owners (Id, BirthNumber, Firstname, Surname) VALUES
(1, '9901024356', 'Joe', 'Doe'),
(2, '6507124351', 'Franta', 'Jetel'),
(3, '0002296666', 'Eva', 'Adam')

SET IDENTITY_INSERT owners OFF;

/* relation table owner -> cars */

DELETE r_car_owner;

INSERT INTO r_car_owner VALUES
(1,1),(2,1),(3,1),(4,2),(5,3),(6,2),(7,3),(8,1),(9,2),(10,3),(11,2),(12,1);
