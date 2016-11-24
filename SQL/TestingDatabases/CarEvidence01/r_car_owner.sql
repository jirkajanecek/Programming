CREATE TABLE [dbo].[r_car_owner]
(
	[Id_car] INT NOT NULL,
	[Id_owner] INT NOT NULL,
	PRIMARY KEY (Id_car, Id_owner)
)